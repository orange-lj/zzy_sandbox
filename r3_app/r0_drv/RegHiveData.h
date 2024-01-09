#pragma once
#define LPRHHEAD LPREGHIVE_NODE_INFO_HEAND
#define RHHEAD REGHIVE_NODE_INFO_HEAND
//
// size - 0x01C
//

typedef struct _REGHIVE_NODE_INFO_ {

	/*0x000 */ struct _REGHIVE_NODE_INFO_* pFlink;	// �ϸ��ڵ�
	/*0x004 */ struct _REGHIVE_NODE_INFO_* pBlink;	// �¸��ڵ�

	/*0x008 */ WCHAR HiveFilePath[MAX_PATH];  // eg:"\Device\HarddiskVolume1\Sandbox\AV\DefaultBox\RegHive"
	/*0x00C */ WCHAR HiveRegPath[MAX_PATH];   // eg:"\REGISTRY\USER\Sandbox_AV_DefaultBox"
	/*0x010 */ ULONG PorcessRefCounts; // ��ǰɳ���н��̼���,��Ϊ����һ������,�ͻ����һ��Ioctl_StartProcess.���л������ǰɳ���Hive�ļ�.������������ü���
	/*0x014 */ BOOL bNeedToDistroy; // ��TRUE,������ǰRegHive���޽���ռ��,����ж�ص�!
	/*0x018 */ BOOL ProcessesLock; // ���ڶ�����̼��ͬ������!��һ��ɳ���п��ܻ���2+������ͬʱ������ʱ����Hive,���Ա���ͬ��,����Ϊ1��ʾ��ռ��,��������״̬!

} REGHIVE_NODE_INFO, * LPREGHIVE_NODE_INFO;

//
// RH ������ܽڵ�
//

typedef struct _REGHIVE_NODE_INFO_HEAND_
{
	/*0x000 */ int			nTotalCounts;
	/*0x004 */ PERESOURCE	QueueLockList; // �������Դ��
	/*0x008 */ REGHIVE_NODE_INFO ListHead;

} REGHIVE_NODE_INFO_HEAND, * LPREGHIVE_NODE_INFO_HEAND;

extern LPREGHIVE_NODE_INFO_HEAND g_ListHead__RegHive;
extern BOOL g_RegHive_Inited_ok;


BOOL
InitRegHive(
);

BOOL
RHCreateTotalHead(
	OUT PVOID* _TotalHead
);