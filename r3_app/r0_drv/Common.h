#pragma once
BOOL InitResource(IN PERESOURCE* rc);

BOOL
OpenEvent(
	IN LPWSTR lpEventName,
	IN ACCESS_MASK DesiredAccess,
	OUT PHANDLE hEvent
);