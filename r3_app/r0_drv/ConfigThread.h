#pragma once

// �߳̽ṹ��Ϣ
typedef struct _THREAD_INFO
{
	BOOL	bIamRunning;
	PVOID	pThreadObj;		// ϵͳ�̶߳���

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