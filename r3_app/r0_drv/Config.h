#pragma once

#include "ConfigData.h"

typedef enum _IOCTL_HANDLERCONF_FUNCTION_TYPE_
{
	_Ioctl_Conf_function_InitData_ = 0x1001, // R3���͸�Ioctl,�������ļ��е�������Ϣȫ������R0�洢 
	_Ioctl_Conf_function_VerifyData_ = 0x1002, // R3���͸�Ioctl,��R0���Buffer,���а����˶�/дָ��������Ϣ (����)
	_Ioctl_Conf_function_ReceiveData_ = 0x1003, // R3���͸�Ioctl,����ȡ���������ļ���Ϣ���ݵ�R0��	(����) 
	_Ioctl_Conf_function_Reload_ = 0x1004, // ���¼��������ļ�,��R3���¶�ȡ�����ļ���,�����и��º�������׸�����	

} IOCTL_HANDLERCONF_FUNCTION_TYPE;


typedef enum _IOCTL_HANDLERCONF_BUFFER_TYPE_
{
	_Ioctl_Conf_Read_ = 1,
	_Ioctl_Conf_Write_ = 2,

} IOCTL_HANDLERCONF_BUFFER_TYPE;


typedef struct _IOCTL_HANDLERCONF_BUFFER_IniDataA_ // size - 0x018
{
	/*0x000 */ LPSTR SeactionName;
	/*0x004 */ ULONG SeactionNameLength;

	/*0x008 */ LPSTR KeyName;
	/*0x00C */ ULONG KeyNameLength;

	/*0x010 */ LPSTR ValueName;
	/*0x014 */ ULONG ValueNameLength;

} IOCTL_HANDLERCONF_BUFFER_IniDataA, * LPIOCTL_HANDLERCONF_BUFFER_IniDataA;


typedef struct _IOCTL_HANDLERCONF_BUFFER_IniDataW_ // size - 0x018
{
	/*0x000 */ LPWSTR SeactionName;
	/*0x004 */ ULONG SeactionNameLength;

	/*0x008 */ LPWSTR KeyName;
	/*0x00C */ ULONG KeyNameLength;

	/*0x010 */ LPWSTR ValueName;
	/*0x014 */ ULONG ValueNameLength;

} IOCTL_HANDLERCONF_BUFFER_IniDataW, * LPIOCTL_HANDLERCONF_BUFFER_IniDataW;


#define IOCTL_HANDLERCONF_BUFFER_NAMELENGTH 0x50

typedef struct _IOCTL_HANDLERCONF_BUFFER_VerifyData_ // size - 0x24
{
	/*0x000 */ char SeactionName[IOCTL_HANDLERCONF_BUFFER_NAMELENGTH];	// eg: "GlobalSetting"
	/*0x004 */ char KeyName[IOCTL_HANDLERCONF_BUFFER_NAMELENGTH];		// eg: "OpenIpcPath"
	/*0x008 */ IOCTL_HANDLERCONF_BUFFER_TYPE Flag;		// ��/д���
	/*0x00C */ PVOID Data;			// ����R3��ѯ��������

} IOCTL_HANDLERCONF_BUFFER_VerifyData, * LPIOCTL_HANDLERCONF_BUFFER_VerifyData;


typedef struct _IOCTL_HANDLERCONF_BUFFER_ReceiveData_ // size - 0x08
{
	/*0x000 */ ULONG NameLength;
	/*0x004 */ LPSTR szName; // ansi��ʽ

} IOCTL_HANDLERCONF_BUFFER_ReceiveData, * LPIOCTL_HANDLERCONF_BUFFER_ReceiveData;


typedef struct _IOCTL_HANDLERCONF_BUFFER_RELAOD_ // size - 0x04
{
	/*0x000 */ ULONG Reserved;

} IOCTL_HANDLERCONF_BUFFER_RELAOD, * LPIOCTL_HANDLERCONF_BUFFER_RELAOD;


//
// ����Configuration���ܽṹ��
//

typedef struct _IOCTL_HANDLERCONF_BUFFER_ // size - 0x24
{
	union
	{
		IOCTL_HANDLERCONF_BUFFER_IniDataA		InitData;
		IOCTL_HANDLERCONF_BUFFER_VerifyData		VerifyData;
		IOCTL_HANDLERCONF_BUFFER_ReceiveData	ReceiveData;
		IOCTL_HANDLERCONF_BUFFER_RELAOD			ReloadData;
	} u;

} IOCTL_HANDLERCONF_BUFFER, * LPIOCTL_HANDLERCONF_BUFFER;



typedef struct _IOCTL_HANDLERCONF_GLOBAL_
{
	/*0x000 */ IOCTL_HANDLERCONF_BUFFER_VerifyData VerifyDataCurrent;
	/*0x004 */ PB_CONFIG_COMMON ValueListHead;

} IOCTL_HANDLERCONF_GLOBAL, * LPIOCTL_HANDLERCONF_GLOBAL;

extern LPIOCTL_HANDLERCONF_GLOBAL g_Ioctl_HandlerConf_GlobalData;

BOOL
InitConfig(
);

