// the raw Win32 API
#include <windows.h>
#include <commctrl.h>
#include "resource.h"

static char g_szClassName[] = "MyWindowClass";
static HINSTANCE g_hInst = NULL;
HICON hIcon = NULL;;
HMENU hMenu = NULL;

// prototypes
BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM );
void CenterDialog(HWND hdlg);

VOID DoPropertySheet(HWND hwndOwner);
BOOL CALLBACK Prop1DialogProc();// HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM );
BOOL CALLBACK Prop2DialogProc();// HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM );



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
			
			// This will create a separate prop sheet
			//DoPropertySheet( GetDlgItem(hWnd, IDC_TAB1) );

			// What we need is a tabbed control which contains our prop pages


			return TRUE;
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

	
	//TC_ITEM
	//SendDlgItemMessage( hWnd, IDC_TAB1, TCM_INSERTITEM, 0L, 0L );


	while(GetMessage(&msg, NULL, 0, 0))
	{
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);
		IsDialogMessage(hWnd, &msg); // intended for modeless dialog boxes!!!   
	}
	return msg.wParam;
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



// DoPropertySheet - creates a property sheet that contains two pages.
// hwndOwner - handle of the owner window of the property sheet
VOID DoPropertySheet(HWND hwndOwner)
{
    PROPSHEETPAGE psp[2];
    PROPSHEETHEADER psh;

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[0].hInstance = g_hInst;
    psp[0].pszTemplate = MAKEINTRESOURCE(IDD_PROPPAGE1);
    psp[0].pszIcon = NULL; //MAKEINTRESOURCE(IDI_FONT);
    psp[0].pfnDlgProc = Prop1DialogProc;
    psp[0].pszTitle = "Resources"; //MAKEINTRESOURCE(IDS_FONT)
    psp[0].lParam = 0;
    psp[0].pfnCallback = NULL;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[1].hInstance = g_hInst;
    psp[1].pszTemplate = MAKEINTRESOURCE(IDD_PROPPAGE2);
    psp[1].pszIcon = NULL; //MAKEINTRESOURCE(IDI_BORDER);
    psp[1].pfnDlgProc = Prop2DialogProc;
    psp[1].pszTitle = "Channels"; //MAKEINTRESOURCE(IDS_BORDER);
    psp[1].lParam = 0;
    psp[1].pfnCallback = NULL;

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE;
    psh.hwndParent =  hwndOwner;
    psh.hInstance = g_hInst;
    psh.pszIcon = NULL; //MAKEINTRESOURCE(IDI_CELL_PROPERTIES);
    psh.pszCaption = NULL; //(LPSTR) "Cell Properties";
    psh.nPages = sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = 0;
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;
    psh.pfnCallback = NULL;

    PropertySheet(&psh);
    return;
}



BOOL CALLBACK Prop1DialogProc()// HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{/*
	switch (uMsg) {
		case WM_INITDIALOG:
			// center dialog over parent
			CenterDialog(hWnd);
			break;
	}
		
	if( WM_COMMAND == uMsg )
		if( IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam) )
			EndDialog( hWnd, TRUE );
	
	return WM_INITDIALOG == uMsg ? TRUE : FALSE;*/
	return true;
}


BOOL CALLBACK Prop2DialogProc()// HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{/*
	switch (uMsg) {
		case WM_INITDIALOG:
			// center dialog over parent
			CenterDialog(hWnd);
			break;
	}
		
	if( WM_COMMAND == uMsg )
		if( IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam) )
			EndDialog( hWnd, TRUE );
	
	return WM_INITDIALOG == uMsg ? TRUE : FALSE;*/
	return true;
}