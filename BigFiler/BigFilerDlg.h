
// BigFilerDlg.h : header file
//

#pragma once


// CBigFilerDlg dialog
class CBigFilerDlg : public CDialogEx
{
// Construction
public:
	CBigFilerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BIGFILER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strPath;
	double m_dSize;
	double m_dFreeSpace;
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedOk();
};
