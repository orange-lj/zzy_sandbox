#pragma once

#define LPSDHEAD LPREGISTRY_USERSID_INFO_HEAND
#define SDHEAD REGISTRY_USERSID_INFO_HEAND
typedef struct _REGISTRY_USERSID_INFO_ {

	/*0x000 */ struct _REGISTRY_USERSID_INFO_* pFlink; // �ϸ����
	/*0x004 */ struct _REGISTRY_USERSID_INFO_* pBlink; // �¸����

	/*0x008 */ ULONG StructSize;  // �ýṹ����ܴ�С
	/*0x00C */ ULONG Length_RegitryUserSID; // �ýṹ������ַ���1�ĳ���
	/*0x010 */ ULONG Length_CurrentUserName; // �ýṹ������ַ���2�ĳ���
	/*0x014 */ WCHAR RegitryUserSID[MAX_PATH]; // �ýṹ������ַ���1�ĵ�ַ
	/*0x018 */ WCHAR CurrentUserName[MAX_PATH]; // �ýṹ������ַ���2�ĵ�ַ, eg:"AV"

} REGISTRY_USERSID_INFO, * LPREGISTRY_USERSID_INFO;


//
// SID������ܽڵ�
//

typedef struct _REGISTRY_USERSID_INFO_HEAND_
{
	/*0x000 */ int			nTotalCounts;
	/*0x004 */ PERESOURCE	QueueLockList; // �����������
	/*0x008 */ REGISTRY_USERSID_INFO ListHead;

} REGISTRY_USERSID_INFO_HEAND, * LPREGISTRY_USERSID_INFO_HEAND;

extern LPREGISTRY_USERSID_INFO_HEAND g_ListHead__RegistryUserSID;
extern BOOL g_SecurityDataManager_Inited_ok;

BOOL
InitSecurityData(
);

BOOL
SDCreateTotalHead(
	OUT PVOID* _TotalHead
);