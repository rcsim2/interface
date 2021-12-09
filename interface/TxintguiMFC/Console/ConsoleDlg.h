// ConsoleDlg.h : header file
//

#if !defined(AFX_CONSOLEDLG_H__C0839E2A_5748_11D4_A852_444553540001__INCLUDED_)
#define AFX_CONSOLEDLG_H__C0839E2A_5748_11D4_A852_444553540001__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CConsoleDlg dialog

class CConsoleDlg : public CDialog
{
// Construction
public:
	CConsoleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CConsoleDlg)
	enum { IDD = IDD_CONSOLE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsoleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CConsoleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOLEDLG_H__C0839E2A_5748_11D4_A852_444553540001__INCLUDED_)
