﻿
// r3_appDlg.h: 头文件
//

#pragma once


// Cr3appDlg 对话框
class Cr3appDlg : public CDialogEx
{
// 构造
public:
	Cr3appDlg(CWnd* pParent = nullptr);	// 标准构造函数
	wchar_t* m_szProcName;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_R3_APP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTest();
	void ShowLogInfo(IN LPWSTR szInfo);
	BOOL IsDrvLoaded();
	static DWORD WINAPI OnLoadThread(LPVOID lpParameter);
	void _ThreadProc();
};
