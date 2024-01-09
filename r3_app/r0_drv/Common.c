#include "struct.h"
#include "Common.h"

BOOL InitResource(IN PERESOURCE* rc)
{
	// 初始化资源锁
	*rc = (PERESOURCE)kmalloc(sizeof(ERESOURCE));
	if (NULL == *rc)
	{
		dprintf("error! | InitResource(); 申请资源锁内存失败! \n");
		return FALSE;
	}

	ExInitializeResource(*rc);
	return TRUE;
}

BOOL OpenEvent(IN LPWSTR lpEventName, IN ACCESS_MASK DesiredAccess, OUT PHANDLE hEvent)
{
	UNICODE_STRING EventName;
	OBJECT_ATTRIBUTES ObjectAttributes;
	NTSTATUS status = STATUS_UNSUCCESSFUL;

	if ((NULL == lpEventName) || (NULL == hEvent))
	{
		dprintf("error! | OpenEvent(); | Invalid Paramaters; failed! \n");
		return FALSE;
	}

	RtlInitUnicodeString(&EventName, lpEventName);
	InitializeObjectAttributes(&ObjectAttributes, &EventName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, 0);

	status = ZwOpenEvent(
		hEvent,
		EVENT_MODIFY_STATE,
		&ObjectAttributes
	);

	if (!NT_SUCCESS(status))
	{
		dprintf("error! | OpenEvent(); | (status=0x%08lx) \n", status);
		return FALSE;
	}

	return TRUE;
}
