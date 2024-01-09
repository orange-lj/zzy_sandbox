#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <wtypes.h>
#include <windows.h>
#include <Winsvc.h>	// Ϊ��ʹ��SCM����

#ifdef __cplusplus
extern "C" {
#endif

class CDriver 
{
public:
	CDriver(LPCSTR pszDriverPath, LPCSTR pszLinkName);
// ����
	// ��������Ҳ����˵������DriverEntry������������
	virtual BOOL StartDriver();
	// ���豸����ȡ�õ���������һ�����
	virtual BOOL OpenDevice();
protected:
	char m_szLinkName[56];	// ������������
	char m_szSysName[64];

	BOOL m_bStarted;	// ָ�������Ƿ�����
	BOOL m_bCreateService;	// ָ���Ƿ񴴽��˷���

	HANDLE m_hSCM;		// SCM���ݿ���
	HANDLE m_hService;	// ������
	HANDLE m_hDriver;	// �豸���
};


#ifdef __cplusplus
}
#endif