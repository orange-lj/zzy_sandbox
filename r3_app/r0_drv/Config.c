#include "struct.h"
#include "Common.h"
//#include "DispatchIoctl.h"
#include "Memory.h"
#include "ConfigThread.h"
#include "config.h"



BOOL InitConfig()
{
	BOOL bRet = FALSE;

	// 1. ��ʼ��GrayList����
	bRet = InitConfigData();
	if (FALSE == bRet)
	{
		dprintf("error! | InitConfig() - InitConfigData(); | \n");
		return FALSE;
	}

	// 3. ����һ���߳������������ļ���Ϣ
	CreateConfigThread(FALSE);
}


