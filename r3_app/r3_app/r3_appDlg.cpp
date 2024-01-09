
// r3_appDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "r3_app.h"
#include "r3_appDlg.h"
#include "afxdialogex.h"
#include"my_version.h"
#include"ioctl.h"
#include"Work.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
BOOL g_Inited = FALSE;
CString  g_szFileName = L"D:\\1.exe";
TCHAR m_szRootPath[MAX_PATH];

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cr3appDlg 对话框



Cr3appDlg::Cr3appDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_R3_APP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cr3appDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cr3appDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDTEST, &Cr3appDlg::OnBnClickedTest)
END_MESSAGE_MAP()


// Cr3appDlg 消息处理程序

BOOL Cr3appDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_szProcName = (LPWSTR)(LPCTSTR)g_szFileName;
	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(g_szFileName); // 在EDITSELECT控件中显示文件路径

	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(FALSE);
	GetDlgItem(IDUNLOAD)->EnableWindow(FALSE);
	GetDlgItem(IDTEST)->EnableWindow(TRUE);

	GetModuleFileName(NULL, m_szRootPath, ARRSIZEOF(m_szRootPath));
	PathRemoveFileSpec(m_szRootPath);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cr3appDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cr3appDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cr3appDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI Cr3appDlg::OnLoadThread(LPVOID lpParameter)
{
	Cr3appDlg* pThis = (Cr3appDlg*)lpParameter;
	if (NULL != pThis) 
	{
		pThis->_ThreadProc();
	}
	return 0;
}

void Cr3appDlg::_ThreadProc()
{
	BOOL bRet = FALSE;
	if (g_Inited) { return; }
	// 操作配置文件相关
	ShowLogInfo(L"初始化配置文件 \n");
	bRet = HandlerConf();
	if (FALSE == bRet)
	{
		ShowLogInfo(L"error! | HandlerConf() | 操作配置文件失败 \n");
		return;
	}
	ShowLogInfo(L"加载hy.sys \n");
	CHAR buffer[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buffer);
	sprintf(buffer + strlen(buffer), "\\r0_drv.sys");
	bRet = LoadDriver(buffer, ProteinBoxDrv_LinkName, &g_drv_ProteinBoxDrv);
	if (FALSE == bRet)
	{
		ShowLogInfo(L"error! | LoadDriver() | 加载ProteinBox.sys失败 \n");
		goto _error_;
	}
	// 激活R0的等待线程,R3负责将配置文件数据传给驱动
	bRet = g_Conf->GetDrvPointer(g_drv_ProteinBoxDrv);
	if (FALSE == bRet)
	{
		ShowLogInfo(L"error! g_Conf->GetDrvPointer() \n");
		goto _error_;
	}
	bRet = g_Conf->Wakeup_R0_InitConfigData();
	if (FALSE == bRet)
	{
		ShowLogInfo(L"error! g_Conf->Wakeup_R0_InitConfigData() \n");
		goto _error_;
	}
	//
	// 这里要无限等待R0，因为驱动顶多一分钟后会激活我们，所以无限等待，反正一分钟后会有结果
	// 如果等待时间过短，会出问题。
	//
	ShowLogInfo(L"无限等待R0... \n");
	bRet = g_Conf->Waitfor_R0_InitConfigData();
	if (FALSE == bRet)
	{
		ShowLogInfo(L"error! g_Conf->Waitfor_R0_InitConfigData() \n");
		goto _error_;
	}
	ShowLogInfo(L"等待成功，go on \n");
_error_:
	if (g_Conf)
	{
		delete g_Conf;
		g_Conf = NULL;
	}
	g_Inited = FALSE;
	return;
}

void Cr3appDlg::OnBnClickedTest()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (IsDrvLoaded()) 
	{
		ShowLogInfo(L"驱动未被卸载,需重启系统 \n");
		return;
	}*/
	GetDlgItem(IDTEST)->EnableWindow(FALSE);
	HANDLE hThread = CreateThread(NULL, 0, OnLoadThread, this, 0, NULL);
	if (hThread)
		CloseHandle(hThread);

	return;
}

void Cr3appDlg::ShowLogInfo(IN LPWSTR szInfo)
{
	CString Temp;
	GetDlgItem(IDC_EDIT_SHOWINFO)->GetWindowText(Temp);

	CString LogInfo = szInfo;
	Temp.Append(L"\r\n");
	Temp += LogInfo;

	GetDlgItem(IDC_EDIT_SHOWINFO)->SetWindowText(Temp);
	return;
}

BOOL Cr3appDlg::IsDrvLoaded()
{
	HANDLE hFile = CreateFileW(
		g_PBLinkName,
		FILE_READ_ATTRIBUTES | FILE_READ_DATA | SYNCHRONIZE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		0
	);

	if (INVALID_HANDLE_VALUE != hFile)
	{
		CloseHandle(hFile);
		return TRUE;
	}

	return FALSE;
}
