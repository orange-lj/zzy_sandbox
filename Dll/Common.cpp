#include "Common.h"

PVOID kmalloc(ULONG length)
{
	PVOID buffer = NULL;
	buffer = new PVOID[length];
	if (NULL == buffer) 
	{
		ExitProcess(0xFFFFFFFF);
	}
	memset(buffer, 0, length * sizeof(PVOID));
	return (PVOID)buffer;
}
