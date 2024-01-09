#include "struct.h"
#include"Common.h"
#include "Memory.h"

LPMMHEAD g_ListHead__MemoryManager = NULL;
BOOL g_MemoryManager_Inited_ok = FALSE;

BOOL InitMemoryManager()
{
	BOOL bRet = FALSE;
	if (FALSE == g_MemoryManager_Inited_ok)
	{
		bRet = MMCreateTotalHead((PVOID)&g_ListHead__MemoryManager);
		g_MemoryManager_Inited_ok = bRet;

		if (FALSE == bRet)
		{
			dprintf("error! | InitMemoryManager(); \n");
		}
	}
}

BOOL MMCreateTotalHead(OUT PVOID* _TotalHead)
{
	BOOL bRet = FALSE;
	LPMMHEAD* pTotalHead = (LPMMHEAD*)_TotalHead;
	if (NULL != *pTotalHead) { return TRUE; }

	// 为总结构分配内存
	*pTotalHead = (LPMMHEAD)kmalloc(sizeof(MMHEAD));
	if (NULL == *pTotalHead)
	{
		dprintf("error! | MMCreateTotalHead(); Allocate memory failed! \n");
		return FALSE;
	}

	RtlZeroMemory(*pTotalHead, sizeof(MMHEAD));

	// 初始化资源锁
	bRet = InitResource(&((LPMMHEAD)*pTotalHead)->QueueLockList);
	if (FALSE == bRet)
	{
		dprintf("error! | MMCreateTotalHead() - InitResource(); 申请资源锁内存失败! \n");
		kfree((PVOID)*pTotalHead);
		return FALSE;
	}

	// 初始化链表头
	InitializeListHead(&((LPMMHEAD)*pTotalHead)->ListHead.MemoryListEntry);

	((LPMMHEAD)*pTotalHead)->nTotalCounts = 0;

	return TRUE;
}
