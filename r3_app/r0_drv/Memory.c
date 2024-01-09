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

	// Ϊ�ܽṹ�����ڴ�
	*pTotalHead = (LPMMHEAD)kmalloc(sizeof(MMHEAD));
	if (NULL == *pTotalHead)
	{
		dprintf("error! | MMCreateTotalHead(); Allocate memory failed! \n");
		return FALSE;
	}

	RtlZeroMemory(*pTotalHead, sizeof(MMHEAD));

	// ��ʼ����Դ��
	bRet = InitResource(&((LPMMHEAD)*pTotalHead)->QueueLockList);
	if (FALSE == bRet)
	{
		dprintf("error! | MMCreateTotalHead() - InitResource(); ������Դ���ڴ�ʧ��! \n");
		kfree((PVOID)*pTotalHead);
		return FALSE;
	}

	// ��ʼ������ͷ
	InitializeListHead(&((LPMMHEAD)*pTotalHead)->ListHead.MemoryListEntry);

	((LPMMHEAD)*pTotalHead)->nTotalCounts = 0;

	return TRUE;
}
