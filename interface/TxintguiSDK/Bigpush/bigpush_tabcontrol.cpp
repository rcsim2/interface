// the raw Win32 API
#include <windows.h>
#include <commctrl.h>
#include "resource.h"

static char g_szClassName[] = "MyWindowClass";
static HINSTANCE g_hInst = NULL;
HICON hIcon = NULL;;
HMENU hMenu = NULL;

DLGTEMPLATE* pTabPage[4];
HWND g_hTabControl = NULL;
HWND g_hTabCurrent = NULL;



// prototypes
LRESULT CALLBACK WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM );

void CenterDialog(HWND hdlg);

void CreateTabControl(HWND hWnd);
void TabChange(HWND hWnd);
void TabCenter(HWND hWnd);

BOOL CALLBACK Tab1DlgProc(HWND hWnd, UINT Message, WPARAM WParam, LPARAM LParam);
BOOL CALLBACK Tab2DlgProc(HWND hWnd, UINT Message, WPARAM WParam, LPARAM LParam);
BOOL CALLBACK Tab3DlgProc(HWND hWnd, UINT Message, WPARAM WParam, LPARAM LParam);
BOOL CALLBACK Tab4DlgProc(HWND hWnd, UINT Message, WPARAM WParam, LPARAM LParam);




// Entry Point: WinMain() 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	WNDCLASSEX WndClass;
	HWND hWnd;
	MSG msg;

	g_hInst = hInstance;

	WndClass.cbSize        = sizeof(WNDCLASSEX);
	WndClass.style         = 0;
	WndClass.lpfnWndProc   = WndProc;
	WndClass.cbClsExtra    = 0;
	WndClass.cbWndExtra    = 0;
	WndClass.hInstance     = g_hInst;
	WndClass.hIcon         = LoadIcon(g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	WndClass.lpszMenuName  = NULL;
	WndClass.lpszClassName = g_szClassName;
	WndClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

 

	//InitCommonControls();

	// Dialog-based app: hWnd = NULL
	// Main Window Proc becomes the Dialog's Callback Proc
	hWnd = CreateDialog( hInstance, MAKEINTRESOURCE(IDD_CONSOLE), (HWND)NULL,
								(DLGPROC)WndProc );



	while(GetMessage(&msg, NULL, 0, 0))
	{
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);
		IsDialogMessage(hWnd, &msg); // intended for modeless dialog boxes!!!   
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	
	switch (Message)
	{
		case WM_INITDIALOG:
			hIcon = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1) );
			SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

			hMenu = GetSystemMenu(hWnd, FALSE);
			if (hMenu != NULL)  {
				EnableMenuItem(hMenu, GetMenuItemID(hMenu,0), MF_GRAYED);
				EnableMenuItem(hMenu, GetMenuItemID(hMenu,2), MF_GRAYED);
				EnableMenuItem(hMenu, GetMenuItemID(hMenu,4), MF_GRAYED);

				AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
				AppendMenu(hMenu, MF_STRING, IDM_ABOUTBOX, "&About...");
			}

			// get the handle to the tab control
			g_hTabControl = GetDlgItem(hWnd,IDC_TAB1);

			// What we need is a tabbed control which contains our prop pages
			CreateTabControl(hWnd);

			// Start up tab
			TabChange(hWnd);

			return TRUE;
			break;
			// You get this if the user want´s to change the tab page
		case WM_NOTIFY:
			if ( ((LPNMHDR)lParam)->code == TCN_SELCHANGE )
				TabChange(hWnd);
		break;
		case WM_SYSCOMMAND:
			switch( (DWORD)wParam )
			{
				case SC_CLOSE:
					DestroyWindow(hWnd);    // this won't kill the app!! 
					PostQuitMessage(0);     // kill!!!
					break;
				case IDM_ABOUTBOX:
					DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd,
								(DLGPROC)AboutProc );
					break;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDOK:
					//EndDialog( hWnd, TRUE );  // this won't kill the app!!!
					DestroyWindow(hWnd);        // this won't kill the app!!!
					PostQuitMessage(0);         // kill!!!
					break;
				case IDCANCEL:
					//EndDialog( hWnd, FALSE );
					DestroyWindow(hWnd);
					PostQuitMessage(0);
					break;
			}
			break;
		case WM_PAINT:
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
			return TRUE;
			break;
		default:
			return FALSE;
	}
	return FALSE;
}


BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{
	switch (uMsg) {
		case WM_INITDIALOG:
			// center dialog over parent
			CenterDialog(hWnd);
			break;
	}
		
	if( WM_COMMAND == uMsg )
		if( IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam) )
			EndDialog( hWnd, TRUE );
	
	 return WM_INITDIALOG == uMsg ? TRUE : FALSE;
}


void CenterDialog(HWND hdlg)
{
	RECT  rcParent;                         // Parent window client rect
	RECT  rcDlg;                            // Dialog window rect
	int   nLeft, nTop;                      // Top-left coordinates
	int   cWidth, cHeight;                  // Width and height
	HWND  hwnd;

	// Get frame window client rect in screen coordinates
	if((hwnd = GetParent(hdlg)) == NULL) {
		rcParent.top = rcParent.left = 0;
		rcParent.right = GetSystemMetrics(SM_CXFULLSCREEN);
		rcParent.bottom = GetSystemMetrics(SM_CYFULLSCREEN);
	}
	else 
		GetWindowRect(hwnd, &rcParent);

	// Determine the top-left point for the dialog to be centered
	GetWindowRect(hdlg, &rcDlg);
	cWidth  = rcDlg.right  - rcDlg.left;
	cHeight = rcDlg.bottom - rcDlg.top;
	nLeft   = rcParent.left + 
			(((rcParent.right  - rcParent.left) - cWidth ) / 2);
	nTop    = rcParent.top  +
			(((rcParent.bottom - rcParent.top ) - cHeight) / 2);
	if (nLeft < 0) nLeft = 0;
	if (nTop  < 0) nTop  = 0;

	// Place the dialog
	MoveWindow(hdlg, nLeft, nTop, cWidth, cHeight, TRUE);
	return;
}


void CreateTabControl(HWND hWnd)
{
	TC_ITEM tcItem[4];

	tcItem[0].mask = TCIF_IMAGE|TCIF_TEXT;
	tcItem[0].pszText = "Resources";
	tcItem[0].iImage = -1;

	tcItem[1].mask = TCIF_IMAGE|TCIF_TEXT;
	tcItem[1].pszText = "Channels";
	tcItem[1].iImage = -1;

	tcItem[2].mask = TCIF_IMAGE|TCIF_TEXT;
	tcItem[2].pszText = "Mapping";
	tcItem[2].iImage = -1;

	tcItem[3].mask = TCIF_IMAGE|TCIF_TEXT;
	tcItem[3].pszText = "Sensitivity";
	tcItem[3].iImage = -1;

	SendMessage( g_hTabControl,TCM_INSERTITEM,0,(LPARAM)&tcItem[0] );
	SendMessage( g_hTabControl,TCM_INSERTITEM,1,(LPARAM)&tcItem[1] );
	SendMessage( g_hTabControl,TCM_INSERTITEM,2,(LPARAM)&tcItem[2] );
	SendMessage( g_hTabControl,TCM_INSERTITEM,3,(LPARAM)&tcItem[3] );

	
	// Note: de property pages hebben: Styles->Border->None
	pTabPage[0] = (DLGTEMPLATE *)LockResource( LoadResource(g_hInst, FindResource(NULL,MAKEINTRESOURCE(IDD_PROPPAGE1),RT_DIALOG)) );
    pTabPage[1] = (DLGTEMPLATE *)LockResource( LoadResource(g_hInst, FindResource(NULL,MAKEINTRESOURCE(IDD_PROPPAGE2),RT_DIALOG)) );
    pTabPage[2] = (DLGTEMPLATE *)LockResource( LoadResource(g_hInst, FindResource(NULL,MAKEINTRESOURCE(IDD_PROPPAGE3),RT_DIALOG)) );
    pTabPage[3] = (DLGTEMPLATE *)LockResource( LoadResource(g_hInst, FindResource(NULL,MAKEINTRESOURCE(IDD_PROPPAGE4),RT_DIALOG)) );

}



void TabChange(HWND hWnd)
{ 
    static int  NewPage;
 
    if (g_hTabCurrent != NULL)
		DestroyWindow(g_hTabCurrent); 
 
    NewPage = TabCtrl_GetCurSel(g_hTabControl); 
 
    if(NewPage == 0) g_hTabCurrent = CreateDialogIndirect(g_hInst,pTabPage[NewPage],g_hTabControl,(DLGPROC)Tab1DlgProc); 
    if(NewPage == 1) g_hTabCurrent = CreateDialogIndirect(g_hInst,pTabPage[NewPage],g_hTabControl,(DLGPROC)Tab2DlgProc); 
    if(NewPage == 2) g_hTabCurrent = CreateDialogIndirect(g_hInst,pTabPage[NewPage],g_hTabControl,(DLGPROC)Tab3DlgProc); 
    if(NewPage == 3) g_hTabCurrent = CreateDialogIndirect(g_hInst,pTabPage[NewPage],g_hTabControl,(DLGPROC)Tab4DlgProc); 
 
    ShowWindow(g_hTabCurrent, SW_SHOW);
}



// Center the tab page within the tab control
void TabCenter(HWND hWnd)
{
	static RECT WinRect;
	static RECT TabRect;

	GetWindowRect(g_hTabControl, &TabRect);
	GetWindowRect(GetParent(hWnd), &WinRect);

	WinRect.left = TabRect.left - WinRect.left;
	WinRect.top  = TabRect.top  - WinRect.top;
 
	TabCtrl_AdjustRect(g_hTabControl, FALSE, &WinRect); 
 
	SetWindowPos(hWnd, HWND_TOP, WinRect.left, WinRect.top, 0, 0, SWP_NOSIZE);
 
}




// This is the dialog of a tab page
BOOL CALLBACK Tab1DlgProc(HWND hWnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
  static RECT WinRect;
  static RECT TabRect;
  static char Temp[256];
  static LONG Index;
 
  switch(Message)
  {
      case WM_INITDIALOG:
          TabCenter(hWnd);
		  break;
 
      default:
          return(FALSE);
  }
 
  return(0);
}


// This is the dialog of a tab page
BOOL CALLBACK Tab2DlgProc(HWND hWnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
  static RECT WinRect;
  static RECT TabRect;
  static char Temp[256];
  static LONG Index;
 
  switch(Message)
  {
      case WM_INITDIALOG:
          TabCenter(hWnd);
		  break;
 
      default:
          return(FALSE);
  }
 
  return(0);
}


// This is the dialog of a tab page
BOOL CALLBACK Tab3DlgProc(HWND hWnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
  static RECT WinRect;
  static RECT TabRect;
  static char Temp[256];
  static LONG Index;
 
  switch(Message)
  {
      case WM_INITDIALOG:
          TabCenter(hWnd);
		  break;
 
      default:
          return(FALSE);
  }
 
  return(0);
}


// This is the dialog of a tab page
BOOL CALLBACK Tab4DlgProc(HWND hWnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
  static RECT WinRect;
  static RECT TabRect;
  static char Temp[256];
  static LONG Index;
 
  switch(Message)
  {
      case WM_INITDIALOG:
          TabCenter(hWnd);
		  break;
 
      default:
          return(FALSE);
  }
 
  return(0);
}