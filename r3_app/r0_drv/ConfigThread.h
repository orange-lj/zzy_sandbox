#pragma once

// 线程结构信息
typedef struct _THREAD_INFO
{
	BOOL	bIamRunning;
	PVOID	pThreadObj;		// 系统线程对象

	HANDLE hEvent;

} THREAD_INFO, * PTHREAD_INFO;

VOID
CreateConfigThread(
	IN BOOL bTerminate
);



VOID
ConfigThread(
	IN PVOID StartContext
);