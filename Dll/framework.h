#pragma once

//#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
//#include <windows.h>
// Windows 头文件
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>

// ... 你的其他代码 ...
