#pragma once
#define LPCDHEAD	LPPB_CONFIG_TOTAL
#define CDHEAD		PB_CONFIG_TOTAL

typedef struct _SEARCH_INFO_
{
	/*0x000 */ PVOID pNode;
	/*0x004 */ ULONG NodeType;
} SEARCH_INFO, * LPSEARCH_INFO;


typedef struct _PB_CONFIG_COMMON_ {

	/*0x000 */ LIST_ENTRY List;
	/*+0x008*/ ULONG NameLength;	// 字符串长度
	/*+0x00C*/ LPWSTR wszName;		// 字符串内容的指针.

} PB_CONFIG_COMMON, * LPPB_CONFIG_COMMON;


typedef struct _PB_CONFIG_COMMONEX_ {

	/*0x000 */ LIST_ENTRY List;
	/*+0x008*/ ULONG NameLength;	// 字符串长度
	/*+0x00C*/ LPWSTR wszName;		// 字符串内容的指针.

	/*0x010 */ LIST_ENTRY SunList;

} PB_CONFIG_COMMONEX, * LPPB_CONFIG_COMMONEX;



typedef struct _PB_CONFIG_VALUE_ {

	/*0x000 */ struct _PB_CONFIG_VALUE_* pFlink; // 上个结点
	/*0x004 */ struct _PB_CONFIG_VALUE_* pBlink; // 下个结点

	/*+0x008*/ ULONG NameLength;	// 字符串长度
	/*+0x00C*/ LPWSTR ValueName;	// 字符串内容的指针. eg: c:\Test.exe

} PB_CONFIG_VALUE, * LPPB_CONFIG_VALUE;



typedef struct _PB_CONFIG_KEY_ {

	/*0x000 */ struct _PB_CONFIG_KEY_* pFlink; // 上个结点
	/*0x004 */ struct _PB_CONFIG_KEY_* pBlink; // 下个结点

	/*+0x008*/ ULONG NameLength;	// 字符串长度
	/*+0x00C*/ LPWSTR KeyName;		// 字符串内容的指针. eg: ForceProcess, OpenIpcPath

	/*0x010 */ PB_CONFIG_VALUE ValueListHead;

} PB_CONFIG_KEY, * LPPB_CONFIG_KEY;



typedef struct _PB_CONFIG_SECTION_
{
	/*0x000 */ struct _PB_CONFIG_SECTION_* pFlink; // 上个结点
	/*0x004 */ struct _PB_CONFIG_SECTION_* pBlink; // 下个结点

	/*+0x008*/ ULONG NameLength;		// 字符串长度
	/*+0x00C*/ LPWSTR SectionName;		// 字符串内容的指针. eg: [GlobalSetting], [UserSetting] 

	/*0x010 */ PB_CONFIG_KEY KeyListHead;

} PB_CONFIG_SECTION, * LPPB_CONFIG_SECTION;



typedef struct _PB_CONFIG_TOTAL_
{
	/*0x000 */ int			SectionCounts;	// Section的数量.
	/*0x004 */ PERESOURCE	QueueLockList; // 链表的自旋锁
	/*0x008 */ PB_CONFIG_SECTION SectionListHead;

} PB_CONFIG_TOTAL, * LPPB_CONFIG_TOTAL;

extern LPPB_CONFIG_TOTAL g_ProteinBox_Conf_TranshipmentStation;
extern LPPB_CONFIG_TOTAL g_ProteinBox_Conf;
extern LPPB_CONFIG_TOTAL g_ProteinBox_Conf_Old;
extern BOOL g_ProteinBox_Conf_Inited_ok;


BOOL
InitConfigData(
);

BOOL
CDCreateTotalHead(
	OUT PVOID* _TotalHead
);