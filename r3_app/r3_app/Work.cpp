#pragma once
#include "Work.h"

CDriver* g_drv_ProteinBoxDrv = NULL;
CConfigEx* g_Conf = NULL;


BOOL HandlerConf()
{
	BOOL bRet = FALSE;
	g_Conf = new CConfigEx();
	if (NULL == g_Conf) 
	{
		return FALSE;
	}

	return TRUE;
}

BOOL LoadDriver(IN LPSTR szDriverPath, IN LPSTR szDriverLinkName, CDriver** drv)
{
	//
	// 1. У������Ϸ���
	//

	if (NULL == szDriverPath || NULL == szDriverLinkName)
	{
		// printf( "error! | LoadDriver() | Invalid Paramaters; failed! \n" );
		return FALSE;
	}
	//
	// 2. ���� & �������豸
	//

	*drv = InitDriver(szDriverPath, szDriverLinkName);

	if (NULL == *drv)
	{
		// printf( "error! | LoadDriver() | Can't Load Driver:\"%ws\" \n", szDriverPath );
		return FALSE;
	}

	return TRUE;
}

CDriver* InitDriver(IN LPSTR szDriverPath, IN LPSTR szDriverLinkName)
{
	//
	// 1. У������Ϸ���
	//

	if (NULL == szDriverPath || NULL == szDriverLinkName)
	{
		// printf( "error! | InitDriver() | Invalid Paramaters; failed! \n" );
		return NULL;
	}
	// ��������
	CDriver* drv = new CDriver(szDriverPath, szDriverLinkName);
	if (!drv->StartDriver() || !drv->OpenDevice())
	{
		char sz[8] = "";
		sprintf(sz, "%d", ::GetLastError());
		//	MessageBox("Load Driver Failed",sz);
	}
	return drv;
}
