// MyPropertySheet.h : header file
//
// CMyPropertySheet is a modeless property sheet that is 
// created once and not destroyed until the application
// closes.  It is initialized and controlled from
// CPropertyFrame.
 // CMyPropertySheet has been customized to include
// a preview window.
 
#ifndef __MYPROPERTYSHEET_H__
#define __MYPROPERTYSHEET_H__

#include "MyPropertyPage1.h"
#include "PreviewWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

class CMyPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyPropertySheet)

// Construction
public:
	CMyPropertySheet(CWnd* pWndParent = NULL);

// Attributes
public:
	CMyPropertyPage1 m_Page1;
	CMyPropertyPage2 m_Page2;
	CMyPropertyPage3 m_Page3;
	CMyPropertyPage4 m_Page4;
	CPreviewWnd m_wndPreview;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPropertySheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyPropertySheet();
		 virtual BOOL OnInitDialog();
	 virtual void PostNcDestroy();

// Generated message map functions
protected:
	//{{AFX_MSG(CMyPropertySheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __MYPROPERTYSHEET_H__
