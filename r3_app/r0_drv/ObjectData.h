#pragma once

#define LPODHEAD LPOBJECT_DATA_INFO_HEAND

#define ODHEAD OBJECT_DATA_INFO_HEAND

#define ObjecTypeLength 0x20	// �����������ĳ���

typedef struct _OBJECT_INIT_
{
	/*0x000 */ ULONG ObjectIndex;
	/*0x004 */ WCHAR szObjecType[ObjecTypeLength]; // ����������,eg:"Process"
	/*0x008 */ ULONG FakeAddrOpen;		// ��������ַ
	/*0x00C */ ULONG FakeAddrParse;	// ��������ַ
	/*0x010 */ ULONG TypeInfoOffset;
	/*0x014 */ ULONG ProcedureOffsetOpen;
	/*0x018 */ ULONG ProcedureOffsetParse;

} OBJECT_INIT, * LPOBJECT_INIT;


typedef struct _OBHOOKDATA_LITTLE_
{
	/*0x000 */ ULONG FakeAddr;		// ��������ַ
	/*0x004 */ ULONG OrignalAddr;	// ԭʼObjectָ��

} OBHOOKDATA_LITTLE, * LPOBHOOKDATA_LITTLE;


typedef struct _OBHOOKDATA_
{
	/*0x000 */ ULONG ObjectIndex;
	/*0x004 */ OBHOOKDATA_LITTLE Open;
	/*0x008 */ OBHOOKDATA_LITTLE Parse;

} OBHOOKDATA, * LPOBHOOKDATA;


//
// ����Oject Hook��صĴ�ṹ
//

typedef struct _OBJECT_DATA_LITTLE_ // size - 0x18
{
	/*0x000 */ BOOL bCare; // �Ƿ��ע��ǰ�ڵ�

	/*0x004 */ ULONG FakeAddr;		// ��������ַ
	/*0x008 */ ULONG OrignalAddr;	// ԭʼObjectָ��
	/*0x01C */ ULONG UnhookPoint;	// ж��ObjectHookʱҪ�޸ĵ��ڴ��ַ
	/*0x010 */ BOOL	 bHooked;		// �Ƿ�ɹ�Hook	
	/*0x018 */ ULONG ProcedureOffset; // ��λObjectָ��ʱ��ƫ��,�����ڲ���ϵͳ�汾

	/*
	  ��XPΪ��,����� TypeInfoOffsetΪ+0x060; ProcedureOffsetΪ +0x03c
	  nt!_OBJECT_TYPE -r // ������������
		+0x060 TypeInfo         : _OBJECT_TYPE_INITIALIZER ;
		  +0x03c ParseProcedure   : (null)
	*/

} OBJECT_DATA_LITTLE, * LPOBJECT_DATA_LITTLE;


typedef struct _OBJECT_DATA_INFO_
{
	/*0x000 */ struct _OBJECT_DATA_INFO_* pFlink; // �ϸ����
	/*0x004 */ struct _OBJECT_DATA_INFO_* pBlink; // �¸����

	/*0x008 */ ULONG ObjectIndex;
	/*0x00C */ WCHAR szObjecType[ObjecTypeLength]; // ����������,eg:"Process"

	/*0x010 */ ULONG TypeInfoOffset; // ��λ_OBJECT_TYPE_INITIALIZERָ��ʱ��ƫ��,�����ڲ���ϵͳ�汾

	/*0x014 */ OBJECT_DATA_LITTLE Open;
	/*0x02C */ OBJECT_DATA_LITTLE Parse;

} OBJECT_DATA_INFO, * LPOBJECT_DATA_INFO;

//
// OD������ܽڵ�
//

typedef struct _OBJECT_DATA_INFO_HEAND_
{
	/*0x000 */ int			nTotalCounts;
	/*0x004 */ PERESOURCE	QueueLockList; // �������Դ��
	/*0x008 */ OBJECT_DATA_INFO ListHead;

} OBJECT_DATA_INFO_HEAND, * LPOBJECT_DATA_INFO_HEAND;

extern LPOBJECT_DATA_INFO_HEAND g_ListHead__ObjectData;
extern BOOL g_ObjectData_Inited_ok;

BOOL
InitObjectData(
);

BOOL
ODCreateTotalHead(
	OUT PVOID* _TotalHead
);