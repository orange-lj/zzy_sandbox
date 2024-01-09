#include "struct.h"
#include "ProcessData.h"

LPPROCESS_NODE_INFO_HEAND g_ListHead__ProcessData = NULL;
static BOOL g_ProcessDataManager_Inited_ok = FALSE;


BOOL InitProcessData()
{
	BOOL bRet = FALSE;

	if (FALSE == g_ProcessDataManager_Inited_ok)
	{
		bRet = PDCreateTotalHead((PVOID)&g_ListHead__ProcessData);
		g_ProcessDataManager_Inited_ok = bRet;

		if (FALSE == bRet)
		{
			dprintf("error! | InitProcessData(); \n");
		}
	}

	return bRet;
}

BOOL PDCreateTotalHead(OUT PVOID* _TotalHead)
{
	BOOL bRet = FALSE;
	LPPDHEAD* pTotalHead = (LPPDHEAD*)_TotalHead;
	if (NULL != *pTotalHead) { return TRUE; }
	// 为总结构体分配内存
	*pTotalHead = (LPPDHEAD)kmalloc(sizeof(PDHEAD));
	if (NULL == *pTotalHead)
	{
		dprintf("error! | PDCreateTotalHead(); Allocate memory failed! \n");
		return FALSE;
	}
	RtlZeroMemory(*pTotalHead, sizeof(PDHEAD));
	// 初始化资源锁
	bRet = InitResource(&((LPPDHEAD)*pTotalHead)->QueueLockList);
	if (FALSE == bRet)
	{
		dprintf("error! | PDCreateTotalHead() - InitResource(); 申请资源锁内存失败! \n");
		kfree((PVOID)*pTotalHead);
		return FALSE;
	}
	// 初始化链表头
	InitializeListHead((PLIST_ENTRY) & ((LPPDHEAD)*pTotalHead)->ListHead);
	((LPPDHEAD)*pTotalHead)->nTotalCounts = 0;

	return TRUE;
}
