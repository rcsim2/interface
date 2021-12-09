// the raw Win32 API
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"


// globals (are good for you)
char g_szClassName[] = "MyWindowClass";
HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;

HICON hIcon = NULL;;
HMENU hMenu = NULL;

DLGTEMPLATE* pTabPage[5];
HWND g_hTabControl = NULL;
HWND g_hTabCurrent = NULL;

bool g_bHorizontal = false;
bool g_bStop = false;

BOOL g_bReady = TRUE;
BOOL g_bActive = TRUE; 


// VxD variables
#define VMYXD_APIFUNC_1 1
#define VMYXD_APIFUNC_2 2

HANDLE  hVxD = 0;
DWORD   cbBytesReturned;
DWORD   dwErrorCode;
//DWORD   RetInfo[13];	// 1 Sync Pulse + 10 Channels + 1 IntCount + 1 PulseLength

// ints rule!!!!!
// Note: better use int because DWORD is unsigned!!!!!!!!!!!
int		RetInfo[88]; 	// big sucker to test
//int	PrevRetInfo[88]; 

int		IntCount = 0;
int		ChannelTotal = 0;

CHAR*   strVxDFileName = "Vmyxd.vxd";
CHAR    lpBuffer[MAX_PATH];
CHAR    strVxDFilePath[MAX_PATH];


#define COLOR_BLUE 	RGB(0,0,255)
#define COLOR_BLACK	RGB(0,0,0)
#define COLOR_RED	RGB(255,0,0)
#define COLOR_WHITE	RGB(255,255,255)

COLORREF g_crColor = RGB(255,255,255);

COLORREF COLOUR_TEXT = RGB(255,255,255);
COLORREF COLOUR_BAR	 = RGB(0,0,255);
COLORREF COLOR_CONNECTED = COLOR_BLUE;

COLORREF g_crColorBar[11] = {
	COLOR_BLUE,
	COLOR_BLUE,COLOR_BLUE,COLOR_BLUE,COLOR_BLUE,COLOR_BLUE,
	COLOR_BLUE,COLOR_BLUE,COLOR_BLUE,COLOR_BLUE,COLOR_BLUE,
};
 


#define STRICT

#ifdef STRICT
WNDPROC wpOrigProgressBar1Proc;
WNDPROC wpOrigProgressBar2Proc;
WNDPROC wpOrigProgressBar3Proc;
WNDPROC wpOrigProgressBar4Proc;
WNDPROC wpOrigProgressBar5Proc;
WNDPROC wpOrigProgressBar6Proc;
WNDPROC wpOrigProgressBar7Proc;
WNDPROC wpOrigProgressBar8Proc;
WNDPROC wpOrigProgressBar9Proc;
WNDPROC wpOrigProgressBar10Proc;
#else   
FARPROC wpOrigProgressBar1Proc;
FARPROC wpOrigProgressBar2Proc;
FARPROC wpOrigProgressBar3Proc;
FARPROC wpOrigProgressBar4Proc;
FARPROC wpOrigProgressBar5Proc;
FARPROC wpOrigProgressBar6Proc;
FARPROC wpOrigProgressBar7Proc;
FARPROC wpOrigProgressBar8Proc;
FARPROC wpOrigProgressBar9Proc;
FARPROC wpOrigProgressBar10Proc;
#endif


HHOOK hHook = NULL;


bool g_hChMute[11] = {
	false,
	false,false,false,false,false,
	false,false,false,false,false,
};







// prototypes
LRESULT CALLBACK DlgWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM );

void CenterDialog(HWND hdlg);

void CreateTabControl(HWND hWnd);
void TabChange(HWND hWnd);
void TabCenter(HWND hWnd);

BOOL CALLBACK Tab1DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab2DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab3DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab4DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Tab5DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void GameLoop();

//void DrawBar(HWND hwnd,int percent);
//void DrawBarEx(HWND hwnd,int percent, bool horz);
void DrawBarEx(HWND hwnd, int percent, bool horz, COLORREF crColorBar, COLORREF crColorText);


VOID APIENTRY DisplayContextMenu(HWND hwnd, POINT pt);



LRESULT APIENTRY ProgressBar1SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar2SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar3SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar4SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar5SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar6SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar7SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar8SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar9SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY ProgressBar10SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



LRESULT CALLBACK KeyboardProc( int code, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK MessageProc( int code, WPARAM wParam, LPARAM lParam );
  








// Entry Point: WinMain() 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	WNDCLASSEX WndClass;
	HWND hWnd;
	MSG msg;



	g_hInst = hInstance;

	WndClass.cbSize        = sizeof(WNDCLASSEX);
	WndClass.style         = 0;
	WndClass.lpfnWndProc   = DlgWndProc;
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
								(DLGPROC)DlgWndProc );
	
	// always handy
	g_hWnd = hWnd;

	
	// Load VxD
	if ( !GetSystemDirectory(lpBuffer, MAX_PATH) ) {
		dwErrorCode = GetLastError();
		printf("Could not get Sys Dir, Error code: %ld\n", dwErrorCode);    
	} else {
		strcpy(strVxDFilePath, "\\\\.\\");
		strcat(strVxDFilePath, lpBuffer);
		strcat(strVxDFilePath, "\\");
		strcat(strVxDFilePath, strVxDFileName);         
	}   

	// Dynamically load and prepare to call VMYXD
	// The CREATE_NEW flag is not necessary
	// Note: if we have added version info into the vxd with adrc2vxd we will only see
	// that info in System Information if we load the vxd from the Windows\System dir
	//hVxD = CreateFile("\\\\.\\C:\\Windows\\System\\Vmyxd.vxd", 0,0,0,
	//                    CREATE_NEW, FILE_FLAG_DELETE_ON_CLOSE, 0);
	
	hVxD = CreateFile(strVxDFilePath, 0, 0, NULL, 0, FILE_FLAG_DELETE_ON_CLOSE, NULL);

	
	// Note: this is only possible if the vxd is registered in the Registry
	//hVxD = CreateFile("\\\\.\\VMYXD", 0,0,0,
	//                    CREATE_NEW, FILE_FLAG_DELETE_ON_CLOSE, 0);

	if ( hVxD == INVALID_HANDLE_VALUE )
	{
		dwErrorCode = GetLastError();
		if ( dwErrorCode == ERROR_NOT_SUPPORTED )
		{
			printf("VxD loaded but device does not support DeviceIOCTL, Error code: %ld\n", dwErrorCode);
			
			//printf("\nHit any key to quit\n");
			//while ( !_kbhit() );
		}
		if ( dwErrorCode == ERROR_FILE_NOT_FOUND )
		{
			printf("Could not find VxD or it is not dynamically loadable, Error code: %ld\n", dwErrorCode);
			
			//printf("\nHit any key to quit\n");
			//while ( !_kbhit() );
		}
	}
	else
	{
		printf("VxD loaded and DeviceIOCTL ready to go!\n\n");
	}

/*
	// Standard message pump for dialog based apps
	while(GetMessage(&msg, NULL, 0, 0))
	{
		// like this you dork, because we must still catch other messages when it is no dialog message
		if ( !IsDialogMessage(hWnd, &msg) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
*/


	// Special message pump
	BOOL bGotMsg; 
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message  )
	{
		// Use PeekMessage() if the app is active, so we can use idle time to
		// render the scene. Else, use GetMessage() to avoid eating CPU time.
		if( g_bActive )
			bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );
		else
			bGotMsg = GetMessage( &msg, NULL, 0U, 0U );
		
		if( bGotMsg )
		{      
			if ( !IsDialogMessage(hWnd, &msg) ) {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else 
		{
			// Render a frame during idle time (no messages are waiting)
			if( g_bActive && g_bReady )
				//Render3DEnvironment();
				GameLoop();
		}
	}

	

	return msg.wParam;
}


LRESULT CALLBACK DlgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// loop-the-loop
	//GameLoop(hWnd);

	switch (uMsg)
	{
		case WM_INITDIALOG:
			hIcon = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1) );
			SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon); // ICON_SMALL geeft geen icon entry in Alt-tab dialog

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

			// Set focus to OK button
			// Note: Kan niet met SetFocus()
			SendMessage(hWnd,WM_NEXTDLGCTL,0,FALSE);

			// TODO: dotted line focus on first tab so user kan tab
			// to next tab right away: see GetRight Config
			// DONE: by setting Tab Order of Tab Control to 1 we can now
			// use arrows to switch tabs. Still have no focus on tab at start-up
			// though...
	

			return TRUE;
			break;
		case WM_NOTIFY:
			// You get this if the user want´s to change the tab page
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
		case WM_KEYDOWN: // fuck, why not???
			  // because catching key input in dialogs takes special steps
			  // see: the froggie apps
			  //if (VK_HOME == wParam)
			  MessageBox(NULL,"qqq!","QQQ!",MB_OK);

			  return 0;
		break;
		/*case WM_PAINT:
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
			return TRUE;
			break;*/
		default:
			return FALSE;
	}
	return FALSE;
}


BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{
	switch (uMsg) {
		case WM_INITDIALOG:
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
	if((hwnd = GetParent(hdlg)) == NULL || IsIconic(hwnd)) {
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

	// insert the tabs
	SendMessage( g_hTabControl,TCM_INSERTITEM,0,(LPARAM)&tcItem[0] );
	SendMessage( g_hTabControl,TCM_INSERTITEM,1,(LPARAM)&tcItem[1] );
	SendMessage( g_hTabControl,TCM_INSERTITEM,2,(LPARAM)&tcItem[2] );
	//SendMessage( g_hTabControl,TCM_INSERTITEM,3,(LPARAM)&tcItem[3] );

	
	// Dit is niet nodig omdat we CreateDialog() gebruiken
	// Load the dialog pages for the tab control
	// Note: de dialog pages hebben: Styles->Border->None
	// Don't use property pages since they have More Styles->Disabled
	// Let op: pTabPage[0] = Dialog 1
	//pTabPage[0] = (DLGTEMPLATE*)LockResource( LoadResource(g_hInst, FindResource(NULL,MAKEINTRESOURCE(IDD_DIALOG1),RT_DIALOG)) );
    //pTabPage[1] = (DLGTEMPLATE*)LockResource( LoadResource(g_hInst, FindResource(NULL,MAKEINTRESOURCE(IDD_DIALOG2),RT_DIALOG)) );
    //pTabPage[2] = (DLGTEMPLATE*)LockResource( LoadResource(g_hInst, FindResource(NULL,MAKEINTRESOURCE(IDD_DIALOG3),RT_DIALOG)) );
    //pTabPage[3] = (DLGTEMPLATE*)LockResource( LoadResource(g_hInst, FindResource(NULL,MAKEINTRESOURCE(IDD_DIALOG4),RT_DIALOG)) );

}



void TabChange(HWND hWnd)
{ 
    static int  NewPage;


	
    if (g_hTabCurrent != NULL)
		DestroyWindow(g_hTabCurrent); 
 
    NewPage = TabCtrl_GetCurSel(g_hTabControl); 
 
    // Let op: NewPage 0 is Dialog 1
	//if(NewPage == 0) g_hTabCurrent = CreateDialogIndirect(g_hInst,pTabPage[NewPage],g_hTabControl,(DLGPROC)Tab1DlgProc); 
    //if(NewPage == 1) g_hTabCurrent = CreateDialogIndirect(g_hInst,pTabPage[NewPage],g_hTabControl,(DLGPROC)Tab2DlgProc); 
    //if(NewPage == 2) g_hTabCurrent = CreateDialogIndirect(g_hInst,pTabPage[NewPage],g_hTabControl,(DLGPROC)Tab3DlgProc); 
    //if(NewPage == 3) g_hTabCurrent = CreateDialogIndirect(g_hInst,pTabPage[NewPage],g_hTabControl,(DLGPROC)Tab4DlgProc);

	// Dit is veel beter: geen arrays en pointers!!!!!!!!!
	// Note: make main dialog (hWnd) owner of tab dialogs
	if(NewPage == 0) g_hTabCurrent = CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG1),hWnd,(DLGPROC)Tab1DlgProc); 
	if(NewPage == 1)
		if (g_bHorizontal)
			g_hTabCurrent = CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG5),hWnd,(DLGPROC)Tab5DlgProc);
		else
			g_hTabCurrent = CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG2),hWnd,(DLGPROC)Tab2DlgProc);
	if(NewPage == 2) g_hTabCurrent = CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG3),hWnd,(DLGPROC)Tab3DlgProc); 
	//if(NewPage == 3) g_hTabCurrent = CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG4),g_hTabControl,(DLGPROC)Tab4DlgProc); 
 

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
	WinRect.top  = (TabRect.top  - WinRect.top) - GetSystemMetrics(SM_CYCAPTION);
 
	TabCtrl_AdjustRect(g_hTabControl, FALSE, &WinRect); 
 
	SetWindowPos(hWnd, HWND_TOP, WinRect.left, WinRect.top, 0, 0, SWP_NOSIZE);
}




// This is the dialog procedure of tab page 1
BOOL CALLBACK Tab1DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  HBRUSH hBrushDlg;
  

  switch(uMsg)
  {
      case WM_INITDIALOG:
          TabCenter(hWnd);
		  SetFocus(hWnd);
		  
		  if (!g_bStop) Button_SetText( GetDlgItem(hWnd,IDC_BUTTON1), "&Stop" );
		  else Button_SetText( GetDlgItem(hWnd,IDC_BUTTON1), "&Resume" );

		  // we need a hook to get key messages in dialog
		  //hHook = SetWindowsHookEx( WH_KEYBOARD, (HOOKPROC)KeyboardProc, NULL, GetCurrentThreadId() );
		  //hHook = SetWindowsHookEx( WH_MSGFILTER, (HOOKPROC)MessageProc, NULL, GetCurrentThreadId() );

		  break;
      case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDOK:

					break;
				case IDCANCEL:

					break;
				case IDC_BUTTON1:
					g_bStop = !g_bStop;
					if (!g_bStop) Button_SetText( GetDlgItem(hWnd,IDC_BUTTON1), "&Stop" );
					else Button_SetText( GetDlgItem(hWnd,IDC_BUTTON1), "&Resume" );
					break;
				case IDC_BUTTON2:
					ListBox_ResetContent( GetDlgItem(hWnd,IDC_LIST1) );
					break;
			}
			break;
      /*case WM_RBUTTONDOWN:
			  MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			  return 0;
		  break;*/
	  case WM_KEYDOWN: // fuck, why not???
		  // because catching key input in dialogs takes special steps
		  // see: the froggie apps
          //if (VK_HOME == wParam)
			  MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			  return 0;
		  break;
	  case WM_CHAR:
		  MessageBox(NULL,"qqq!","QQQ!",MB_OK);
		  break;
	  case WM_CTLCOLORSTATIC:
		  if ((HWND)lParam == GetDlgItem(hWnd,IDC_STATICSTATUS))
			  SetTextColor( (HDC)wParam, g_crColor );

		  SetBkColor( (HDC)wParam, GetSysColor(COLOR_MENU) ); // zet alleen background van characters niet van entire window
		  hBrushDlg = GetStockObject(NULL_BRUSH) ;
          return (UINT)hBrushDlg ;
		  break;
	  //case WM_PAINT:
			//PAINTSTRUCT ps;
			//BeginPaint(hWnd, &ps);
			
			//EndPaint(hWnd, &ps);
			//return 0;
	      //break;
	  case WM_DESTROY:
		  UnhookWindowsHookEx(hHook);
		  break;
 
      default:
          return(FALSE);
  }
 
  return(0);
}



// This is the dialog procedure of tab page 2
BOOL CALLBACK Tab2DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HICON hIcon;
	//POINT pt;
	int i;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;


	
	switch(uMsg)
	{
	case WM_INITDIALOG:
        TabCenter(hWnd);
		//MessageBox(NULL,"QQQ","xxx",MB_OK);
		SetFocus(hWnd);

		hIcon = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON2) );
		SendMessage( GetDlgItem(hWnd, IDC_BUTTONCOLOR), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon );
		
	    
		// This is very nice but we better set the colors via a customized color dialog box
		// (or maybe we should just drop the gratuitous idea of individual bar colors)
		// Or should we...
		// Subclass the progress bar control
		// This allows is to catch mouse clicks in the progress bar window
		// We can then initiate a color dialog for setting the bar's color
		wpOrigProgressBar1Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT1), GWL_WNDPROC, (LONG)ProgressBar1SubclassProc );
		wpOrigProgressBar2Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT2), GWL_WNDPROC, (LONG)ProgressBar2SubclassProc );
		wpOrigProgressBar3Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT3), GWL_WNDPROC, (LONG)ProgressBar3SubclassProc );
		wpOrigProgressBar4Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT4), GWL_WNDPROC, (LONG)ProgressBar4SubclassProc );
		wpOrigProgressBar5Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT5), GWL_WNDPROC, (LONG)ProgressBar5SubclassProc );
		wpOrigProgressBar6Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT6), GWL_WNDPROC, (LONG)ProgressBar6SubclassProc );
		wpOrigProgressBar7Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT7), GWL_WNDPROC, (LONG)ProgressBar7SubclassProc );
		wpOrigProgressBar8Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT8), GWL_WNDPROC, (LONG)ProgressBar8SubclassProc );
		wpOrigProgressBar9Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT9), GWL_WNDPROC, (LONG)ProgressBar9SubclassProc );
		wpOrigProgressBar10Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT10), GWL_WNDPROC, (LONG)ProgressBar10SubclassProc );
		

		
		// Huh?! This is enough to make *all* progress bars respond to double-clicks
		// dat komt natuurlijk omdat je de class style van progress bars verandert
		// make the fucker respond to double-clicks
		SetClassLong( GetDlgItem(hWnd, IDC_PROGRESSVERT1), GCL_STYLE, (LONG)CS_DBLCLKS );


		// set mute buttons according to channel active state
		for (i=1; i<=10; i++) {
			if (g_hChMute[i])
				SendMessage( GetDlgItem(hWnd, IDC_CHECK21+(i-1)), BM_SETCHECK, BST_CHECKED, 0 );
			else
				SendMessage( GetDlgItem(hWnd, IDC_CHECK21+(i-1)), BM_SETCHECK, BST_UNCHECKED, 0 );
		}

		 

		break;
	case WM_KEYDOWN: // fuck, why not???
		// because catching key input in dialogs takes special steps
		// see: the froggie apps
        //if (VK_HOME == wParam) {
			  MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			  //return 0;
		//}
		break;
	/*case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// works only with static controls, not progress bars
		if ( ChildWindowFromPoint(hWnd, pt) == GetDlgItem(hWnd, IDC_PROGRESSVERT1) ) {
			MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				//hbrush = CreateSolidBrush(cc.rgbResult);
				//COLOUR_BAR = cc.rgbResult;
				//COLOR_CONNECTED = cc.rgbResult;
			}
		}
		break;*/
	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
			case IDC_BUTTONHORIZONTAL:
				if (g_hTabCurrent != NULL)
					DestroyWindow(g_hTabCurrent);
				g_hTabCurrent = CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG5),g_hWnd,(DLGPROC)Tab5DlgProc); 
				ShowWindow(g_hTabCurrent, SW_SHOW);

				g_bHorizontal = true;
				break;
			case IDC_BUTTONCOLOR:
				//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
				if (ChooseColor(&cc)==TRUE) {
					//hbrush = CreateSolidBrush(cc.rgbResult);
					//COLOUR_BAR = cc.rgbResult;
					for (i=0; i<=11; i++) {
						g_crColorBar[i] = cc.rgbResult;
					}
					COLOR_CONNECTED = cc.rgbResult;
				}
				break;

			case IDC_CHECK21: g_hChMute[1] = !g_hChMute[1]; break;
			case IDC_CHECK22: g_hChMute[2] = !g_hChMute[2]; break;
			case IDC_CHECK23: g_hChMute[3] = !g_hChMute[3]; break;
			case IDC_CHECK24: g_hChMute[4] = !g_hChMute[4]; break;
			case IDC_CHECK25: g_hChMute[5] = !g_hChMute[5]; break;
			case IDC_CHECK26: g_hChMute[6] = !g_hChMute[6]; break;
			case IDC_CHECK27: g_hChMute[7] = !g_hChMute[7]; break;
			case IDC_CHECK28: g_hChMute[8] = !g_hChMute[8]; break;
			case IDC_CHECK29: g_hChMute[9] = !g_hChMute[9]; break;
			case IDC_CHECK30: g_hChMute[10] = !g_hChMute[10]; break;

			case IDC_BUTTONMASTER:
				static bool toggle = true;
				toggle = !toggle;

				for (i=1; i<=10; i++) {
					if (toggle) {
						g_hChMute[i] = false;
						SendMessage( GetDlgItem(hWnd, IDC_CHECK21+(i-1)), BM_SETCHECK, BST_UNCHECKED, 0 );
					} else {
						g_hChMute[i] = true;
						SendMessage( GetDlgItem(hWnd, IDC_CHECK21+(i-1)), BM_SETCHECK, BST_CHECKED, 0 );
					}
				}
				break;
		}
		break;

	default:
        return(FALSE);
	}

	return(0);
}



// This is the dialog procedure of tab page 3
BOOL CALLBACK Tab3DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//int dIndex;
	//PAINTSTRUCT ps;
	int i;


	switch(uMsg)
	{
		case WM_INITDIALOG:
			TabCenter(hWnd);
			//SetFocus(hWnd);

			for (i=0; i<10; i++) {
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Pitch");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Roll");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Nick");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Tail");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Throttle");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Function6");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Function7");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Function8");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Function9");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Function10");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Function11");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Function12");

				//SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_SETCURSEL, 0, 0);
			}

			
			// mc-20 specific
			// should only do this the very first time the app starts
			// after that we should read settings from the Registry
			SendMessage( GetDlgItem(hWnd, IDC_COMBO1), CB_SETCURSEL, 4, 0);
			SendMessage( GetDlgItem(hWnd, IDC_COMBO2), CB_SETCURSEL, 1, 0);
			SendMessage( GetDlgItem(hWnd, IDC_COMBO3), CB_SETCURSEL, 2, 0);
			SendMessage( GetDlgItem(hWnd, IDC_COMBO4), CB_SETCURSEL, 3, 0);
			SendMessage( GetDlgItem(hWnd, IDC_COMBO6), CB_SETCURSEL, 0, 0);
	
			
/*			
			dIndex = ComboBox_AddString( GetDlgItem(hWnd, IDC_COMBO1), "Pitch" );
			//ComboBox_SetTopIndex( GetDlgItem(hWnd, IDC_COMBO1), dIndex );
			//SendMessage( GetDlgItem(hWnd, IDC_COMBO1), CB_SETTOPINDEX, dIndex, 0);
			//SendMessage( GetDlgItem(hWnd, IDC_COMBO1), CB_SETITEMDATA, dIndex, ID_PITCH);

			dIndex = ComboBox_AddString( GetDlgItem(hWnd, IDC_COMBO1), "Roll");
			dIndex = ComboBox_AddString( GetDlgItem(hWnd, IDC_COMBO1), "Nick");
			dIndex = ComboBox_AddString( GetDlgItem(hWnd, IDC_COMBO1), "Tail");
			dIndex = ComboBox_AddString( GetDlgItem(hWnd, IDC_COMBO1), "Throttle");
*/
	
		return TRUE;
		break;
		/*case WM_LBUTTONDOWN:
			MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			break;*/
		case WM_COMMAND:			
			switch( HIWORD(wParam) )
			{
				case CBN_SELCHANGE:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					break;
				case CBN_SETFOCUS:
					//SetCapture( GetDlgItem(hWnd, IDC_COMBO1) );
					break;
				//case CBN_CLOSEUP:
				//	SetCapture( hWnd );
				//	break;
			}
		break;
		/*case WM_PAINT:
			BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
			break;*/
		default:
			return(FALSE);
	}

	return(0);
}




// This is the dialog procedure of tab page 4
BOOL CALLBACK Tab4DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
      case WM_INITDIALOG:
          TabCenter(hWnd);
		  break;
 
      default:
          return(FALSE);
  }
 
  return(0);
}


// This is the dialog procedure of tab page 5
BOOL CALLBACK Tab5DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HICON hIcon;
	int i;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



	switch(uMsg)
	{
		case WM_INITDIALOG:
			TabCenter(hWnd);
			SetFocus(hWnd);

			// better not do this
			/*
			if (ChannelTotal != 0) {
				// disable all windows higher than ChannelTotal
				// we'll only do this if Signal Connected
				// gotta make sure controls have consecutive IDs
				for (int i=10; i>ChannelTotal; i--) {
					EnableWindow( GetDlgItem(hWnd, (IDC_PROGRESSHORZ1-1)+i), FALSE);
					EnableWindow( GetDlgItem(hWnd, (IDC_STATICCH1-1)+i), FALSE);
					EnableWindow( GetDlgItem(hWnd, (IDC_STATIC1-1)+i), FALSE);
				}
			}
			*/

			hIcon = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON2) );
			SendMessage( GetDlgItem(hWnd, IDC_BUTTONCOLOR), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon );

			// Subclass the progress bar control
			// This allows is to catch mouse clicks in the progress bar window
			// We can then initiate a color dialog for setting the bar's color
			wpOrigProgressBar1Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ1), GWL_WNDPROC, (LONG)ProgressBar1SubclassProc );
			wpOrigProgressBar2Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ2), GWL_WNDPROC, (LONG)ProgressBar2SubclassProc );
			wpOrigProgressBar3Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ3), GWL_WNDPROC, (LONG)ProgressBar3SubclassProc );
			wpOrigProgressBar4Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ4), GWL_WNDPROC, (LONG)ProgressBar4SubclassProc );
			wpOrigProgressBar5Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ5), GWL_WNDPROC, (LONG)ProgressBar5SubclassProc );
			wpOrigProgressBar6Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ6), GWL_WNDPROC, (LONG)ProgressBar6SubclassProc );
			wpOrigProgressBar7Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ7), GWL_WNDPROC, (LONG)ProgressBar7SubclassProc );
			wpOrigProgressBar8Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ8), GWL_WNDPROC, (LONG)ProgressBar8SubclassProc );
			wpOrigProgressBar9Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ9), GWL_WNDPROC, (LONG)ProgressBar9SubclassProc );
			wpOrigProgressBar10Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSHORZ10), GWL_WNDPROC, (LONG)ProgressBar10SubclassProc );
			
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDC_BUTTONVERTICAL:
					if (g_hTabCurrent != NULL)
						DestroyWindow(g_hTabCurrent);
					g_hTabCurrent = CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG2),g_hWnd,(DLGPROC)Tab2DlgProc); 
					ShowWindow(g_hTabCurrent, SW_SHOW);
					g_bHorizontal = false;
					break;
				case IDC_BUTTONCOLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						//hbrush = CreateSolidBrush(cc.rgbResult);
						//COLOUR_BAR = cc.rgbResult;
						for (i=0; i<=11; i++) {
							g_crColorBar[i] = cc.rgbResult;
						}
						COLOR_CONNECTED = cc.rgbResult;
					}
					break;
			}
		break;
 
		default:
			return(FALSE);
  }
 
  return(0);
}



// loop-the-loop
void GameLoop()
{
	
	// Poll the VxD values
	DeviceIoControl(hVxD, VMYXD_APIFUNC_1,
		(LPVOID)NULL, 0,
		(LPVOID)RetInfo, sizeof(RetInfo),
		&cbBytesReturned, NULL);

	
	// This is the buffer we get from the VxD
	// RetInfo[0]  == SyncPulse

	// RetInfo[1]  == Ch1
	// RetInfo[2]  == Ch2
	// RetInfo[3]  == Ch3
	// RetInfo[4]  == Ch4
	// RetInfo[5]  == Ch5
	// RetInfo[6]  == Ch6
	// RetInfo[7]  == Ch7			
	// RetInfo[8]  == Ch8
	// RetInfo[9]  == Ch9
	// RetInfo[10] == Ch10

	// RetInfo[11] == IntCount
	// RetInfo[12] == PulseLength
	// RetInfo[13] == ChannelCount


	
	// maak 1000-2000 uS
	for (int i=0; i<=10; i++) {
		if (RetInfo[i] != 0) RetInfo[i]-=250; // leave unused channels alone
		// RetInfo[i] must be signed here!!!!!!!!!!!!!!!!!!
		// We've changed it from DWORD to int so it's OK now
		if (RetInfo[i] < 0)  RetInfo[i] =  0; // never print negative values
	}


	char status[128];
	char sync[128];
	char ch1[128];
	char ch2[128];
	char ch3[128];
	char ch4[128];
	char ch5[128];
	char ch6[128];
	char ch7[128];
	char ch8[128];
	char ch9[128];
	char ch10[128];



	bool newint = false;
	// get total number of interrupts
	if ( RetInfo[11] > IntCount ) {
		newint = true;
		IntCount = RetInfo[11];
	}

	// get total number of channels
	if ( RetInfo[13] > ChannelTotal )
					ChannelTotal = RetInfo[13];

	// set unused channels at length 0
	for (i=10; i>ChannelTotal; i--) {
		RetInfo[i] = 0;
		//ChL[i] = L[0];
	}


	// TODO: catch Frame Data Defective: dat is wanneer sync pulse of channels
	// buiten de te verwachten waarden vallen
	//bool datadef = false;
	if (RetInfo[0] < 5000 /*|| RetInfo[0] > 15000*/) ChannelTotal = 0;	// check sync
	for (i=1; i<=ChannelTotal; i++) {
		if (RetInfo[i] < 600 || RetInfo[i] > 2400) {		// check channels
			//datadef = true;
			ChannelTotal = 0;
			//ChL[i] = L[0];
		}
	}
	
	// Als de zender uit wordt gezet zal in
	// de VxD ChannelCount op blijft lopen (omdat geen Sync Pulse meer zal worden
	// gevonden die ChannelCount op 0 zet (in de VxD zelf))
	
	// We kunnen dus veel beter OOK een hoog oplopende ChannelTotal als indicatie
	// beschouwen van defective data.
	// Zeker omdat de sync en channels soms toevallig toch binnen de waardes
	// vallen, terwijl hoog oplopende ChannelTotal duidelijk betekent: fucky data
	if (ChannelTotal > 10)
			ChannelTotal = 0;


	
	// set channels to 0 when they are inactivated by mute button
	for (i=1; i<=10; i++) {
		if (g_hChMute[i]) RetInfo[i] = 0;
	}



	
	// values to be printed
	float fsync = (float)RetInfo[0]/1000 ;
	float fch1  = (float)RetInfo[1]/1000 ;
	float fch2  = (float)RetInfo[2]/1000 ;
	float fch3  = (float)RetInfo[3]/1000 ;
	float fch4  = (float)RetInfo[4]/1000 ;
	float fch5  = (float)RetInfo[5]/1000 ;
	float fch6  = (float)RetInfo[6]/1000 ;
	float fch7  = (float)RetInfo[7]/1000 ;
	float fch8  = (float)RetInfo[8]/1000 ;
	float fch9  = (float)RetInfo[9]/1000 ;
	float fch10 = (float)RetInfo[10]/1000;

	// klein beetje vertragen anders loopt GameLoop() sneller dan er interrupts
	// worden ontvangen en zouden we No Signal printen
	// Dit werkt ook prima tegen text flicker in de static controls: te snel updaten
	// is niet goed
	// Probleem is dat we afhankelijk van de CPU snelheid kunnen zijn...
	Sleep(20);
	

    // red status when no/defective signal
	if (!newint) {
		//SetTextColor( GetDC(GetDlgItem(g_hTabCurrent,IDC_STATICSTATUS)), RGB(255,0,0) );
		g_crColor = COLOR_RED;
		sprintf( status, "No Signal                  ");
	} else {
		if (ChannelTotal == 0) {
			//SetTextColor( GetDC(GetDlgItem(g_hTabCurrent,IDC_STATICSTATUS)), COLOUR_RED);
			g_crColor = COLOR_RED;
			sprintf( status, "Signal Defective       ");
		} else {
			//SetTextColor( GetDC(GetDlgItem(g_hTabCurrent,IDC_STATICSTATUS)), COLOUR_BLACK);
			g_crColor = COLOR_CONNECTED;//COLOR_BLUE;
			sprintf( status, "Signal Connected       ");
		}
	}
	
	

/*
	if ( GetKeyState('A') & 0x80 ) {
		g_bColor = !g_bColor;
		InvalidateRect( GetDlgItem(g_hTabCurrent,IDC_STATICSTATUS), NULL, TRUE );
	}
*/



	//SendMessage( GetDlgItem(g_hTabCurrent,IDC_STATICSTATUS),
	//	WM_CTLCOLORSTATIC, (unsigned int)GetDC(GetDlgItem(g_hTabCurrent,IDC_STATICSTATUS)),
	//	GetDlgItem(g_hTabCurrent,IDC_STATICSTATUS) );






	// format
	sprintf( sync,  "%.3f ms          ", fsync );
	sprintf( ch1,  "%.3f ms      ", fch1 );
	sprintf( ch2,  "%.3f ms      ", fch2 );
	sprintf( ch3,  "%.3f ms      ", fch3 );
	sprintf( ch4,  "%.3f ms      ", fch4 );
	sprintf( ch5,  "%.3f ms      ", fch5 );
	sprintf( ch6,  "%.3f ms      ", fch6 );
	sprintf( ch7,  "%.3f ms      ", fch7 );
	sprintf( ch8,  "%.3f ms      ", fch8 );
	sprintf( ch9,  "%.3f ms      ", fch9 );
	sprintf( ch10, "%.3f ms      ", fch10);

	
	
	// set the text
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC1,  ch1 );
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC2,  ch2 );
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC3,  ch3 );
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC4,  ch4 );
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC5,  ch5 );
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC6,  ch6 );
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC7,  ch7 );
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC8,  ch8 );
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC9,  ch9 );
	SetDlgItemText(g_hTabCurrent ,IDC_STATIC10, ch10);


	SetDlgItemText(g_hTabCurrent ,IDC_STATICSTATUS, status );
	SetDlgItemText(g_hTabCurrent ,IDC_STATICSYNC, sync );

	// ints kun je meteen printen	
	SetDlgItemInt(g_hTabCurrent ,IDC_STATICINTCOUNT, IntCount, TRUE);	
	SetDlgItemInt(g_hTabCurrent ,IDC_STATICCHANCOUNT, ChannelTotal, TRUE);

	
	// check if we should draw horizontal of vertical bars
	if (g_bHorizontal) {
		// draw horizontal bars (1000-2000 uS becomes 0-100 percent)
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), int((RetInfo[1]-1000)/10), true, g_crColorBar[1], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ2), int((RetInfo[2]-1000)/10), true, g_crColorBar[2], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ3), int((RetInfo[3]-1000)/10), true, g_crColorBar[3], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ4), int((RetInfo[4]-1000)/10), true, g_crColorBar[4], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ5), int((RetInfo[5]-1000)/10), true, g_crColorBar[5], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ6), int((RetInfo[6]-1000)/10), true, g_crColorBar[6], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ7), int((RetInfo[7]-1000)/10), true, g_crColorBar[7], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ8), int((RetInfo[8]-1000)/10), true, g_crColorBar[8], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ9), int((RetInfo[9]-1000)/10), true, g_crColorBar[9], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ10), int((RetInfo[10]-1000)/10), true, g_crColorBar[10], COLOR_WHITE );
	} else {
		// draw vertical bars
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT1), int((RetInfo[1]-1000)/10), false, g_crColorBar[1], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT2), int((RetInfo[2]-1000)/10), false, g_crColorBar[2], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT3), int((RetInfo[3]-1000)/10), false, g_crColorBar[3], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT4), int((RetInfo[4]-1000)/10), false, g_crColorBar[4], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT5), int((RetInfo[5]-1000)/10), false, g_crColorBar[5], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT6), int((RetInfo[6]-1000)/10), false, g_crColorBar[6], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT7), int((RetInfo[7]-1000)/10), false, g_crColorBar[7], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT8), int((RetInfo[8]-1000)/10), false, g_crColorBar[8], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT9), int((RetInfo[9]-1000)/10), false, g_crColorBar[9], COLOR_WHITE );
		DrawBarEx( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT10), int((RetInfo[10]-1000)/10), false, g_crColorBar[10], COLOR_WHITE );
	}

/*
	for (i=1; i<=ChannelTotal; i++) {
		if (g_bHorizontal)
			DrawBarEx( GetDlgItem(g_hTabCurrent,(IDC_PROGRESSHORZ1-1)+i), int((RetInfo[i]-1000)/10), true );
		else
			DrawBarEx( GetDlgItem(g_hTabCurrent,(IDC_PROGRESSVERT1-1)+i), int((RetInfo[i]-1000)/10), false );
	}
*/
	
/*	
	// test with keys
	if ( GetKeyState('A') & 0x80 ) {
		//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
		++nPos;

		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT1), PBM_SETRANGE, 0, MAKELPARAM(0, 100) );
		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT1), PBM_SETSTEP, 1, 0 );
		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT1), PBM_SETPOS, nPos, 0 );

		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), PBM_SETRANGE, 0, MAKELPARAM(0, 100) );
		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), PBM_SETSTEP, 1, 0 );
		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), PBM_SETPOS, nPos, 0 );

		DrawBar( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), nPos );

	}
	if ( GetKeyState('S') & 0x80 ) {
		--nPos;

		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT1), PBM_SETRANGE, 0, MAKELPARAM(0, 100) );
		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT1), PBM_SETSTEP, 1, 0 );
		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSVERT1), PBM_SETPOS, nPos, 0 );

		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), PBM_SETRANGE, 0, MAKELPARAM(0, 100) );
		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), PBM_SETSTEP, 10, 0 );
		//SendMessage( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), PBM_SETPOS, nPos, 0 );

		DrawBar( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), nPos );

	}
*/



	// fill the pulse length listbox
	// we krijgen hier synchronisatie problemen tussen GameLoop() en de VxD
	// daarom kunnen we beter gewoon de sync/channel lengths printen
	if (!g_bStop) {
		int dIndex;


		// new format
		sprintf( sync,  "%6.3f ms     ", fsync );
		sprintf( ch1,  "%7.3f ms      ", fch1 );
		sprintf( ch2,  "%7.3f ms      ", fch2 );
		sprintf( ch3,  "%7.3f ms      ", fch3 );
		sprintf( ch4,  "%7.3f ms      ", fch4 );
		sprintf( ch5,  "%7.3f ms      ", fch5 );
		sprintf( ch6,  "%7.3f ms      ", fch6 );
		sprintf( ch7,  "%7.3f ms      ", fch7 );
		sprintf( ch8,  "%7.3f ms      ", fch8 );
		sprintf( ch9,  "%7.3f ms      ", fch9 );
		sprintf( ch10, "%7.3f ms      ", fch10);

		
		char* entry[11];
		entry[0] = sync;
		entry[1] = ch1;
		entry[2] = ch2; 
		entry[3] = ch3; 
		entry[4] = ch4; 
		entry[5] = ch5; 
		entry[6] = ch6; 
		entry[7] = ch7; 
		entry[8] = ch8; 
		entry[9] = ch9; 
		entry[10]= ch10;


#define MAXHISTORY	4096

		// list box can contain max 2^16 items so clear the box from time to time
		if(ListBox_GetCount( GetDlgItem(g_hTabCurrent,IDC_LIST1) ) == MAXHISTORY)
			ListBox_ResetContent( GetDlgItem(g_hTabCurrent,IDC_LIST1) );
		
		// add entries (one per game loop)
		static int i = -1;
		++i;

		// print sync/channel lengths if Signal Connected
		if (ChannelTotal != 0){
			if (i<=ChannelTotal) {
				dIndex = ListBox_AddString( GetDlgItem(g_hTabCurrent,IDC_LIST1), entry[i] );
				ListBox_SetTopIndex( GetDlgItem(g_hTabCurrent,IDC_LIST1),dIndex );
			} else {
				i = -1;
			}
		} else {
			if (newint) {
				// directly print the pulse length if Signal Defective
				char pulselength[128];
				float fpulselength = (float)RetInfo[12]/1000;
				sprintf( pulselength,  "%7.3f ms", fpulselength );

				dIndex = ListBox_AddString( GetDlgItem(g_hTabCurrent,IDC_LIST1), pulselength );
				ListBox_SetTopIndex( GetDlgItem(g_hTabCurrent,IDC_LIST1),dIndex );
			} else {
				 // print nothing if No Signal
			}
		}


		//ListBox_SetItemData( GetDlgItem(g_hTabCurrent,IDC_LIST1),dIndex,1 );
		//ListBox_SetTopIndex( GetDlgItem(g_hTabCurrent,IDC_LIST1),dIndex );
	}

}




/* 
void DrawBar(HWND hwnd,int percent)
{
#define COLOUR_TEXT		RGB(255,255,255)
#define COLOUR_BAR		RGB(0,0,255)

    RECT rect;
    GetClientRect(hwnd,&rect);
 
    int xdivider=MulDiv(percent,rect.right,100);
 
    RECT rectfull,rectempty;
    SetRect(&rectfull,0,0,xdivider,rect.bottom);
    SetRect(&rectempty,xdivider,0,rect.right,rect.bottom);
 
    char buffer[20];
    //wsprintf(buffer,"%d %%",percent);
	wsprintf(buffer,"",percent);
    int x=int(rect.right/2);
    int y=int(rect.bottom*0.9);
 
    HDC hdc=GetDC(hwnd);
 
    SetTextAlign(hdc,TA_CENTER | TA_BASELINE);
 
    // full rectangle
    SetTextColor(hdc,COLOUR_TEXT);
    SetBkColor  (hdc,COLOUR_BAR);
    ExtTextOut  (hdc,x,y,ETO_CLIPPED | ETO_OPAQUE,
            &rectfull, buffer,strlen(buffer),NULL);
 
    // empty rectangle
    SetTextColor(hdc,COLOUR_BAR);
    SetBkColor  (hdc,COLOUR_TEXT);
    ExtTextOut  (hdc,x,y,ETO_CLIPPED | ETO_OPAQUE,
            &rectempty,buffer,strlen(buffer),NULL);
 
    ReleaseDC(hwnd,hdc);
}
*/


void DrawBarEx(HWND hwnd, int percent, bool horz, COLORREF crColorBar, COLORREF crColorText)
{
//#define COLOUR_TEXT		RGB(255,255,255)
//#define COLOUR_BAR		RGB(0,0,255)

    RECT rect;
    GetClientRect(hwnd,&rect);
	int divider;

	// omit this and big bars might be drawn anywhere...
	if (hwnd == NULL)
		return;

	if (horz) {
		divider=MulDiv(percent,rect.right,100);
	} else {
		divider=MulDiv(percent,rect.bottom,100);
		divider=rect.bottom-divider;
	}
 
    RECT rectfull,rectempty;
	if (horz) {
		SetRect(&rectfull,0,0,divider,rect.bottom);
		SetRect(&rectempty,divider,0,rect.right,rect.bottom);
	} else {
		SetRect(&rectempty,0,0,rect.right,divider);
		SetRect(&rectfull,0,divider,rect.right,rect.bottom);
	}
 
    char buffer[20];
	int x;
	int y;

	if (horz) {
		//wsprintf(buffer,"%d %%",percent);
		wsprintf(buffer,"",percent);
		x=int(rect.right/2);
		y=int(rect.bottom*0.9);
	} else {
		//wsprintf(buffer,"%d %%",percent);
		wsprintf(buffer,"",percent);
		x=int(rect.right/2);
		y=int(rect.bottom/2);
	}
 
    HDC hdc=GetDC(hwnd);
 
    SetTextAlign(hdc,TA_CENTER | TA_BASELINE);
 
    // full rectangle
    SetTextColor(hdc,crColorText);
    SetBkColor  (hdc,crColorBar);
    ExtTextOut  (hdc,x,y,ETO_CLIPPED | ETO_OPAQUE,
            &rectfull, buffer,strlen(buffer),NULL);
 
    // empty rectangle
    SetTextColor(hdc,crColorBar);
    SetBkColor  (hdc,crColorText);
    ExtTextOut  (hdc,x,y,ETO_CLIPPED | ETO_OPAQUE,
            &rectempty,buffer,strlen(buffer),NULL);
 
    ReleaseDC(hwnd,hdc);
}




VOID APIENTRY DisplayContextMenu(HWND hwnd, POINT pt) 
{ 
    HMENU hmenu;            // top-level menu 
    HMENU hmenuTrackPopup;  // shortcut menu 
 
    // Load the menu resource. 
     if ((hmenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1))) == NULL) 
        return; 
 
    // TrackPopupMenu cannot display the menu bar so get 
    // the handle of the first shortcut menu. 
    hmenuTrackPopup = GetSubMenu(hmenu, 0); 
 
    // Display the shortcut menu. Track the right mouse 
    // button. 
    TrackPopupMenu(hmenuTrackPopup, 
            TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
            pt.x, pt.y, 0, hwnd, NULL); 
 
    // Destroy the menu. 
    DestroyMenu(hmenu); 
} 
 


// Haken, jawohl
LRESULT CALLBACK KeyboardProc( int code, WPARAM wParam, LPARAM lParam )
{
	if (code < 0)
		return CallNextHookEx(hHook, code, wParam, lParam) ;

	if (wParam == (WPARAM)VkKeyScan('q')) {
		if (!(lParam & 0x80000000))
			MessageBox(NULL,"qqq!","QQQ!",MB_OK);
	}
	return 0;

}


// Haken, jawohl
LRESULT CALLBACK MessageProc( int code, WPARAM wParam, LPARAM lParam )
{
	if (code < 0)
		return CallNextHookEx(hHook, code, wParam, lParam) ;

	switch ( ((LPMSG)lParam)->message) 
	{
		case WM_KEYDOWN:
			//if (wParam == (WPARAM)VkKeyScan('q')) //???????
				MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			break;

	}

	return 0;
}










// Subclass procedure 1
LRESULT APIENTRY ProgressBar1SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[1] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[1] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 2
LRESULT APIENTRY ProgressBar2SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[2] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[2] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 3
LRESULT APIENTRY ProgressBar3SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[3] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[3] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 4
LRESULT APIENTRY ProgressBar4SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[4] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[4] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 5
LRESULT APIENTRY ProgressBar5SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[5] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[5] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 6
LRESULT APIENTRY ProgressBar6SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[6] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[6] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 7
LRESULT APIENTRY ProgressBar7SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[7] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[7] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 8
LRESULT APIENTRY ProgressBar8SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[8] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[8] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 9
LRESULT APIENTRY ProgressBar9SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[9] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[9] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 10
LRESULT APIENTRY ProgressBar10SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	//HMENU hMenu;
	POINT pt;

	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	//HWND hwnd;                      // owner window
	//HBRUSH hbrush;                  // brush handle
	//static DWORD rgbCurrent;        // initial color selection
	
	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD) acrCustClr;
	cc.rgbResult = RGB(0,0,255); //rgbCurrent;
	cc.Flags = /*CC_FULLOPEN |*/ CC_RGBINIT;



    // not necessary???
	if (uMsg == WM_GETDLGCODE) 
        return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_RBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			// this is for windows menus
			//hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			//SetMenu(hWnd, hMenu);

			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hWnd, &pt);

			DisplayContextMenu(hWnd, pt);
			break;
		case WM_LBUTTONDBLCLK:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (ChooseColor(&cc)==TRUE) {
				g_crColorBar[10] = cc.rgbResult;
			}
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case ID_POPUP_COLOR:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					if (ChooseColor(&cc)==TRUE) {
						g_crColorBar[10] = cc.rgbResult;
					}
					break;
			}
			break;
			
	}

    return CallWindowProc((FARPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}