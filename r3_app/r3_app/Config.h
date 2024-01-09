#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wtypes.h>
#include <windows.h>
#include"CLoadDriver.h"

#define ARRSIZEOF(x) (sizeof(x)/sizeof(x[0]))
#define CONFIG_FILE_NAME_MAIN	"hy.ini"
#define CONFIG_FILE_NAME_TMPL	"Templates.ini"


#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#define kfree( TAG ) if ( TAG ) { free( (PVOID)TAG ); }
typedef enum _ConfStringType_
{
	T_SECTION,		// eg: [GlobalSetting]
	T_KEY,			// eg: OpenFilePath
	T_KEY_MORE,		// eg: ���к���ַ���sudam*,,imadu*
	T_BLANKLINE,	// �ֺ�';' ��Ϊע����
	T_ERROR			// ����ĸ�ʽ

} ConfStringType;


#define KEY_LEN             50
#define SECTION_LEN         30
//
// eg: "firefox.exe,%Favorites%"
//

typedef struct _CONFIG_VALUE_INFO_
{
	/*+0x000*/ struct _CONFIG_VALUE_INFO_* next;
	/*+0x004*/ int NameLength;		// �ַ�������
	/*+0x008*/ char* ValueName;	// �ַ������ݵ�ָ��,��̬����&�ͷ�

} CONFIG_VALUE_INFO, * LPCONFIG_VALUE_INFO;


//
// eg: "OpenFilePath"
//

typedef struct _CONFIG_KEY_INFO_
{
	/*+0x000*/ struct _CONFIG_KEY_INFO_* next; // ָ���¸��ڵ�eg:CloseIpcPath
	/*+0x004*/ char KeyName[KEY_LEN + 1];	// eg:OpenIpcPath
	/*+0x008*/ LPCONFIG_VALUE_INFO ValueHead;		// eg: �ýڵ㽫���һ�������������ڴ洢11,22,33��Щ�ַ���

} CONFIG_KEY_INFO, * LPCONFIG_KEY_INFO;


//
// eg: "[GlobalSetting]"
//

typedef struct _CONFIG_SECTION_INFO_
{
	/*+0x000*/ struct _CONFIG_SECTION_INFO_* next;
	/*+0x004*/ char SectionName[SECTION_LEN + 1];
	/*+0x008*/ LPCONFIG_KEY_INFO KeyHead;

} CONFIG_SECTION_INFO, * LPCONFIG_SECTION_INFO;


#ifdef __cplusplus
extern "C" {
#endif

class CConfig 
{
public:
	LPCONFIG_SECTION_INFO m_pProfile;
	virtual BOOL Load(IN char* filename);
	int AnalysisTextLine(const char* pszText);
	LPCONFIG_SECTION_INFO LoadProfile(const char* filename);
	void GetSectionName(const char* pszText, char* section_name);
	void GetKeyInfo(const char* pszText, char* key_name, LPCONFIG_VALUE_INFO* key_value);
	LPCONFIG_VALUE_INFO AllocateNodeValue();
	int GetStringLength(IN const char* pszText);
	BOOL BuildNodeValue(IN LPCONFIG_VALUE_INFO ListHead, IN const char* szName, IN int Length);
	LPCONFIG_VALUE_INFO FixNodeValue(IN const char* szName, IN int Length);
	void GetKeyInfoEx(IN const char* pszText, OUT LPCONFIG_VALUE_INFO* key_value);
	void FreeProfile(LPCONFIG_SECTION_INFO pProfileInfo);
	void DistroyNodeValue(IN PVOID ListHead);

private:
	char m_szName[MAX_PATH];	// ��Ҫ�����ļ���ȫ·��
};

class CConfigEx :public CConfig 
{
public:
	CConfigEx();
	~CConfigEx();
	BOOL InitData();
	VOID __InitData(LPSTR lpdest, LPSTR lpsrc);
	BOOL _StartThread();
	static DWORD WINAPI WorkThread(LPVOID lpParameter);
	void _ThreadProc();
	// ��ʼ����Ϣ
	BOOL Load();
	BOOL GetDrvPointer(CDriver* drv);
	BOOL Wakeup_R0_InitConfigData();
	BOOL Waitfor_R0_InitConfigData();
private:
	BOOL m_bInitOK;
	BOOL m_bAbortThread;
	HANDLE m_hEvent_InitConfigData_wakeup_R0;  // ��������һ�̻߳�һֱ�ȴ�,��R3���ô��¼��Żἤ��ȴ���
	HANDLE m_hEvent_InitConfigData_wakeup_R3;  // Ӧ�ò�һֱ�ȴ�,��R0����������ʱ�õ����¼�
	HANDLE m_hEvent_waitfor_r0;  // Ӧ�ò�һֱ�ȴ�,��R0����������ʱ�õ����¼�
	HANDLE m_hEvent_wakeup_r0;  // Ӧ�ò�׼�������ݺ�,֪ͨ�ȴ���ROʱ�õ����¼�
	HANDLE m_hWorkThread;

	char m_szEvent_InitConfigData_wakeup_R0[MAX_PATH];
	char m_szEvent_InitConfigData_wakeup_R3[MAX_PATH];
	char m_szEvent_waitfor_r0[MAX_PATH];	// "Global\\Proteinbox_ConfEvent_waitfor_r0"
	char m_szEvent_wakeup_r0[MAX_PATH];	// "Global\\Proteinbox_ConfEvent_wakeup_r0"
	 
	char m_szProcteiBoxMainIni[MAX_PATH];		// ��Ҫ�����ļ���ȫ·��
	char m_szProcteiBoxTemplateIni[MAX_PATH];	// ���������ļ���ȫ·��
	CDriver* m_drv_ProteinBoxDrv;
};

#ifdef __cplusplus
	}
#endif