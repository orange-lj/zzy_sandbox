#include "struct.h"
#include "Common.h"
//#include "DispatchIoctl.h"
#include "Memory.h"
#include "ConfigThread.h"
#include "config.h"



BOOL InitConfig()
{
	BOOL bRet = FALSE;

	// 1. 初始化GrayList链表
	bRet = InitConfigData();
	if (FALSE == bRet)
	{
		dprintf("error! | InitConfig() - InitConfigData(); | \n");
		return FALSE;
	}

	// 3. 创建一个线程来处理配置文件信息
	CreateConfigThread(FALSE);
}


