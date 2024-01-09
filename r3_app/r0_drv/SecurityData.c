#include "struct.h"
#include "SecurityData.h"
#include "Common.h"

LPREGISTRY_USERSID_INFO_HEAND g_ListHead__RegistryUserSID = NULL;
BOOL g_SecurityDataManager_Inited_ok = FALSE;

BOOL InitSecurityData()
{
	BOOL bRet = FALSE;

	if (FALSE == g_SecurityDataManager_Inited_ok)
	{
		bRet = SDCreateTotalHead((PVOID)&g_ListHead__RegistryUserSID);
		g_SecurityDataManager_Inited_ok = bRet;

		if (FALSE == bRet)
		{
			dprintf("error! | InitSecurityData(); \n");
		}
	}

	return bRet;
}

BOOL SDCreateTotalHead(OUT PVOID* _TotalHead)
{
	BOOL bRet = FALSE;
	LPSDHEAD* pTotalHead = (LPSDHEAD*)_TotalHead;
	if (NULL != *pTotalHead) { return TRUE; }

	// Ϊ�ܽṹ������ڴ�
	*pTotalHead = (LPSDHEAD)kmalloc(sizeof(SDHEAD));
	if (NULL == *pTotalHead)
	{
		dprintf("error! | SDCreateTotalHead(); Allocate memory failed! \n");
		return FALSE;
	}

	RtlZeroMemory(*pTotalHead, sizeof(SDHEAD));

	// ��ʼ����Դ��
	bRet = InitResource(&((LPSDHEAD)*pTotalHead)->QueueLockList);
	if (FALSE == bRet)
	{
		dprintf("error! | SDCreateTotalHead() - InitResource(); ������Դ���ڴ�ʧ��! \n");
		kfree((PVOID)*pTotalHead);
		return FALSE;
	}

	// ��ʼ������ͷ
	InitializeListHead((PLIST_ENTRY) & ((LPSDHEAD)*pTotalHead)->ListHead);

	((LPSDHEAD)*pTotalHead)->nTotalCounts = 0;

	return TRUE;
}
