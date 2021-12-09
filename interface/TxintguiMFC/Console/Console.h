// Console.h : main header file for the CONSOLE application
//

#if !defined(AFX_CONSOLE_H__C0839E28_5748_11D4_A852_444553540001__INCLUDED_)
#define AFX_CONSOLE_H__C0839E28_5748_11D4_A852_444553540001__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CConsoleApp:
// See Console.cpp for the implementation of this class
//

class CConsoleApp : public CWinApp
{
public:
	CConsoleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsoleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CConsoleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONSOLE_H__C0839E28_5748_11D4_A852_444553540001__INCLUDED_)
