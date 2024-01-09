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

	// 为总结构体分配内存
	*pTotalHead = (LPSDHEAD)kmalloc(sizeof(SDHEAD));
	if (NULL == *pTotalHead)
	{
		dprintf("error! | SDCreateTotalHead(); Allocate memory failed! \n");
		return FALSE;
	}

	RtlZeroMemory(*pTotalHead, sizeof(SDHEAD));

	// 初始化资源锁
	bRet = InitResource(&((LPSDHEAD)*pTotalHead)->QueueLockList);
	if (FALSE == bRet)
	{
		dprintf("error! | SDCreateTotalHead() - InitResource(); 申请资源锁内存失败! \n");
		kfree((PVOID)*pTotalHead);
		return FALSE;
	}

	// 初始化链表头
	InitializeListHead((PLIST_ENTRY) & ((LPSDHEAD)*pTotalHead)->ListHead);

	((LPSDHEAD)*pTotalHead)->nTotalCounts = 0;

	return TRUE;
}
