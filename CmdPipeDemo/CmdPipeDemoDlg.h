
// CmdPipeDemoDlg.h: 头文件
//

#pragma once


// CCmdPipeDemoDlg 对话框
class CCmdPipeDemoDlg : public CDialogEx
{
// 构造
public:
	CCmdPipeDemoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMDPIPEDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	//父进程读
	HANDLE m_pReadHandle;
	//父进程写
	HANDLE m_pWriteHandle;
	//子进程读
	HANDLE m_cReadHandle;
	//子进程写
	HANDLE m_cWriteHandle;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCreatepipe();
	
	afx_msg void OnBnClickedCreateChildProcess();
	afx_msg void OnBnClickedWritepipe();
	afx_msg void OnBnClickedReadpipe();
};
