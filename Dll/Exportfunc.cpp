#include "pch.h"
#include "Exportfunc.h"
#include"../Base/Ioctl.h"
#include"Common.h"
/*++
Routine Description:
  PBStart.exe 跟PBSrv.exe通信的主要函数,你妹啊(+﹏+)~

Arguments:
  pBuffer - 各种纠结的结构体指针

Return Value:
--*/
DLL_API PVOID DLL_CallServer(IN PVOID pBuffer)
{
	BOOL bFlag = FALSE;
	PVOID MsgBody = NULL;
	NTSTATUS status = STATUS_SUCCESS;
	DLL_PORT_MESSAGE RequestMessage, ReplyMessage;
	LPDLL_PORT_MESSAGE pRequestMessage = NULL;
	PRPC_IN_HEADER pRpcInBuffer = (PRPC_IN_HEADER)pBuffer;
	LPRPC_OUT_HEADER pOutBuffer = (LPRPC_OUT_HEADER)kmalloc(sizeof(RPC_OUT_HEADER));

	return DLL_API PVOID();
}



/*++
Routine Description:
  释放RPC返回的内存
--*/
DLL_API VOID DLL_FreeReply(IN PVOID pBuffer)
{
	return DLL_API VOID();
}


/*++
Routine Description:
  查询配置文件,需要和驱动通信

Arguments:
  section - eg:"[GlobalSetting]"
  key - eg:"ForceProcess"
  MaxLength - @pBuffer缓冲区的最大长度
  pBuffer - 保存查询到的信息

Return Value:
  TRUE / FALSE

--*/
DLL_API BOOL DLL_QueryConf(IN LPWSTR section, IN LPWSTR key, IN ULONG MaxLength, OUT PVOID pBuffer)
{
	return DLL_API BOOL();
}
