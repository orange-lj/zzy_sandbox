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

	// Ϊ�ܽṹ������ڴ�
	*pTotalHead = (LPRHHEAD)kmalloc(sizeof(RHHEAD));
	if (NULL == *pTotalHead)
	{
		dprintf("error! | RHCreateTotalHead(); Allocate memory failed! \n");
		return FALSE;
	}

	RtlZeroMemory(*pTotalHead, sizeof(RHHEAD));

	// ��ʼ����Դ��
	bRet = InitResource(&((LPRHHEAD)*pTotalHead)->QueueLockList);
	if (FALSE == bRet)
	{
		dprintf("error! | RHCreateTotalHead() - InitResource(); ������Դ���ڴ�ʧ��! \n");
		kfree((PVOID)*pTotalHead);
		return FALSE;
	}

	// ��ʼ������ͷ
	InitializeListHead((PLIST_ENTRY) & ((LPRHHEAD)*pTotalHead)->ListHead);

	((LPRHHEAD)*pTotalHead)->nTotalCounts = 0;

	return TRUE;
}
