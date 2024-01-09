#pragma once
#include <ntifs.h>
#include <windef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define dprintf if (DBG) DbgPrint
#define kmalloc(_s)	ExAllocatePoolWithTag(NonPagedPool, _s, 'ADUS')
#define kfree(_p)	if(_p){ ExFreePool(_p); _p = NULL ; }
extern BOOL g_Driver_Inited_phrase1;