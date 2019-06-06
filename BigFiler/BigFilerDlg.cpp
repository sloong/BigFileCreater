
// BigFilerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BigFiler.h"
#include "BigFilerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBigFilerDlg dialog



CBigFilerDlg::CBigFilerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBigFilerDlg::IDD, pParent)
	, m_strPath(_T(""))
	, m_dSize(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBigFilerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_dSize);
}

BEGIN_MESSAGE_MAP(CBigFilerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CBigFilerDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDOK, &CBigFilerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBigFilerDlg message handlers

BOOL CBigFilerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBigFilerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBigFilerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBigFilerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBigFilerDlg::OnBnClickedButtonSelect()
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME time;
	CString strTime;
	//Get the local time
	GetLocalTime(&time);
	strTime.Format(TEXT("%d%d%d%d%d%d%d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wMinute, time.wMilliseconds);
	CFileDialog saveDlg(FALSE, TEXT("Big Temp Files (*TMP)"), strTime, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT("Big Temp Files (*.TMP)|*.TMP;)||"));
	saveDlg.m_ofn.lpstrTitle = TEXT("Select path to save file");
	saveDlg.m_ofn.nMaxFile = MAX_PATH;  

	// If user click ok button, do it, else, not do anything.
	if( IDOK == saveDlg.DoModal() )
	{
		m_strPath = saveDlg.GetPathName();
		ULARGE_INTEGER FreeAv,TotalBytes,FreeBytes;
	
		//Get the disk free space
		GetDiskFreeSpaceEx(saveDlg.GetFolderPath(),&FreeAv,&TotalBytes,&FreeBytes);
		long LeaveMb=(long)(FreeBytes.QuadPart/1024/1024);

		//to GB
		m_dFreeSpace =(double)LeaveMb/1024;
		// Format the string.
		CString m_strFreeSpace;
		m_strFreeSpace.Format(TEXT("Free Space is %.2lf GB"), m_dFreeSpace);
		SetDlgItemText(IDC_STATIC_DISKINFO,m_strFreeSpace);
		SetDlgItemText(IDC_EDIT_PATH,m_strPath);
	}
}


void CBigFilerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	UpdateData(TRUE);
	if ( m_dSize <= 0 || m_strPath == L"" )
	{
		MessageBox(TEXT("Input error!"));
		return;
	}
	if ( m_dSize > m_dFreeSpace )
	{
		MessageBox(TEXT("Disk is no enough space! "));
		return;
	}

	CString strCmd;
	ULONGLONG ullSize = (ULONGLONG)m_dSize * 1024 *1024 *1024;
	//if the file is exist ,delete it.
	DeleteFile(m_strPath);

	strCmd.Format(TEXT("fsutil file createnew %s %I64u"), m_strPath, ullSize);
	//system function can run the cmd line, but it cannot show hide.
	//system(strCmd);
	LPTSTR  pStr = (LPTSTR)strCmd.GetBuffer(strCmd.GetLength()*sizeof(TCHAR)); 
	//This function is a old function ,main in the 16bit computer.
	//in this application , we used the CreateProcess.
	//WinExec(pStr,SW_HIDE);

	//setting some must param
	STARTUPINFO si = {0}; 
	si.cb = sizeof(STARTUPINFO); 
	si.dwFlags = STARTF_USESHOWWINDOW; 
	//run the app with not show window.
	si.wShowWindow = SW_HIDE; 
	PROCESS_INFORMATION pi; 
	//setting end

	//This function well call the "CMD" to run the app, 
	//so if you want run the cmd order, 
	//just give the cmd order, not the app name.
	if( 0 != CreateProcess(NULL,pStr,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
	{
		STARTUPINFO st = {0}; 
		st.cb = sizeof(STARTUPINFO); 
		st.dwFlags = STARTF_USESHOWWINDOW; 
		//run the app with not show window.
		st.wShowWindow = SW_SHOW; 
		PROCESS_INFORMATION pr; 

		CString str = TEXT("explorer.exe /select, ");
		str += m_strPath;
		LPTSTR path = str.GetBuffer(str.GetLength()*sizeof(TCHAR));
		if( 0 == CreateProcess(NULL,path,NULL,NULL,TRUE,0,NULL,NULL,&st,&pr))
		{
			MessageBox(TEXT("Error."));
		}
	}
	else
	{
		MessageBox(TEXT("Create failed."));
	}
}
