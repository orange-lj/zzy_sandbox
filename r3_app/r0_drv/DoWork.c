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

	// 1. 初始化系统信息
	bRet = GetVersion();
	if (FALSE == bRet)
	{
		dprintf("[hy_Drv] error! | DoWork() - GetVersion(); \n");
		return;
	}
	//GetProcessNameOffset();
	// 2.1 初始化内存管理器
	if (FALSE == InitMemoryManager())
	{
		dprintf("error! | DoWork() - InitMemoryManager(); \n");
		return;
	}
	// 2.2 初始化进程节点管理器
	if (FALSE == InitProcessData())
	{
		dprintf("error! | DoWork() - InitProcessData(); \n");
		return;
	}
	// 2.3 初始化SID节点管理器
	if (FALSE == InitSecurityData())
	{
		dprintf("error! | DoWork() - InitSecurityData(); \n");
		return;
	}
	// 2.4 初始化RegHive节点管理器
	if (FALSE == InitRegHive())
	{
		dprintf("error! | DoWork() - InitRegHive(); \n");
		return;
	}
	// 2.5 初始化ObjectHook节点管理器
	if (FALSE == InitObjectData())
	{
		dprintf("error! | DoWork() - InitObjectData(); \n");
		return;
	}
	// 2.6 初始化配置文件相关信息------------
	if (FALSE == InitConfig())
	{
		dprintf("error! | DoWork() - InitConfig(); \n");
		return;
	}
	// 2.7 初始化Inline Hook引擎
	if (FALSE == LoadInlineHookEngine())
	{
		dprintf("error! | DoWork() - LoadInlineHookEngine(); \n");
		return;
	}
}
