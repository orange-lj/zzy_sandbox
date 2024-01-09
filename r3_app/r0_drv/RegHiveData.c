#include "struct.h"
#include "RegHiveData.h"
#include "Common.h"


LPREGHIVE_NODE_INFO_HEAND g_ListHead__RegHive = NULL;
BOOL g_RegHive_Inited_ok = FALSE;

BOOL InitRegHive()
{
	BOOL bRet = FALSE;

	if (FALSE == g_RegHive_Inited_ok)
	{
		bRet = RHCreateTotalHead((PVOID)&g_ListHead__RegHive);
		g_RegHive_Inited_ok = bRet;

		if (FALSE == bRet)
		{
			dprintf("error! | InitRegHive(); \n");
		}
	}

	return bRet;
}

BOOL RHCreateTotalHead(OUT PVOID* _TotalHead)
{
	BOOL bRet = FALSE;
	LPRHHEAD* pTotalHead = (LPRHHEAD*)_TotalHead;
	if (NULL != *pTotalHead) { return TRUE; }

	// 为总结构体分配内存
	*pTotalHead = (LPRHHEAD)kmalloc(sizeof(RHHEAD));
	if (NULL == *pTotalHead)
	{
		dprintf("error! | RHCreateTotalHead(); Allocate memory failed! \n");
		return FALSE;
	}

	RtlZeroMemory(*pTotalHead, sizeof(RHHEAD));

	// 初始化资源锁
	bRet = InitResource(&((LPRHHEAD)*pTotalHead)->QueueLockList);
	if (FALSE == bRet)
	{
		dprintf("error! | RHCreateTotalHead() - InitResource(); 申请资源锁内存失败! \n");
		kfree((PVOID)*pTotalHead);
		return FALSE;
	}

	// 初始化链表头
	InitializeListHead((PLIST_ENTRY) & ((LPRHHEAD)*pTotalHead)->ListHead);

	((LPRHHEAD)*pTotalHead)->nTotalCounts = 0;

	return TRUE;
}
