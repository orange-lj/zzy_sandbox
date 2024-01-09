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

	if (FALSE == bTerminate) { // 创建进程

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
		// 初始化该进程的结构体
		//

		g_tmpThread_info.bIamRunning = TRUE;
		ZwClose(ThreadHandle);
	}
	else // 销毁进程
	{
		if (FALSE == g_tmpThread_info.bIamRunning) { return; }

		// 若线程没有结束,需等待其结束,随后释放内存
		g_tmpThread_info.bIamRunning = FALSE;

		ZwSetEvent(g_tmpThread_info.hEvent, 0);  // 设置为"受信"状态,唤醒等待的线程

		if (g_tmpThread_info.pThreadObj != NULL) { // 等待线程的结束
			KeWaitForSingleObject(g_tmpThread_info.pThreadObj, Executive, KernelMode, FALSE, NULL);
		}

		if (g_tmpThread_info.pThreadObj != NULL) { // 若 thread object存在,释放掉,要不会BSOD
			ObDereferenceObject(g_tmpThread_info.pThreadObj);
			g_tmpThread_info.pThreadObj = NULL;
		}
	}

	return;
}

VOID ConfigThread(IN PVOID StartContext)
{
	BOOL bRet = FALSE;

	// 打开等待事件
	bRet = OpenEvent(g_ConfEvent_InitConfigData_wakeup_R0, EVENT_ALL_ACCESS, &g_tmpThread_info.hEvent);
	if (FALSE == bRet)
	{
		dprintf("error! | ConfigThread() - OpenEvent(); | \"%s\" \n", g_ConfEvent_InitConfigData_wakeup_R0);
		//goto _OVER_;
	}
	// 等待事件，一直到"受信"状态
	dprintf("[hy_Drv] I'm Waiting for Config Data ... \n");
	ZwWaitForSingleObject(g_tmpThread_info.hEvent, FALSE, NULL);
	if (FALSE == g_tmpThread_info.bIamRunning) 
	{ 
		goto _CLEAR_;
	}
	// 若驱动未完成初始化操作,当前状态为不响应任何操作
	if (FALSE == g_Driver_Inited_phrase1)
	{
		dprintf("error! | ConfigThread(); | 已收到R3传来激活命令,但驱动未完成初始化操作,当前状态为不响应任何操作. \n");
		goto _CLEAR_;
	}

_CLEAR_:
	ZwClose(g_tmpThread_info.hEvent);
}
