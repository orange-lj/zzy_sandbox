#include "struct.h"
#include "ObjectData.h"
#include "Common.h"

LPOBJECT_DATA_INFO_HEAND g_ListHead__ObjectData = NULL;
BOOL g_ObjectData_Inited_ok = FALSE;

BOOL InitObjectData()
{
	BOOL bRet = FALSE;

	if (FALSE == g_ObjectData_Inited_ok)
	{
		bRet = ODCreateTotalHead((PVOID)&g_ListHead__ObjectData);
		g_ObjectData_Inited_ok = bRet;

		if (FALSE == bRet)
		{
			dprintf("error! | InitObjectData(); \n");
		}
	}

	return bRet;
}

BOOL ODCreateTotalHead(OUT PVOID* _TotalHead)
{
	BOOL bRet = FALSE;
	LPODHEAD* pTotalHead = (LPODHEAD*)_TotalHead;
	if (NULL != *pTotalHead) { return TRUE; }

	// Ϊ�ܽṹ������ڴ�
	*pTotalHead = (LPODHEAD)kmalloc(sizeof(ODHEAD));
	if (NULL == *pTotalHead)
	{
		dprintf("error! | ODCreateTotalHead(); Allocate memory failed! \n");
		return FALSE;
	}

	RtlZeroMemory(*pTotalHead, sizeof(ODHEAD));

	// ��ʼ����Դ��
	bRet = InitResource(&((LPODHEAD)*pTotalHead)->QueueLockList);
	if (FALSE == bRet)
	{
		dprintf("error! | ODCreateTotalHead() - InitResource(); ������Դ���ڴ�ʧ��! \n");
		kfree((PVOID)*pTotalHead);
		return FALSE;
	}

	// ��ʼ������ͷ
	InitializeListHead((PLIST_ENTRY) & ((LPODHEAD)*pTotalHead)->ListHead);

	((LPODHEAD)*pTotalHead)->nTotalCounts = 0;

	return TRUE;
}
