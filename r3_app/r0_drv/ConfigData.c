#include "struct.h"
#include "Config.h"
#include "ConfigThread.h"
#include "Common.h"
#include "Memory.h"
#include "ConfigData.h"


LPPB_CONFIG_TOTAL g_ProteinBox_Conf_TranshipmentStation = NULL;
LPPB_CONFIG_TOTAL g_ProteinBox_Conf = NULL;
LPPB_CONFIG_TOTAL g_ProteinBox_Conf_Old = NULL;
BOOL g_ProteinBox_Conf_Inited_ok = FALSE;


BOOL InitConfigData()
{
	BOOL bRet = FALSE;

	if ( FALSE == g_ProteinBox_Conf_Inited_ok )
	{
		if (g_ProteinBox_Conf_TranshipmentStation)
		{
			g_ProteinBox_Conf_Old = g_ProteinBox_Conf_TranshipmentStation;
			g_ProteinBox_Conf_TranshipmentStation = NULL;
		}

		bRet = CDCreateTotalHead((PVOID)&g_ProteinBox_Conf_TranshipmentStation);
		g_ProteinBox_Conf_Inited_ok = bRet;

		if (FALSE == bRet)
		{
			dprintf("error! | InitConfigData(); \n");
		}
	}

	return bRet;
}

BOOL CDCreateTotalHead(OUT PVOID* _TotalHead)
{
	BOOL bRet = FALSE;
	LPCDHEAD* pTotalHead = (LPCDHEAD*)_TotalHead;
	if (NULL != *pTotalHead) { return TRUE; }

	// Ϊ�ܽṹ������ڴ�
	*pTotalHead = (LPCDHEAD)kmalloc(sizeof(CDHEAD));
	if (NULL == *pTotalHead)
	{
		dprintf("error! | CDCreateTotalHead(); Allocate memory failed! \n");
		return FALSE;
	}

	RtlZeroMemory(*pTotalHead, sizeof(CDHEAD));

	// ��ʼ����Դ��
	bRet = InitResource(&((LPCDHEAD)*pTotalHead)->QueueLockList);
	if (FALSE == bRet)
	{
		dprintf("error! | CDCreateTotalHead() - InitResource(); ������Դ���ڴ�ʧ��! \n");
		kfree((PVOID)*pTotalHead);
		return FALSE;
	}

	// ��ʼ������ͷ
	InitializeListHead((PLIST_ENTRY) & ((LPCDHEAD)*pTotalHead)->SectionListHead);
	InitializeListHead((PLIST_ENTRY) & (((LPCDHEAD)*pTotalHead)->SectionListHead).KeyListHead);
	InitializeListHead((PLIST_ENTRY) & ((((LPCDHEAD)*pTotalHead)->SectionListHead).KeyListHead).ValueListHead);

	((LPCDHEAD)*pTotalHead)->SectionCounts = 0;

	return TRUE;
}
