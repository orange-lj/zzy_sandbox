#pragma once

#define LPMMHEAD PMEMORY_HEAD_INFO
#define MMHEAD MEMORY_HEAD_INFO
//
// 内存节点
//

typedef struct _MEMORY_INFO_
{
	/*0x000 */ LIST_ENTRY MemoryListEntry; // 指向下个节点

	/*0x004 */ ULONG nSize; // 内存大小
	/*0x008 */ ULONG pAddress; // 内存地址

	/*0x00C */ ULONG Tag; // 内存块标记

} MEMORY_INFO, * PMEMORY_INFO;




//
// 内存管理的总节点
//

typedef struct _MEMORY_HEAD_INFO_
{
	/*0x000 */ PERESOURCE	QueueLockList; // 链表的自旋锁
	/*0x004 */ ULONG		nTotalCounts; // 子节点总数
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