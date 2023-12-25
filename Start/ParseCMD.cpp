#include "ParseCMD.h"
#include "../Base/Ioctl.h"
#include"../Dll/Exportfunc.h"
#pragma comment(lib,"Dll.lib")
ParseCMD::ParseCMD()
{
}

ParseCMD::~ParseCMD()
{
}

BOOL ParseCMD::ParseCommandLine()
{
	ULONG length = 0;
	HRESULT hRet = E_OUTOFMEMORY;
	LPWSTR lpCmdLine = GetCommandLineW();
	LPWSTR lpCmdLineL = GetCommandLineWEx(lpCmdLine);

	return 0;
}

BOOL ParseCMD::StartCtrl()
{

	return TRUE;
}

LPWSTR ParseCMD::GetCommandLineWEx(IN LPWSTR lpCmdLine)
{
	WCHAR Context;
	BOOL bFlag = FALSE;
	LPWSTR ptr = lpCmdLine;
	while (*ptr == ' ') 
	{
		++ptr;
	}
	while (TRUE) 
	{
		Context = *ptr;
		if (!Context) 
		{ 
			break; 
		}
		if (Context == '\"') 
		{
			bFlag = (bFlag == 0);
		}
		else 
		{
			if (Context == ' ' && FALSE == bFlag) 
			{ 
				break; 
			}
		}
		++ptr;
	}
	while (*ptr == ' ') 
	{
		++ptr;
	}
	return ptr;
}

BOOL ParseCMD::INIGetConf(IN LPWSTR Key)
{
	ULONG nIndex = 0;
	BOOL bRet = FALSE;
	WCHAR Context[0x10] = L"";
	RPC_IN_HEADER pRpcInBuffer;
	LPRPC_OUT_HEADER pRpcOutBuffer = NULL;

	pRpcInBuffer.Flag = _PBSRV_APINUM_INIGetCurUserSection_;
	pRpcInBuffer.DataLength = 8;

	pRpcOutBuffer = (LPRPC_OUT_HEADER)DLL_CallServer(&pRpcInBuffer);
	if (NULL == pRpcOutBuffer) 
	{
	
	}
	if (pRpcOutBuffer) 
	{
		// 这里不用里面的内容,直接查询 [GlobalSetting]
		DLL_FreeReply(pRpcOutBuffer);
	}
	bRet = DLL_QueryConf(L"GlobalSetting", Key, 0x10, (PVOID)Context);
	if (bRet) 
	{
		if (0 == wcsicmp(Context, L"y") || 0 == wcsicmp(Context, L"1")) 
		{
			return TRUE;
		}
	}
	return FALSE;
}
