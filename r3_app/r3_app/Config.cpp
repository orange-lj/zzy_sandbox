#include "Config.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib") 

CConfigEx::CConfigEx():
	m_bInitOK(FALSE),
	m_bAbortThread(FALSE),
	m_drv_ProteinBoxDrv(NULL),
	m_hEvent_InitConfigData_wakeup_R0(NULL),
	m_hEvent_InitConfigData_wakeup_R3(NULL),
	m_hEvent_waitfor_r0(NULL),
	m_hEvent_wakeup_r0(NULL),
	m_hWorkThread(NULL)
{
	BOOL bRet = FALSE;
	RtlZeroMemory(m_szProcteiBoxMainIni, MAX_PATH);
	RtlZeroMemory(m_szProcteiBoxTemplateIni, MAX_PATH);

	strcpy(m_szEvent_InitConfigData_wakeup_R0, "Global\\Proteinbox_ConfEvent_InitConfigData_wakeup_R0");
	strcpy(m_szEvent_InitConfigData_wakeup_R3, "Global\\Proteinbox_ConfEvent_InitConfigData_wakeup_R3");
	strcpy(m_szEvent_waitfor_r0, "Global\\Proteinbox_ConfEvent_waitfor_r0");
	strcpy(m_szEvent_wakeup_r0, "Global\\Proteinbox_ConfEvent_wakeup_r0");

	bRet = InitData();
}

BOOL CConfigEx::InitData()
{
	BOOL bRet = FALSE;
	if (TRUE == m_bInitOK) { return TRUE; }

	//
	// 1. 初始化配置文件
	//
	__InitData(m_szProcteiBoxMainIni, CONFIG_FILE_NAME_MAIN);
	// 判断文件是否存在
	bRet = PathFileExistsA(m_szProcteiBoxMainIni);
	if (FALSE == bRet)
	{
		return FALSE;
	}
	__InitData(m_szProcteiBoxTemplateIni, CONFIG_FILE_NAME_TMPL);
	// 判断文件是否存在
	bRet = PathFileExistsA(m_szProcteiBoxTemplateIni);
	if (FALSE == bRet)
	{
		//MYTRACE(" \n");
		return FALSE;
	}

	//
	// 2. 创建事件对象和工作线程
	//
	m_hEvent_InitConfigData_wakeup_R0 = CreateEventA(NULL, FALSE, FALSE, m_szEvent_InitConfigData_wakeup_R0);
	if (NULL == m_hEvent_InitConfigData_wakeup_R0)
	{
		//MYTRACE("error! | CConfigEx::InitData() - CreateEvent | 创建事件对象失败: %s \n", m_szEvent_InitConfigData_wakeup_R0);
		return FALSE;
	}

	m_hEvent_InitConfigData_wakeup_R3 = CreateEventA(NULL, FALSE, FALSE, m_szEvent_InitConfigData_wakeup_R3);
	if (NULL == m_hEvent_InitConfigData_wakeup_R3)
	{
		//MYTRACE("error! | CConfigEx::InitData() - CreateEvent | 创建事件对象失败: %s \n", m_szEvent_InitConfigData_wakeup_R3);
		return FALSE;
	}

	m_hEvent_waitfor_r0 = CreateEventA(NULL, FALSE, FALSE, m_szEvent_waitfor_r0);
	if (NULL == m_hEvent_waitfor_r0)
	{
		//MYTRACE("error! | CConfigEx::InitData() - CreateEvent | 创建事件对象失败: %s \n", m_szEvent_waitfor_r0);
		return FALSE;
	}

	m_hEvent_wakeup_r0 = CreateEventA(NULL, FALSE, FALSE, m_szEvent_wakeup_r0);
	if (NULL == m_hEvent_wakeup_r0)
	{
		//MYTRACE("error! | CConfigEx::InitData() - CreateEvent | 创建事件对象失败: %s \n", m_szEvent_waitfor_r0);
		return FALSE;
	}
	bRet = _StartThread();
	return TRUE;
}

VOID CConfigEx::__InitData(LPSTR lpdest, LPSTR lpsrc)
{
	CHAR szFileName[MAX_PATH];
	GetModuleFileNameA(NULL, szFileName, ARRSIZEOF(szFileName));
	PathRemoveFileSpecA(szFileName);
	PathAppendA(szFileName, lpsrc);
	RtlZeroMemory(lpdest, MAX_PATH);
	strcpy(lpdest, szFileName);
}

BOOL CConfigEx::_StartThread()
{
	m_hWorkThread = CreateThread(NULL, 0, WorkThread, this, 0, NULL);
	if (NULL == m_hWorkThread)
	{
		//MYTRACE("error! | CConfigEx::_StartThread(); | 创建工作线程失败\n");
		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI CConfigEx::WorkThread(LPVOID lpParameter)
{
	CConfigEx* pThis = (CConfigEx*)lpParameter;
	if (NULL != pThis)
	{
		pThis->_ThreadProc();
	}
	return 0;
}

void CConfigEx::_ThreadProc()
{
	DWORD dwWaitVal;
	Load();
	while (1) 
	{
		dwWaitVal = WaitForSingleObject(m_hEvent_waitfor_r0, INFINITE);
		if (TRUE == m_bAbortThread)
		{
			//MYTRACE("工作线程接收到退出消息, 退出之. \n");
			break;
		}
		if (WAIT_FAILED == dwWaitVal)
			break;
		if (WAIT_OBJECT_0 != dwWaitVal) { continue; }

	}
}

BOOL CConfigEx::Load()
{
	return CConfig::Load(m_szProcteiBoxMainIni);
}

BOOL CConfigEx::GetDrvPointer(CDriver* drv)
{
	m_drv_ProteinBoxDrv = drv;
	if (NULL == m_drv_ProteinBoxDrv)
	{
		//MYTRACE("error! | CConfigEx::GetDrvPointer(); | 参数不合法 \n");
		return FALSE;
	}
	return TRUE;
}

BOOL CConfigEx::Wakeup_R0_InitConfigData()
{
	if (NULL == m_hEvent_InitConfigData_wakeup_R0) { return FALSE; }

	SetEvent(m_hEvent_InitConfigData_wakeup_R0); // 置事件为TRUE
	return TRUE;
}

BOOL CConfigEx::Waitfor_R0_InitConfigData()
{
	DWORD dwTimeout = 1 * 1000; // 2秒钟
	if (NULL == m_hEvent_InitConfigData_wakeup_R3) { return FALSE; }

	DWORD dwWaitResult = WaitForSingleObject(m_hEvent_InitConfigData_wakeup_R3, /*dwTimeout*/ INFINITE);

	if (WAIT_FAILED == dwWaitResult || WAIT_TIMEOUT == dwWaitResult)
	{
		return FALSE;
	}

	return TRUE;
}


CConfigEx::~CConfigEx()
{
	//UnLoad();
}

BOOL CConfig::Load(IN char* filename)
{
	if (NULL == filename)
	{
		// printf( "error! | CConfig::Load | Invalid Paramaters; failed! \n" );
		return FALSE;
	}
	strcpy(m_szName, filename);
	m_pProfile = LoadProfile(m_szName);
	if (m_pProfile) { return TRUE; }

	return FALSE;
}

int CConfig::AnalysisTextLine(const char* pszText)
{
	int len = strlen(pszText);
	int ret = T_KEY_MORE;
	char* p1 = NULL;

	if (len == 0 || pszText[0] == ';')  // line start with ';' is considered as comment line
	{
		ret = T_BLANKLINE;
	}
	else if ((pszText[0] == '[') && (pszText[len - 1] == ']'))
	{
		ret = T_SECTION;
	}
	else
	{
		p1 = (char*)strchr(pszText, '=');
		if ((p1 != NULL) && (p1 - (char*)pszText >= 1))
		{
			ret = T_KEY;
		}
	}

	return ret;
}

LPCONFIG_SECTION_INFO CConfig::LoadProfile(const char* filename)
{
	char buff[256] = { '\0' };
	CONFIG_SECTION_INFO* pResult = NULL;
	CONFIG_SECTION_INFO* pLastSec = NULL;
	CONFIG_SECTION_INFO* pTempSec = NULL;
	CONFIG_KEY_INFO* pTempKey = NULL;
	CONFIG_KEY_INFO* pLastKey = NULL;
	char* lpLastKeyName = NULL;
	int Length = 0;

	FILE* fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (!feof(fp))
		{
			if (NULL == fgets(buff, 256, fp)) { continue; }

			Length = strlen(buff) - 1;

			if (0x0a == *(PBYTE)(buff + Length))
			{
				buff[Length] = '\0';    // erase last 0x0a(\n)
			}

			switch (AnalysisTextLine(buff))
			{
			case T_SECTION:
				pTempSec = (CONFIG_SECTION_INFO*)malloc(sizeof(CONFIG_SECTION_INFO));
				//CHECK_MEM(pTempSec);
				GetSectionName(buff, pTempSec->SectionName);

				pTempSec->next = NULL;
				pTempSec->KeyHead = NULL;
				if (pResult == NULL)
				{
					pResult = pTempSec;
				}

				if (pLastSec != NULL)
				{
					pLastSec->next = pTempSec;
				}

				pLastSec = pTempSec;
				pLastKey = NULL;
				break;

			case T_KEY:
				if (pLastSec != NULL)
				{
					pTempKey = (CONFIG_KEY_INFO*)malloc(sizeof(CONFIG_KEY_INFO));
					//CHECK_MEM(pTempKey);
					GetKeyInfo(buff, pTempKey->KeyName, &pTempKey->ValueHead);
					lpLastKeyName = pTempKey->KeyName;
					pTempKey->next = NULL;
					if (pLastKey != NULL)
					{
						pLastKey->next = pTempKey;
					}
					else
					{
						pLastSec->KeyHead = pTempKey;
					}

					pLastKey = pTempKey;
				}
				break;

			case T_KEY_MORE:
				if (pLastKey != NULL)
				{
					GetKeyInfoEx(buff, &pLastKey->ValueHead);
				}
				break;

			case T_BLANKLINE:
				break;

			default:
				// printf( "profile %s format not support \n", filename );
				FreeProfile(pResult);
				fclose(fp);
				return NULL;
			}
		}

		fclose(fp);
	}

	return pResult;
}

void CConfig::GetSectionName(const char* pszText, char* section_name)
{
	int length = min(strlen(pszText) - 2, SECTION_LEN);

	strncpy(section_name, pszText + 1, length);
	section_name[length] = '\0';

	return;
}

void CConfig::GetKeyInfo(const char* pszText, char* key_name, LPCONFIG_VALUE_INFO* key_value)
{
	BOOL bOver = FALSE;

	//
	// 1. 校验参数合法性
	//

	if (NULL == pszText || '\0' == *pszText)
	{
		// printf( "error! | CConfig::GetKeyInfo() | Invalid Paramaters; failed! \n" );
		return;
	}

	//
	// 2. 获取Key的名字
	//

	char* p1 = (char*)strchr(pszText, '=');
	if (p1 && key_name)
	{
		while (pszText < p1) { *key_name++ = *pszText++; }
		*key_name = '\0';

		p1++;
	}
	else
	{
		p1 = (char*)pszText;
	}

	// 为该Value分配一个链表头,不管等号后面有无内容,都要分配一个表头
	*key_value = AllocateNodeValue();
	if (NULL == *key_value) { return; }

	LPCONFIG_VALUE_INFO pValueHead = (LPCONFIG_VALUE_INFO)*key_value;

	if ('\0' == *p1) { return; } // 等号'='后面无内容

	//
	// 3. 获取Value的内容
	//

	int Length = 0;
	char* pStart = p1;
	char* pEnd = strchr(p1, ',');

	if (NULL == pEnd)
	{
		bOver = TRUE;
		Length = GetStringLength(pStart); // 等号后有内容,但是无逗号,说明就一个字符串
	}
	else
	{
		Length = (int)(pEnd - pStart); // 取第一截逗号前的字符串内容,填充至表头
	}

	pValueHead->NameLength = Length + 1;
	pValueHead->ValueName = (char*)malloc(Length + 1);
	if (pValueHead->ValueName)
	{
		strncpy(pValueHead->ValueName, pStart, Length);
		pValueHead->ValueName[Length] = '\0';
	}

	if (TRUE == bOver) { return; }

	pStart += Length + 1;
	if ('\0' == *pStart) { return; }

	pEnd = strchr(pStart, ',');
	if (NULL == pEnd)
	{
		BuildNodeValue(pValueHead, pStart, GetStringLength(pStart));
		return;
	}

	// 以逗号','为截断，保存各段字符串到链表中
	while (*pStart != '\0')
	{
		// 得到当前字符串的长度
		Length = (int)(pEnd - pStart);

		// 若长度为0，表明内容为"=,2222"(等号连着逗号) 或者 "11,,22"(2个连着的逗号)，跳过继续
		if (0 == Length) { goto _WHILE_NEXT_; }

		// 长度不为0，表明有内容，拷贝到新申请的内存中
		if (FALSE == BuildNodeValue(pValueHead, pStart, Length)) { goto _WHILE_NEXT_; }

	_WHILE_NEXT_:
		pStart += Length + 1;
		pEnd = strchr(pStart, ',');
		if (NULL == pEnd)
		{
			// 说明是最后一个字符串了。
			BuildNodeValue(pValueHead, pStart, GetStringLength(pStart));
			break;
		}
	}

	return;
}

LPCONFIG_VALUE_INFO CConfig::AllocateNodeValue()
{
	LPCONFIG_VALUE_INFO pNode = (LPCONFIG_VALUE_INFO)malloc(sizeof(CONFIG_VALUE_INFO));
	if (NULL == pNode)
	{
		// printf( "error! | AllocateNodeValue() | 申请内存失败 \n" );
		return NULL;
	}

	pNode->next = NULL;
	pNode->NameLength = 0;
	pNode->ValueName = NULL;

	return pNode;
}

int CConfig::GetStringLength(IN const char* pszText)
{
	int Length = 0;

	if (NULL == pszText)
	{
		return 0;
	}

	while (*pszText++)
	{
		Length++;
	}

	return Length;
}

BOOL CConfig::BuildNodeValue(IN LPCONFIG_VALUE_INFO ListHead, IN const char* szName, IN int Length)
{
	LPCONFIG_VALUE_INFO pNode = NULL;

	//
	// 1. 参数合法性检测
	//

	if (NULL == ListHead || NULL == szName || 0 == Length)
	{
		// printf( "error! | BuildNodeValue() | 参数不合法 \n" );
		return FALSE;
	}

	//
	// 2. 填充Value节点
	//

	pNode = FixNodeValue(szName, Length);
	if (NULL == pNode)
	{
		// printf( "error! | BuildNodeValue() - FixNodeValue() | 填充节点失败 \n" );
		return FALSE;
	}

	//
	// 3. 插入到单向链表的表头
	//

	if (NULL == ListHead->next)
	{
		ListHead->next = pNode;
	}
	else
	{
		pNode->next = ListHead->next;
		ListHead->next = pNode;
	}

	return TRUE;
}

LPCONFIG_VALUE_INFO CConfig::FixNodeValue(IN const char* szName, IN int Length)
{
	LPCONFIG_VALUE_INFO pNode = NULL;

	//
	// 1. 参数合法性检测
	//

	if (NULL == szName || 0 == Length)
	{
		// printf( "error! | InsertValue() | 参数不合法 \n" );
		return NULL;
	}

	//
	// 2.1 分配新节点，填充之
	//

	pNode = AllocateNodeValue();
	if (NULL == pNode)
	{
		// printf( "error! | InsertValue() | 申请内存失败 \n" );
		return NULL;
	}

	// 2.2 为字符串分配内存
	pNode->NameLength = Length + 1;
	pNode->ValueName = (char*)malloc(Length + 1);
	if (pNode->ValueName)
	{
		strncpy(pNode->ValueName, szName, Length);
		pNode->ValueName[Length] = '\0';
	}

	return pNode;
}

void CConfig::GetKeyInfoEx(IN const char* pszText, OUT LPCONFIG_VALUE_INFO* key_value)
{
	char* p1 = (char*)pszText;
	LPCONFIG_VALUE_INFO pValueHead = (LPCONFIG_VALUE_INFO)*key_value;

	//
	// 1. 校验参数合法性
	//

	if ((NULL == p1) || ('\0' == *p1))
	{
		// printf( "error! | CConfig::GetKeyInfoEx() | Invalid Paramaters; failed! \n" );
		return;
	}

	//
	// 2. 追加Value的内容
	//

	int Length = 0;
	char* pStart = p1;
	char* pEnd = strchr(p1, ',');
	if (NULL == pEnd)
	{
		BuildNodeValue(pValueHead, pStart, GetStringLength(pStart));
		return;
	}

	while (*pStart != '\0')
	{
		// 得到当前字符串的长度
		Length = (int)(pEnd - pStart);

		// 若长度为0，表明内容为"=,2222"(等号连着逗号) 或者 "11,,22"(2个连着的逗号)，跳过继续
		if (0 == Length) { goto _WHILE_NEXT_; }

		// 长度不为0，表明有内容，拷贝到新申请的内存中
		BuildNodeValue(pValueHead, pStart, Length);

	_WHILE_NEXT_:
		pStart += Length + 1;
		pEnd = strchr(pStart, ',');
		if (NULL == pEnd) // 说明是最后一个字符串了。
		{
			BuildNodeValue(pValueHead, pStart, GetStringLength(pStart));
			break;
		}
	}

	return;
}

void CConfig::FreeProfile(LPCONFIG_SECTION_INFO pProfileInfo)
{
	CONFIG_SECTION_INFO* pTempSec = NULL;
	CONFIG_SECTION_INFO* pCurrentSec = pProfileInfo;
	CONFIG_KEY_INFO* pTempKey = NULL;
	CONFIG_KEY_INFO* pCurrentKey = NULL;

	while (pCurrentSec != NULL)
	{
		pTempSec = pCurrentSec;
		pCurrentKey = pTempSec->KeyHead;
		pCurrentSec = pCurrentSec->next;

		while (pCurrentKey != NULL)
		{
			pTempKey = pCurrentKey;
			pCurrentKey = pCurrentKey->next;

			DistroyNodeValue((PVOID)pTempKey->ValueHead);
			free(pTempKey);
		}

		free(pTempSec);
	}

	return;
}


void CConfig::DistroyNodeValue(IN PVOID ListHead)
{
	LPCONFIG_VALUE_INFO pNodeHead = (LPCONFIG_VALUE_INFO)ListHead;
	LPCONFIG_VALUE_INFO pNodeCurrent, pValueNext;

	//
	// 1. 参数合法性检测
	//

	if (NULL == ListHead)
	{
		// printf( "error! | DistroyValue() | 参数不合法 \n" );
		return;
	}

	//
	// 2. 释放Key节点对应的所有字符串内存
	//

	pNodeCurrent = pNodeHead;
	while (pNodeCurrent)
	{
		pValueNext = pNodeCurrent->next; // 得到下个节点

		// 释放当前节点
		kfree(pNodeCurrent->ValueName);
		kfree(pNodeCurrent);

		pNodeCurrent = pValueNext;
	}

	return;
}