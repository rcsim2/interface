// the raw Win32 API
#include <windows.h>
#include <commctrl.h>
#include "resource.h"

static char g_szClassName[] = "MyWindowClass";
static HINSTANCE g_hInst = NULL;
HICON hIcon = NULL;;
HMENU hMenu = NULL;


#define C_PAGES 3

typedef struct tag_dlghdr { 
    HWND hwndTab;       // tab control 
    HWND hwndDisplay;   // current child dialog box 
    RECT rcDisplay;     // display rectangle for the tab control 
    DLGTEMPLATE *apRes[C_PAGES]; 
} DLGHDR;  



// prototypes
LRESULT CALLBACK WndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM );

void CenterDialog(HWND hdlg);

VOID WINAPI OnTabbedDialogInit(HWND hwndDlg);
DLGTEMPLATE * WINAPI DoLockDlgRes(LPCSTR lpszResName);
VOID WINAPI OnSelChanged(HWND hwndDlg);
VOID WINAPI OnChildDialogInit(HWND hwndDlg);
BOOL CALLBACK ChildDialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM );






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


			// What we need is a tabbed control which contains our prop pages
			OnTabbedDialogInit( GetDlgItem(hWnd, IDC_TAB1) );

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



VOID WINAPI OnTabbedDialogInit(HWND hwndDlg)
{ 
    DLGHDR *pHdr = (DLGHDR *) LocalAlloc(LPTR, sizeof(DLGHDR)); 
    DWORD dwDlgBase = GetDialogBaseUnits(); 
    int cxMargin = LOWORD(dwDlgBase) / 4; 
    int cyMargin = HIWORD(dwDlgBase) / 8;
	TC_ITEM tie;
	RECT rcTab; 
    HWND hwndButton;
	RECT rcButton;
	int i;

    // Save a pointer to the DLGHDR structure. 
    SetWindowLong(hwndDlg, GWL_USERDATA, (LONG) pHdr);
	
    // Create the tab control.     
	//InitCommonControls(); 

    pHdr->hwndTab = CreateWindow( WC_TABCONTROL, "", 
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, 100, 100, 
        hwndDlg, NULL, g_hInst, NULL );
	
	if (pHdr->hwndTab == NULL) {
        // handle error
	}

    // Add a tab for each of the three child dialog boxes. 
    tie.mask = TCIF_TEXT | TCIF_IMAGE;     tie.iImage = -1; 
    tie.pszText = "First";     TabCtrl_InsertItem(pHdr->hwndTab, 0, &tie); 
    tie.pszText = "Second";    TabCtrl_InsertItem(pHdr->hwndTab, 1, &tie); 
    tie.pszText = "Third";     TabCtrl_InsertItem(pHdr->hwndTab, 2, &tie);  
    
	// Lock the resources for the three child dialog boxes. 
    pHdr->apRes[0] = DoLockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE1)); 
    pHdr->apRes[1] = DoLockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE2)); 
    pHdr->apRes[2] = DoLockDlgRes(MAKEINTRESOURCE(IDD_PROPPAGE3));  
    
	// Determine the bounding rectangle for all child dialog boxes. 
    SetRectEmpty(&rcTab);
	for (i = 0; i < C_PAGES; i++) { 
        if (pHdr->apRes[i]->cx > rcTab.right) 
            rcTab.right = pHdr->apRes[i]->cx; 
        if (pHdr->apRes[i]->cy > rcTab.bottom) 
            rcTab.bottom = pHdr->apRes[i]->cy;     
	} 
    rcTab.right = rcTab.right * LOWORD(dwDlgBase) / 4; 
    rcTab.bottom = rcTab.bottom * HIWORD(dwDlgBase) / 8;  
    
	// Calculate how large to make the tab control, so 
    // the display area can accomodate all the child dialog boxes. 
    TabCtrl_AdjustRect(pHdr->hwndTab, TRUE, &rcTab); 
    OffsetRect(&rcTab, cxMargin - rcTab.left, 
            cyMargin - rcTab.top);
	
	// Calculate the display rectangle. 
    CopyRect(&pHdr->rcDisplay, &rcTab); 
    TabCtrl_AdjustRect(pHdr->hwndTab, FALSE, &pHdr->rcDisplay); 
	
    // Set the size and position of the tab control, buttons, 
    // and dialog box. 
    SetWindowPos(pHdr->hwndTab, NULL, rcTab.left+5, rcTab.top+5, 
            rcTab.right - rcTab.left-150, rcTab.bottom - rcTab.top-200, 
            SWP_NOZORDER);
/*	
	// Move the first button below the tab control. 
    hwndButton = GetDlgItem(hwndDlg, IDOK); 
    SetWindowPos(hwndButton, NULL, 
            rcTab.left, rcTab.bottom + cyMargin, 0, 0, 
            SWP_NOSIZE | SWP_NOZORDER);
	
    // Determine the size of the button. 
    GetWindowRect(hwndButton, &rcButton);     rcButton.right -= rcButton.left; 
    rcButton.bottom -= rcButton.top; 
	
    // Move the second button to the right of the first. 
    hwndButton = GetDlgItem(hwndDlg, IDCANCEL); 
    SetWindowPos(hwndButton, NULL, 
        rcTab.left + rcButton.right + cxMargin, 
        rcTab.bottom + cyMargin, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
*/	
    // Size the dialog box.
	SetWindowPos(hwndDlg, NULL, 0, 0, 
        (rcTab.right + cyMargin +  2 * GetSystemMetrics(SM_CXDLGFRAME))-140, 
        (rcTab.bottom + rcButton.bottom + 2 * cyMargin + 
        2 * GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION))-600,
		SWP_NOMOVE | SWP_NOZORDER);
	
    // Simulate selection of the first item.     
	OnSelChanged(hwndDlg);
}


// DoLockDlgRes - loads and locks a dialog template resource. 
// Returns a pointer to the locked resource. 
// lpszResName - name of the resource  
DLGTEMPLATE * WINAPI DoLockDlgRes(LPCSTR lpszResName)
{ 
    HRSRC hrsrc = FindResource(NULL, lpszResName, RT_DIALOG); 
    HGLOBAL hglb = LoadResource(g_hInst, hrsrc); 
    return (DLGTEMPLATE *) LockResource(hglb);
}


// OnSelChanged - processes the TCN_SELCHANGE notification. 
// hwndDlg - handle of the parent dialog box  
VOID WINAPI OnSelChanged(HWND hwndDlg)
{ 
    DLGHDR *pHdr = (DLGHDR *) GetWindowLong(hwndDlg, GWL_USERDATA); 
    int iSel = TabCtrl_GetCurSel(pHdr->hwndTab);
	
    // Destroy the current child dialog box, if any. 
    if (pHdr->hwndDisplay != NULL)
		DestroyWindow(pHdr->hwndDisplay);
	
    // Create the new child dialog box. 
    pHdr->hwndDisplay = CreateDialogIndirect(g_hInst, 
        pHdr->apRes[iSel], hwndDlg, (DLGPROC)ChildDialogProc);
}


// OnChildDialogInit - Positions the child dialog box to fall 
// within the display area of the tab control.  
VOID WINAPI OnChildDialogInit(HWND hwndDlg) 
{ 
    HWND hwndParent = GetParent(hwndDlg); 
    DLGHDR *pHdr = (DLGHDR *) GetWindowLong(hwndParent, GWL_USERDATA); 
    SetWindowPos(hwndDlg, HWND_TOP, pHdr->rcDisplay.left, 
		pHdr->rcDisplay.top, 0, 0, SWP_NOSIZE);
}


BOOL CALLBACK ChildDialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{
	switch (uMsg) {
		case WM_INITDIALOG:
			OnChildDialogInit(hWnd);
			break;
	}
		
	if( WM_COMMAND == uMsg )
		if( IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam) )
			EndDialog( hWnd, TRUE );
	
	 return WM_INITDIALOG == uMsg ? TRUE : FALSE;
} 
