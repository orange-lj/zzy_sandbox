#pragma once
#include<winioctl.h>
#include<windows.h>

#include <ntdef.h>


/*
RPC相关
*/
typedef enum _SBIESRV_APINUM_ 
{
	_PBSRV_APINUM_INIGetCurUserSection_ = 0x1801,
}SBIESRV_APINUM;

typedef struct _RPC_IN_HEADER_ 
{
	DWORD DataLength;
	DWORD  Flag;
}RPC_IN_HEADER,*PRPC_IN_HEADER;


typedef struct __RPC_OUT_HEADER_ 
{
	ULONG ReturnLength;
	union 
	{
		NTSTATUS Status;
		DWORD ErrorCode;
	}u;
}RPC_OUT_HEADER,*LPRPC_OUT_HEADER;

typedef   struct   _CLIENT_ID
{
	HANDLE  UniqueProcess;         //32 vs 64
	HANDLE  UniqueThread;         //32 vs 64
}  CLIENT_ID, * PCLIENT_ID;


//
// 为port消息定义头
// 注意：32位和64位系统，消息头大小不同，一个为24，一个为40
//
typedef   struct   _PORT_MESSAGE
{
	USHORT  DataLength;                 // Length of data following header (bytes)
	USHORT  TotalLength;                 // Length of data + sizeof(PORT_MESSAGE)
	USHORT  Type;                     // Type of the message (LPC_TYPE)
	USHORT  VirtualRangesOffset;         // Offset of array of virtual address ranges
	CLIENT_ID  ClientId;                 // Client identifier of the message sender
	ULONG  MessageId;                 // Identifier of the particular message instance
	union {
		SIZE_T  ClientViewSize;       // Only valid on LPC_CONNECTION_REQUEST message
		ULONG  CallbackId;            // Only valid on LPC_REQUEST message
	};
}  PORT_MESSAGE, * PPORT_MESSAGE;


#define PB_PORT_MESSAGE_MAXLength	0x200
typedef struct _DLL_PORT_MESSAGE_ 
{
	PORT_MESSAGE Header;
	union 
	{
		UCHAR Buffer[PB_PORT_MESSAGE_MAXLength];
	};
}DLL_PORT_MESSAGE,*LPDLL_PORT_MESSAGE;
