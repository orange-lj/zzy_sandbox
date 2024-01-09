#include<ntifs.h>
#include<windef.h>
#include"struct.h"
#include"main.h"
#include"DoWork.h"
NTSTATUS
DispatchCreate(
	PDEVICE_OBJECT pDevObj,
	PIRP pIrp
);

VOID
DriverUnload(
	PDRIVER_OBJECT pDriverObj
);


NTSTATUS
DriverEntry(
	PDRIVER_OBJECT pDriverObj,
	PUNICODE_STRING pRegistryString
)
{
	BOOL bRet = FALSE;
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING ustrLinkName;
	UNICODE_STRING ustrDevName;
	PDEVICE_OBJECT pDevObj;

	DbgPrint(("[hy_Drv] DriverEntry\n"));

	pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	//pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
	//pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	pDriverObj->DriverUnload = DriverUnload;	
	RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
	status = IoCreateDevice(pDriverObj, 0, &ustrDevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);
	if (!NT_SUCCESS(status)) 
	{
		dprintf("[hy_Drv] IoCreateDevice = 0x%x\n", status);
		return status;
	}
	RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
	status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);
	if (!NT_SUCCESS(status)) {
		dprintf("[hy_Drv] IoCreateSymbolicLink = 0x%x\n", status);
		IoDeleteDevice(pDevObj);
		return status;
	}
	DoWork(TRUE);
	return STATUS_SUCCESS;
}

NTSTATUS
DispatchCreate(
	PDEVICE_OBJECT pDevObj,
	PIRP pIrp
)
{
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;

	dprintf("[hy_Drv] IRP_MJ_CREATE\n");

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

VOID
DriverUnload(
	PDRIVER_OBJECT pDriverObj
)
{
	UNICODE_STRING strLink;
	RtlInitUnicodeString(&strLink, LINK_NAME);
	IoDeleteSymbolicLink(&strLink);
	IoDeleteDevice(pDriverObj->DeviceObject);
	dprintf("[hy_Drv] Unloaded\n");
}
