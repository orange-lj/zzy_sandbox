#include"struct.h"
#include"Version.h"
#include"DoWork.h"
#include"Memory.h"
#include"ProcessData.h"
#include"SecurityData.h"
#include"RegHiveData.h"
#include"ObjectData.h"
#include"Config.h"

BOOL g_Driver_Inited_phrase1 = FALSE;

VOID DoWork(BOOL bWork)
{
	if (TRUE == bWork)
	{
		Starting();
	}
	else
	{
		//Stoping();
	}

	return;
}

VOID Starting()
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	BOOL	 bRet = FALSE;

	// 1. ��ʼ��ϵͳ��Ϣ
	bRet = GetVersion();
	if (FALSE == bRet)
	{
		dprintf("[hy_Drv] error! | DoWork() - GetVersion(); \n");
		return;
	}
	//GetProcessNameOffset();
	// 2.1 ��ʼ���ڴ������
	if (FALSE == InitMemoryManager())
	{
		dprintf("error! | DoWork() - InitMemoryManager(); \n");
		return;
	}
	// 2.2 ��ʼ�����̽ڵ������
	if (FALSE == InitProcessData())
	{
		dprintf("error! | DoWork() - InitProcessData(); \n");
		return;
	}
	// 2.3 ��ʼ��SID�ڵ������
	if (FALSE == InitSecurityData())
	{
		dprintf("error! | DoWork() - InitSecurityData(); \n");
		return;
	}
	// 2.4 ��ʼ��RegHive�ڵ������
	if (FALSE == InitRegHive())
	{
		dprintf("error! | DoWork() - InitRegHive(); \n");
		return;
	}
	// 2.5 ��ʼ��ObjectHook�ڵ������
	if (FALSE == InitObjectData())
	{
		dprintf("error! | DoWork() - InitObjectData(); \n");
		return;
	}
	// 2.6 ��ʼ�������ļ������Ϣ------------
	if (FALSE == InitConfig())
	{
		dprintf("error! | DoWork() - InitConfig(); \n");
		return;
	}
	// 2.7 ��ʼ��Inline Hook����
	if (FALSE == LoadInlineHookEngine())
	{
		dprintf("error! | DoWork() - LoadInlineHookEngine(); \n");
		return;
	}
}
