#include "pch.h"
#include "Exportfunc.h"
#include"../Base/Ioctl.h"
#include"Common.h"
/*++
Routine Description:
  PBStart.exe ��PBSrv.exeͨ�ŵ���Ҫ����,���ð�(+�n+)~

Arguments:
  pBuffer - ���־���Ľṹ��ָ��

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
  �ͷ�RPC���ص��ڴ�
--*/
DLL_API VOID DLL_FreeReply(IN PVOID pBuffer)
{
	return DLL_API VOID();
}


/*++
Routine Description:
  ��ѯ�����ļ�,��Ҫ������ͨ��

Arguments:
  section - eg:"[GlobalSetting]"
  key - eg:"ForceProcess"
  MaxLength - @pBuffer����������󳤶�
  pBuffer - �����ѯ������Ϣ

Return Value:
  TRUE / FALSE

--*/
DLL_API BOOL DLL_QueryConf(IN LPWSTR section, IN LPWSTR key, IN ULONG MaxLength, OUT PVOID pBuffer)
{
	return DLL_API BOOL();
}
