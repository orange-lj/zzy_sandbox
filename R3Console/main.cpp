#include <io.h>
#include <fcntl.h>

#include<stdio.h>
#include<conio.h>
#include<Shlwapi.h>
#include<tchar.h>
#include<windows.h>
#include<iostream>
using namespace std;

#pragma comment(lib,"Shlwapi.lib")

#define	ARRSIZEOF(x) (sizeof(x)/sizeof(x[0]))
#define LOADERNAME    _T("Start.exe")


TCHAR m_szRootPath[MAX_PATH];

BOOL RunPE(	IN PCWCHAR szPath,	IN LPCWSTR szCmdLine) 
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	if (NULL == szPath) { return FALSE; }
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if (!CreateProcessW(szPath, const_cast<LPWSTR>(szCmdLine), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		wprintf( L"[失败] 启动进程\"%ls\" \n", szPath);
		return FALSE;
	}

	wprintf( L"[成功] 启动进程\"%ls\" \n", szPath);
	WaitForSingleObject(pi.hProcess, 0xffffffff);
	CloseHandle(pi.hProcess);
	return TRUE;
}

void OnRunPE(IN LPCWSTR szCmdLine) 
{
	GetModuleFileName(NULL, m_szRootPath, ARRSIZEOF(m_szRootPath));
	PathRemoveFileSpecW(m_szRootPath);
	wchar_t szLoaderPath[MAX_PATH] = L"";
	PathCombineW(szLoaderPath, m_szRootPath, LOADERNAME);
	RunPE(szLoaderPath, szCmdLine);
	return;
}


int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	while (TRUE)
	{
		wstring index;
		wcout <<
			L"1.启动notepad" << endl <<
			L"======================" << endl <<

			L"请输入序号:";
		MSG msg;
		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
			{
				DispatchMessage(&msg);
			}
			else 
			{
				if (_kbhit()) 
				{
					break;
				}
				Sleep(10);
			}
		}
		wcin >> index;
		if (index.compare(L"1") == 0) 
		{
			OnRunPE(L"C:\\Windows\\notepad.exe");
		}
	}
	return 0;
}