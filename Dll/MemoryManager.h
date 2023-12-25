#pragma once

typedef struct _PROCESS_GLOBALMEMORY_NODE_ 
{
	ULONG Reserved[0x30];
	ULONG kmalloc_for_frequently_used_memory_BaseAddrOffset;
	struct 
	{
		BYTE nLockNtCreateKey;
		BYTE nLockNtC
	};
};

class MemoryManager
{
};

