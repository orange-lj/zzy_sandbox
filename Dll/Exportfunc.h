#pragma once

#include "framework.h"
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif

extern DLL_API int nDll;
DLL_API PVOID DLL_CallServer(IN PVOID pBuffer);
DLL_API VOID DLL_FreeReply(IN PVOID pBuffer);
DLL_API BOOL DLL_QueryConf(IN LPWSTR section, IN LPWSTR key, IN ULONG MaxLength, OUT PVOID pBuffer);


#ifdef __cplusplus
}
#endif