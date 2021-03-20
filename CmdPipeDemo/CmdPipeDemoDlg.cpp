
// CmdPipeDemoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CmdPipeDemo.h"
#include "CmdPipeDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CCmdPipeDemoDlg 对话框



CCmdPipeDemoDlg::CCmdPipeDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMDPIPEDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCmdPipeDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCmdPipeDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_CREATEPIPE, &CCmdPipeDemoDlg::OnBnClickedCreatepipe)
	
	ON_BN_CLICKED(BTN_CREATE_CHILD_PROCESS, &CCmdPipeDemoDlg::OnBnClickedCreateChildProcess)
	ON_BN_CLICKED(BTN_WRITEPIPE, &CCmdPipeDemoDlg::OnBnClickedWritepipe)
	ON_BN_CLICKED(BTN_READPIPE, &CCmdPipeDemoDlg::OnBnClickedReadpipe)
END_MESSAGE_MAP()


// CCmdPipeDemoDlg 消息处理程序

BOOL CCmdPipeDemoDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCmdPipeDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCmdPipeDemoDlg::OnPaint()
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
HCURSOR CCmdPipeDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCmdPipeDemoDlg::OnBnClickedCreatepipe()
{
	SECURITY_ATTRIBUTES sa;
	sa.bInheritHandle = true;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(sa);

	if (::CreatePipe(&m_pReadHandle, &m_cWriteHandle, &sa, 0) && ::CreatePipe(&m_cReadHandle, &m_pWriteHandle, &sa, 0)) {
		AfxMessageBox(_T("创建管道成功\n"));
		return;
	}
}




void CCmdPipeDemoDlg::OnBnClickedCreateChildProcess()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	
	si.hStdError = m_cWriteHandle;
	si.hStdOutput = m_cWriteHandle;
	si.hStdInput = m_cReadHandle;
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	ZeroMemory(&pi, sizeof(pi));

	
	TCHAR szCommand[] = _T("cmd.exe");
	// Start the child process. 
	//unicode编译模式下第二个参数Command line 不能设置为只读
	if (!CreateProcess(NULL,   // No module name (use command line)
		szCommand,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		TRUE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		AfxMessageBox(_T("CreateProcess failed\n"));
		return;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

}


void CCmdPipeDemoDlg::OnBnClickedWritepipe()
{
	CString buffer;
	this->GetDlgItemText(EDT_WRITEDATA, buffer);
	buffer += _T("\r\n");
	DWORD dwWriteReturn;
	if (::WriteFile(m_pWriteHandle, buffer.GetBuffer(), buffer.GetLength(), &dwWriteReturn, NULL)) {
		AfxMessageBox(_T("写入成功"));
		
	}
}


void CCmdPipeDemoDlg::OnBnClickedReadpipe()
{
	DWORD dwAvaliabe=0;
	DWORD numberReadBytes;
	if (::PeekNamedPipe(m_pReadHandle, NULL, 0, NULL, &dwAvaliabe, NULL) && dwAvaliabe > 0) {
		//让最后一个字节为0
		void* szBuffer =malloc(dwAvaliabe+1);
		memset(szBuffer, 0, dwAvaliabe+1);
		if (::ReadFile(m_pReadHandle, szBuffer, dwAvaliabe, &numberReadBytes, NULL)) {
			char szData[0x1000] = { 0 };
			_snprintf(szData, sizeof(szData)-sizeof(char), "cmd result:%s", szBuffer);
			OutputDebugStringA(szData);
		}
		free(szBuffer);

	}
}
