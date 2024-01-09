#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <wtypes.h>
#include <windows.h>
#include <Winsvc.h>	// 为了使用SCM函数

#ifdef __cplusplus
extern "C" {
#endif

class CDriver 
{
public:
	CDriver(LPCSTR pszDriverPath, LPCSTR pszLinkName);
// 操作
	// 开启服务。也就是说驱动的DriverEntry函数将被调用
	virtual BOOL StartDriver();
	// 打开设备，即取得到此驱动的一个句柄
	virtual BOOL OpenDevice();
protected:
	char m_szLinkName[56];	// 符号连接名称
	char m_szSysName[64];

	BOOL m_bStarted;	// 指定服务是否启动
	BOOL m_bCreateService;	// 指定是否创建了服务

	HANDLE m_hSCM;		// SCM数据库句柄
	HANDLE m_hService;	// 服务句柄
	HANDLE m_hDriver;	// 设备句柄
};


#ifdef __cplusplus
}
#endif