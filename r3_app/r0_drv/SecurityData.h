#pragma once

#define LPSDHEAD LPREGISTRY_USERSID_INFO_HEAND
#define SDHEAD REGISTRY_USERSID_INFO_HEAND
typedef struct _REGISTRY_USERSID_INFO_ {

	/*0x000 */ struct _REGISTRY_USERSID_INFO_* pFlink; // 上个结点
	/*0x004 */ struct _REGISTRY_USERSID_INFO_* pBlink; // 下个结点

	/*0x008 */ ULONG StructSize;  // 该结构体的总大小
	/*0x00C */ ULONG Length_RegitryUserSID; // 该结构保存的字符串1的长度
	/*0x010 */ ULONG Length_CurrentUserName; // 该结构保存的字符串2的长度
	/*0x014 */ WCHAR RegitryUserSID[MAX_PATH]; // 该结构保存的字符串1的地址
	/*0x018 */ WCHAR CurrentUserName[MAX_PATH]; // 该结构保存的字符串2的地址, eg:"AV"

} REGISTRY_USERSID_INFO, * LPREGISTRY_USERSID_INFO;


//
// SID链表的总节点
//

typedef struct _REGISTRY_USERSID_INFO_HEAND_
{
	/*0x000 */ int			nTotalCounts;
	/*0x004 */ PERESOURCE	QueueLockList; // 链表的自旋锁
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