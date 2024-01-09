#include "struct.h"
#include "ConfigThread.h"
#include "Common.h"
#include "Config.h"
#include "ProcessData.h"

#define g_ConfEvent_InitConfigData_wakeup_R0	 L"\\BaseNamedObjects\\Global\\Proteinbox_ConfEvent_InitConfigData_wakeup_R0"
#define g_ConfEvent_InitConfigData_wakeup_R3	 L"\\BaseNamedObjects\\Global\\Proteinbox_ConfEvent_InitConfigData_wakeup_R3"
THREAD_INFO g_tmpThread_info;

VOID CreateConfigThread(IN BOOL bTerminate)
{
	HANDLE ThreadHandle;
	BOOL bRet = FALSE;
	BOOL bOK = FALSE;

	if (FALSE == bTerminate) { // ��������

		if (!NT_SUCCESS(PsCreateSystemThread(&ThreadHandle,
			THREAD_ALL_ACCESS,
			NULL,
			0L,
			NULL,
			ConfigThread,
			NULL))
			)
		{
			dprintf("CreateConfigThread() -->PsCreateSystemThread. Failed\n");
			return;
		}

		ObReferenceObjectByHandle(ThreadHandle,
			THREAD_ALL_ACCESS,
			NULL,
			KernelMode,
			(PVOID*)&g_tmpThread_info.pThreadObj,
			NULL);

		//
		// ��ʼ���ý��̵Ľṹ��
		//

		g_tmpThread_info.bIamRunning = TRUE;
		ZwClose(ThreadHandle);
	}
	else // ���ٽ���
	{
		if (FALSE == g_tmpThread_info.bIamRunning) { return; }

		// ���߳�û�н���,��ȴ������,����ͷ��ڴ�
		g_tmpThread_info.bIamRunning = FALSE;

		ZwSetEvent(g_tmpThread_info.hEvent, 0);  // ����Ϊ"����"״̬,���ѵȴ����߳�

		if (g_tmpThread_info.pThreadObj != NULL) { // �ȴ��̵߳Ľ���
			KeWaitForSingleObject(g_tmpThread_info.pThreadObj, Executive, KernelMode, FALSE, NULL);
		}

		if (g_tmpThread_info.pThreadObj != NULL) { // �� thread object����,�ͷŵ�,Ҫ����BSOD
			ObDereferenceObject(g_tmpThread_info.pThreadObj);
			g_tmpThread_info.pThreadObj = NULL;
		}
	}

	return;
}

VOID ConfigThread(IN PVOID StartContext)
{
	BOOL bRet = FALSE;

	// �򿪵ȴ��¼�
	bRet = OpenEvent(g_ConfEvent_InitConfigData_wakeup_R0, EVENT_ALL_ACCESS, &g_tmpThread_info.hEvent);
	if (FALSE == bRet)
	{
		dprintf("error! | ConfigThread() - OpenEvent(); | \"%s\" \n", g_ConfEvent_InitConfigData_wakeup_R0);
		//goto _OVER_;
	}
	// �ȴ��¼���һֱ��"����"״̬
	dprintf("[hy_Drv] I'm Waiting for Config Data ... \n");
	ZwWaitForSingleObject(g_tmpThread_info.hEvent, FALSE, NULL);
	if (FALSE == g_tmpThread_info.bIamRunning) 
	{ 
		goto _CLEAR_;
	}
	// ������δ��ɳ�ʼ������,��ǰ״̬Ϊ����Ӧ�κβ���
	if (FALSE == g_Driver_Inited_phrase1)
	{
		dprintf("error! | ConfigThread(); | ���յ�R3������������,������δ��ɳ�ʼ������,��ǰ״̬Ϊ����Ӧ�κβ���. \n");
		goto _CLEAR_;
	}

_CLEAR_:
	ZwClose(g_tmpThread_info.hEvent);
}
