#pragma once
#include "Common.h"
#define LPPDHEAD LPPROCESS_NODE_INFO_HEAND

#define PDHEAD PROCESS_NODE_INFO_HEAND
//
// size - 0x078
//

typedef struct _NODE_INFO_C_ {

	/*0x000 */	WCHAR BoxName[MAX_PATH];	// eg:"DefaultBox"
	/*0x044 */  ULONG BoxNameLength;

	/*0x048 */  WCHAR RegisterUserID[MAX_PATH]; // ����ZwQueryInformationProcess()���ݲ���ProcessSessionInformation�õ���SID
										  //eg:"S-1-5-21-583907252-261903793-1177238915-1003"
	/*0x04C */  ULONG RegisterUserIDLength;

	/*0x050 */  ULONG SessionId;

	/*0x058 */  WCHAR FileRootPath[MAX_PATH];  // eg:"\Device\HarddiskVolume1\Sandbox\AV\DefaultBox"
	/*0x05C */  ULONG FileRootPathLength;

	/*0x060 */	WCHAR KeyRootPath[MAX_PATH];	// eg:"\REGISTRY\USER\Sandbox_AV_DefaultBox"
	/*0x064 */	ULONG KeyRootPathLength;

	/*0x068 */  WCHAR LpcRootPath1[MAX_PATH];			// eg:"\Sandbox\AV\DefaultBox\Session_0"
	/*0x06C */  ULONG LpcRootPath1Length;

	/*0x070 */  WCHAR LpcRootPath2[MAX_PATH];			// eg:"_Sandbox_AV_DefaultBox_Session_0"
	/*0x074 */  ULONG LpcRootPath2Length;

} NODE_INFO_C, * PNODE_INFO_C;

#pragma pack(1) // ��ʾ����1byte����
typedef struct _PROCESS_NODE_INFO_ // size - 0x180
{
	struct _PROCESS_NODE_INFO_* pFlink;	// �ϸ��ڵ�
	struct _PROCESS_NODE_INFO_* pBlink;	// �¸��ڵ�
	ULONG PID;			// ��ǰ����ID
	PNODE_INFO_C pNode_C;		// ָ���ṹ�� xxxxxxxxxxxxxxxxxxxxxxx
	LPWSTR lpProcessShortName;	// ���̶���; eg:"unknown executable image"		
	ULONG ImageNameLength; // ����������

	BYTE bIsAVDefaultBox; // ��1�������ض���Ŀ¼�µ��ļ�"������" eg:"\Device\HarddiskVolume1\Sandbox\AV\DefaultBox\xx.exe"
	BYTE bSelfExe;	// ��1�����Ǳ������Լ�Ŀ¼�µĳ���"������";eg:"\Device\HarddiskVolume1\Program Files\Proteinbox\ProteinboxRpcSs.exe"
	BYTE bDiscard;	// ��1������Ҫ���ٵĽ��	
	BYTE bReserved1;


	BYTE bReserved2;
	BYTE bDropAdminRights;
	BYTE bReserved3;
	BYTE bReserved4;

	BOOL bProcessNodeInitOK; // ��ǰ�ڵ��Ƿ��ʼ�����

	// ��ImageNotify Inject Dll ���
	struct
	{
		PVOID BeCoveredOldData; // ����ԭPE������,R3��DLL�����غ����øò������ݻָ��Ѹ�д��PE
		PVOID BeCoveredAddr;	 // �����ǵ�PE�ڴ�����ʼ��ַ
		ULONG BeCoveredSize;	 // �����ǵ�PE�ڴ��Ĵ�С

		ULONG IncrementCounts;  // ͬ�����̻����N�������¶�εĽ���ImageNofity�ص���;�ñ����������Ǳ�ֻ֤����һ�ε�ǰPE��

	} XImageNotifyDLL;

	struct
	{
		BYTE  bFlag_NotifyInternetAccessDenied; // ��ֹ������������Ժ��Ƿ�֪ͨ�û�
		/* �����ļ��е�<InternetAccess>�ṩһ��������ʵİ�������ֻ�����������ĳ���������磬���������ֹ�������磬����Ӧ�ó������ǽ����
		��Internet Access ����Firefox,����ȫ������ [GlobalSettings] ����һ�������飬����<InternetAccess_Firefox>,�����ֻ��һ�����Ա�� */

		BOOL bFlagInited; // pNodeHead1,2,3��ָ���Node���,��ʼ�����,��1
		PERESOURCE pResource;				// ����3���ڵ����Դ��
		LIST_ENTRY OpenFilePathListHead;	// ��������ֱ�Ӳ������ַ���,����:"\\Device\\NamedPipe\\protected_storage", g_Array_Device[] 
		LIST_ENTRY ClosedFilePathListHead;	// �����ֹ���ʵ��ַ���,����:"\\Device\\LanmanRedirector","\\Device\\Mup" ,�����ֹ����"D:\Work"Ŀ¼
		LIST_ENTRY ReadFilePathListHead;	// ���������ļ��� @ReadFilePath ��Ӧ������
	} XFilePath;

	struct
	{
		BYTE bFlag_NotifyStartRunAccessDenied;
		BYTE bFlag_Denny_Access_KnownDllsSession;  // eg: 'Y'����"N",�Ƿ��ֹ����ȫ��Session����" "\\KnownDlls\\*"

		BOOL bFlagInited; // Node���,��ʼ�����,��1
		PERESOURCE pResource;				// ����2���ڵ����Դ��
		LIST_ENTRY OpenIpcPathListHead;	/* ��������ֱ�Ӳ������ַ���,����:"*\\BaseNamedObjects*\\PS_SERVICE_STARTED", "\\RPC Control\\OLE*",
											"\\RPC Control\\epmapper", "\\RPC Control\\protected_storage", g_Array_BaseNamedObjects[], g_Array_RPC[]
											*/
		LIST_ENTRY ClosedIpcPathListHead;	// �����ֹ���ʵ��ַ���,�������ļ��� @ClosedIpcPath ��Ӧ������
	} XIpcPath;

	struct
	{
		BYTE bFlag_BlockFakeInput; // �Ƿ��ֹ���ⰴ��; TRUEΪ��ֹ
		BYTE bFlag_BlockWinHooks;  // �Ƿ��ֹNtUserSetWindowsHookEx,NtUserSetWinEventHook�ĵ���
		BYTE bFlag_BlockSysParam;	// �Ƿ��ֹNtUserSystemParametersInfo,NtUserSetSysColors�ĵ���
		BYTE bFlag_SendPostMessage_allowed; // �Ƿ�����NtUser**������Ϣ. TRUEΪ����

		HWND CurrentHwnd; // ����NtUserGetForegroundWindow�õ��ܺ��ߵĴ��ھ��,��ʱ�洢���˴�
		// ��ɳ���еĳ����״β����ô���,ɳ���R3�ᵯ����ֹ��Ϣ,����N��(N>=2)������ͬ�Ĵ���,���ٵ�����Ϣ��ʾ��
		// ���ֿ�ʼ������������,�����µ�����ʾ��,���������ʱ�洢��ǰ��HWND,������R3�����Ƿ񵯿���ʾ�û�

		PVOID TranshipmentStation; //  ����ZwAllocateVirtualMemory()������ڴ��ַ; ��СΪ0x1000
		// ͨ��NtUserGetClassName()�����õ���ǰhwnd��Ӧ��ClassName. ��ָ�����Ϊ��ǰ���̵���ʱ�ڴ���,
		// ÿ�ζ��Ὣ��ȡ�������ݿ�������һ���С���ڴ�,�����Լ�����յ�.  ���ڴ������ͷ�,��������ʱ��Ȼ��û��

		PVOID VictimClassName; // �ܺ��ߵĴ����ַ���ָ��,��̬������ڴ�,��������ʱ��Ҫ�ͷŵ�

		BOOL bFlagInited; // WndListHead���,��ʼ�����,��1
		PERESOURCE pResource;	// ���½ڵ����Դ��
		LIST_ENTRY WndListHead; // ������ʵĴ�������	
	} XWnd;

	struct
	{
		BOOL bFlagInited; // Node���,��ʼ�����,��1
		PERESOURCE pResource;				// ����2���ڵ����Դ��
		LIST_ENTRY DirectListHead;	// ��������ֱ�Ӳ������ַ���( @OpenKeyPath )
		LIST_ENTRY DennyListHead;	// �����ֹ���ʵ��ַ���,�������ļ��� @ClosedKeyPath ��Ӧ������
		LIST_ENTRY ReadOnlyListHead; // ֻ������ Read-Only Access ( @ReadKeyPath )

	} XRegKey;

	/*0x0E0 */ ULONG FileTraceFlag;
	/*0x0E4 */ ULONG PipeTraceFlag;

	/*0x0E8 */
	/*0x0EC */ PVOID pResource2;
	/*0x0F0 */ PVOID pNode_RegHive;

	/*0x0F4 */ BYTE bFlag_BuildNode_RegeditKey_OK;

	/*0x0F8 */ PVOID pNodeHead_WhiteList_OpenKeyPath;
	/*0x104 */ PVOID pNodeHead_WhiteList_ClosedKeyPath;
	/*0x110 */ PVOID pNodeHead_WhiteList_ReadKeyPath;

	/*0x11C */ ULONG KeyTraceFlag;

	/*0x124 */ BYTE bFlag_BuildNode_RPC_BaseNamedObjects_OK;


	/*0x140 */ ULONG Flag_OpenProcedure_ThreadProcess;// & 3��ֵ,����Ҫ����DbgPrint��ӡ��Ϣ; &2��ֵ,�������������ǰ����/�߳�; &1��ֵ,������ֹ������ǰ����/�߳� 
	/*
	if ( Flag_OpenProcedure_ThreadProcess & 3 )
	  {
		if ( status >= 0 )
		{
		  if ( Flag_OpenProcedure_ThreadProcess & 1 )
		  {
			szInfo2 = 'A'; // Allowed
			goto _printf_;
		  }
		}
		else
		{
		  if ( Flag_OpenProcedure_ThreadProcess & 2 )
		  {
			szInfo2 = 'D'; // Denney
	_printf_:
			swprintf(&szInfo1, L"(P%c) %08X %06d", szInfo2, GrantedAccess, PID);
			DbgPrintEx((int)&szInfo1, (int)off_281A8);
			return status;
		  }
		}
	  }

	  ����: (003472) SBIE (PA) 00000400 001672
	*/


	/*0x164 */ HWND CurrentHwnd; // ����NtUserGetForegroundWindow�õ��ܺ��ߵĴ��ھ��,��ʱ�洢���˴�
	// ��ɳ���еĳ����״β����ô���,ɳ���R3�ᵯ����ֹ��Ϣ,����N��(N>=2)������ͬ�Ĵ���,���ٵ�����Ϣ��ʾ��
	// ���ֿ�ʼ������������,�����µ�����ʾ��,���������ʱ�洢��ǰ��HWND,������R3�����Ƿ񵯿���ʾ�û�

	/*0x168 */ PVOID pstrClassName; // ����ZwAllocateVirtualMemory()������ڴ��ַ; ��СΪ0x1000
									 // ͨ��NtUserGetClassName()�����õ���ǰhwnd��Ӧ��ClassName.	
	/*0x16C */ PVOID pImageName2;
	/*0x170 */ PVOID pNodeHead_WhiteList_Wnd; // ָ��ṹ�� Wnd_Head_Info, �������ClassName���ǰ�����,������ɳ���еĳ����������Щ��������細�巢����Ϣ
	/*0x174 */
	/*0x178 */
	/*0x17C */ ULONG GuiTraceFlag; // ���ݱ�־λ,����ָ���Ĵ����,��ɳ���R3������ʾ��ֹ��Ϣ

} PROCESS_NODE_INFO, * LPPROCESS_NODE_INFO;
#pragma pack()

typedef struct _PROCESS_NODE_INFO_HEAND_
{
	/*0x000 */ int			nTotalCounts;
	/*0x004 */ PERESOURCE	QueueLockList; // �����������
	/*0x008 */ PROCESS_NODE_INFO ListHead;

} PROCESS_NODE_INFO_HEAND, * LPPROCESS_NODE_INFO_HEAND;

BOOL
InitProcessData(
);

BOOL
PDCreateTotalHead(
	OUT PVOID* _TotalHead
);