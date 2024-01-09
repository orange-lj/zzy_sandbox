#pragma once

#define LPMMHEAD PMEMORY_HEAD_INFO
#define MMHEAD MEMORY_HEAD_INFO
//
// �ڴ�ڵ�
//

typedef struct _MEMORY_INFO_
{
	/*0x000 */ LIST_ENTRY MemoryListEntry; // ָ���¸��ڵ�

	/*0x004 */ ULONG nSize; // �ڴ��С
	/*0x008 */ ULONG pAddress; // �ڴ��ַ

	/*0x00C */ ULONG Tag; // �ڴ����

} MEMORY_INFO, * PMEMORY_INFO;




//
// �ڴ������ܽڵ�
//

typedef struct _MEMORY_HEAD_INFO_
{
	/*0x000 */ PERESOURCE	QueueLockList; // �����������
	/*0x004 */ ULONG		nTotalCounts; // �ӽڵ�����
	/*0x004 */ MEMORY_INFO  ListHead;

} MEMORY_HEAD_INFO, * PMEMORY_HEAD_INFO;

extern PMEMORY_HEAD_INFO g_ListHead__MemoryManager;



BOOL
InitMemoryManager(
);


BOOL
MMCreateTotalHead(
	OUT PVOID* _TotalHead
);