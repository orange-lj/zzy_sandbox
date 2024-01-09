#include "CLoadDriver.h"

CDriver::CDriver(LPCSTR pszDriverPath, LPCSTR pszLinkName)
{
	strncpy(m_szLinkName, pszLinkName, 55);
	m_bStarted = FALSE;
	m_bCreateService = FALSE;
	m_hSCM = m_hService = NULL;
	m_hDriver = INVALID_HANDLE_VALUE;


	// ��ȫģʽ������

	//�޸�SafeBoot,ʹ�ð�ȫģʽ��Ҳ���Լ���

	HKEY hLicenses = NULL;
	HKEY RegKey;
	DWORD disp;
	LONG Regrt = RegOpenKeyExA(
		HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal",
		0,
		KEY_ALL_ACCESS,
		&hLicenses);

	strcpy(m_szSysName, strrchr(pszDriverPath, '\\') + 1);

	Regrt = RegCreateKeyExA(
		hLicenses,
		m_szSysName,
		0,
		"",
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&RegKey,
		&disp);
	CloseHandle(hLicenses);

	// ��SCM������
	m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (m_hSCM == NULL)
	{
		MessageBoxA(0, "�򿪷�����ƹ�����ʧ��\n",
			"��������Ϊ����ӵ��AdministratorȨ��\n", 0);
		return;
	}

	// ������򿪷���
	m_hService = ::CreateServiceA((SC_HANDLE)m_hSCM, m_szLinkName, m_szLinkName, SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		pszDriverPath, NULL, 0, NULL, NULL, NULL);
	if (m_hService == NULL)
	{

		// ��������ʧ�ܣ���������Ϊ�����Ѿ����ڣ����Ի�Ҫ��ͼ����
		int nError = ::GetLastError();
		if (nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			m_hService = ::OpenServiceA((SC_HANDLE)m_hSCM, m_szLinkName, SERVICE_ALL_ACCESS);
		}
	}
	else
	{
		m_bCreateService = TRUE;
	}
}

BOOL CDriver::StartDriver()
{
	if (m_bStarted)
		return TRUE;
	if (m_hService == NULL)
	{
		return FALSE;
	}

	char szShow[256];
	sprintf(szShow, "%X", m_hService);
	// ��������
	if (!::StartService((SC_HANDLE)m_hService, 0, NULL))
	{
		int nError = ::GetLastError();
		if (nError == ERROR_SERVICE_ALREADY_RUNNING)
			m_bStarted = TRUE;
		else
			::DeleteService((SC_HANDLE)m_hService);
	}
	else
	{
		// �����ɹ��󣬵ȴ������������״̬
		int nTry = 0;
		SERVICE_STATUS ss;
		::QueryServiceStatus((SC_HANDLE)m_hService, &ss);
		while (ss.dwCurrentState == SERVICE_START_PENDING && nTry++ < 80)
		{
			::Sleep(50);
			::QueryServiceStatus((SC_HANDLE)m_hService, &ss);
		}
		if (ss.dwCurrentState == SERVICE_RUNNING)
			m_bStarted = TRUE;
	}


	//�����������,ɾ��ע������������

	HKEY hKey;
	::RegOpenKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services", &hKey);
	HKEY hSubKey;
	::RegOpenKeyA(hKey, m_szLinkName, &hSubKey);
	::RegDeleteKeyA(hSubKey, "Enum");
	::CloseHandle(hSubKey);
	RegDeleteKeyA(hKey, m_szLinkName);

	HKEY hSafeBoot;
	::RegOpenKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal", &hSafeBoot);
	::RegDeleteKeyA(hSafeBoot, m_szSysName);
	::CloseHandle(hSafeBoot);

	return m_bStarted;
}

BOOL CDriver::OpenDevice()
{
	if (m_hDriver != INVALID_HANDLE_VALUE)
		return TRUE;

	// "\\.\"��Win32�ж��屾�ؼ�����ķ�����
	// m_szLinkName���豸����ķ����������ƣ������½ڻ���ϸ����
	char sz[256] = "";
	sprintf(sz, "\\\\.\\%s", m_szLinkName);
	// �����������������豸
	m_hDriver = ::CreateFileA(sz,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return (m_hDriver != INVALID_HANDLE_VALUE);
}
