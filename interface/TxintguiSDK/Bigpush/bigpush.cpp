//-----------------------------------------------------------------------------
// File: Bigpush.cpp
//
// Desc: R/C Sim Transmitter Calibration
//
//
// Copyright (c) 2000 Black Sphere Corp.
//-----------------------------------------------------------------------------
#define STRICT
// NOTE NOTE NOTE: not defining STRICT here causes LNK2001 Errors!!!
//
// From the docs: Enabling STRICT Type Checking 
// Note: If you are writing a C++ application, you don't have the option of applying
// STRICT to only some of your source files. Because of the way C++ "type safe linking"
// works, mixing STRICT and non-STRICT source files in your application can cause 
// linking errors.
//
// So instead of in every source file, it would be better to define STRICT in 
// Project|Settings|C/C++|General|Preprocessor Definitions



#include "bigpush.h"




// globals (are good for you)
char g_szClassName[] = "MyWindowClass";
HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;

HICON hIcon = NULL;
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
 


//#define STRICT


//#ifdef STRICT
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
/*#else   
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
*/


//#ifdef STRICT
WNDPROC wpOrigTrackBar1Proc;
WNDPROC wpOrigTrackBar2Proc;
WNDPROC wpOrigTrackBar3Proc;
WNDPROC wpOrigTrackBar4Proc;
WNDPROC wpOrigTrackBar5Proc;
WNDPROC wpOrigTrackBar6Proc;
WNDPROC wpOrigTrackBar7Proc;
WNDPROC wpOrigTrackBar8Proc;
WNDPROC wpOrigTrackBar9Proc;
WNDPROC wpOrigTrackBar10Proc;
/*#else   
FARPROC wpOrigTrackBar1Proc;
FARPROC wpOrigTrackBar2Proc;
FARPROC wpOrigTrackBar3Proc;
FARPROC wpOrigTrackBar4Proc;
FARPROC wpOrigTrackBar5Proc;
FARPROC wpOrigTrackBar6Proc;
FARPROC wpOrigTrackBar7Proc;
FARPROC wpOrigTrackBar8Proc;
FARPROC wpOrigTrackBar9Proc;
FARPROC wpOrigTrackBar10Proc;
#endif
*/


HHOOK hHook = NULL;


bool g_bChMute[11] = {
	false,
	false,false,false,false,false,
	false,false,false,false,false,
};

// Let's make it a DWORD instead of bool to prevent C4800 warnings
// when reading from registry
// NOTE: using BOOL (i.e. int) instead of bool will prevent C4800 warnings when
// assigning it a DWORD reg value
DWORD g_bChInv[11] = {
	false,
	false,false,false,false,false,
	false,false,false,false,false,
};

bool g_bChExp[11] = {
	false,
	false,false,false,false,false,
	false,false,false,false,false,
};


int g_iChSens[11] = {
	100,
	100,100,100,100,100,
	100,100,100,100,100,
};


bool g_bShowPercent = false;


bool g_bItemFree[12] = {
	true,true,true,true,
	true,true,true,true,
	true,true,true,true,
};


int g_iChMap[11] = {
	-1,
	-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,
};


HINSTANCE g_hinst;	// handle of the application instance 
HWND g_hwndTT;		// handle of the ToolTip control 
HWND g_hwndDlg;		// handle of the dialog box 
HHOOK g_hhk;		// handle of the hook procedure 





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

void LoadVxD();



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
  

LRESULT APIENTRY TrackBar1SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar2SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar3SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar4SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar5SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar6SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar7SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar8SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar9SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT APIENTRY TrackBar10SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 


BOOL DoCreateDialogTooltip(void);
BOOL EnumChildProc(HWND hwndCtrl, LPARAM lParam);
LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
VOID OnWMNotify(LPARAM lParam);


// are we calibrating
bool g_Calibrate = false;


// registry
void RegistryRead();
void RegistryWrite();
void RegistryRead2();
void RegistryWrite2();
void RegistryRead3();
void RegistryWrite3();
void RegistryRead4();
void RegistryWrite4();


// Let's make it a DWORD instead of bool to prevent C4800 warnings
// when reading from registry
// NOTE: using BOOL (i.e. int) instead of bool will prevent C4800 warnings when
// assigning it a DWORD reg value
DWORD g_bAlwaysOnTop = false;
DWORD g_bShowInTray  = true;
DWORD g_bAnimateTrayIcon = true;


// tray icon
//#define MYWM_CLICKONTASKBARICON     (WM_APP+100) /* ein paar selbstgebaute */
#define MYWM_CLICKONTASKBARICON     (WM_USER+1)
void AddTaskBarIcon(HWND hWnd, HICON hIcon);
void DeleteTaskBarIcon(HWND hWnd);
void ModifyTaskBarIcon(HWND hWnd, HICON hIcon, char* szTip);

VOID APIENTRY DisplayContextMenu2(HWND hwnd, POINT pt);

// animated tray icon
HICON hIcon0;
HICON hIcon1;
HICON hIcon2;
char status2[128];
char szTip[64];

//bool bSignalConnected = false;
int iSignalStatus = 0;




// menu bitmap
HDC hdcMemory, hdcBuffer;
HDC hdcDest, hdcSrc, hdcMask;
HWND hBscWnd;

HBITMAP hbmBack1;
HBITMAP hbmBack2;

HBITMAP hbmSrc, hbmMask;

int x, y;

HMENU g_hmenuTrackPopupTray = NULL;



// The one and only CLimitSingleInstance object
// Change what is passed to constructor. GUIDGEN Tool may be of help.
CLimitSingleInstance g_SingleInstanceObj( TEXT("{5C01B124-0194-11d5-BB3B-444553540001}") );


// Registry value allowing R/C Sim (or Calibration itself) to check whether Calibration
// is already running and then Restore/SetForeground the window or Launch the app
DWORD g_hWndCalibration = NULL;


// Registry value allowing Calibration (or R/C Sim Sikorsky itself) to check whether Sikorsky
// is already running and then Restore/SetForeground the window or Launch the app
DWORD g_hWndSikorsky = NULL;











//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry Point
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	// allow only one instance
	if ( g_SingleInstanceObj.IsAnotherInstanceRunning() ) {

		// get HWND of running instance
		RegistryRead3();

		// Restore/SetForeground the window
		ShowWindow( (HWND)g_hWndCalibration, SW_RESTORE );
		SetForegroundWindow( (HWND)g_hWndCalibration );

		// get outta here
		return FALSE;
	}



	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;


	g_hInst = hInstance;

	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.style         = 0;
	wc.lpfnWndProc   = DlgWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = g_hInst;
	wc.hIcon         = LoadIcon(g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm       = LoadIcon(g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1)); //LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

 

	//InitCommonControls();

	// Dialog-based app: hWndParent = NULL
	// Main Window Proc becomes the Dialog's Callback Proc
	hWnd = CreateDialog( hInstance, MAKEINTRESOURCE(IDD_CONSOLE), (HWND)NULL,
								(DLGPROC)DlgWndProc );
	
	// always handy
	g_hWnd = hWnd;

	
	// tell registry we are running
	g_hWndCalibration = (DWORD)hWnd;
	RegistryWrite3();

	
	// Load VxD
	LoadVxD();


	// kludge to make sure that when executing from DevStudio Help is found.
	// DevStudio has ugly habit to execute from project directory
	SetCurrentDirectory( "Release" );


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





//-----------------------------------------------------------------------------
// Name: DlgWndProc()
// Desc:
// Note: ways too quit:
//			IDOK
//			IDCANCEL
//			SC_CLOSE
//			ID_POPUP_EXIT
//-----------------------------------------------------------------------------
LRESULT CALLBACK DlgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// loop-the-loop
	//GameLoop(hWnd);

	//MENUITEMINFO miiAlwaysOnTop;

	POINT pt;
	//RECT rc;

	LPMEASUREITEMSTRUCT lpmis;
	LPDRAWITEMSTRUCT lpdis;

	static int i = 0;





	switch (uMsg)
	{
		case WM_INITDIALOG:
			hIcon = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1) );
			SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon); // ICON_SMALL geeft geen icon entry in Alt-tab dialog

			// read some reg values
			RegistryRead2();

			if (g_bAlwaysOnTop)
				SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );
			else
				SetWindowPos( hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );					
		
			if (g_bShowInTray)
				AddTaskBarIcon( hWnd, hIcon );
			//else
			//	DeleteTaskBarIcon( hWnd );
			
			// system menu stuff
			hMenu = GetSystemMenu(hWnd, FALSE);
			if (hMenu != NULL)  {
				EnableMenuItem(hMenu, GetMenuItemID(hMenu,0), MF_GRAYED);
				EnableMenuItem(hMenu, GetMenuItemID(hMenu,2), MF_GRAYED);
				EnableMenuItem(hMenu, GetMenuItemID(hMenu,4), MF_GRAYED);

/*
				miiAlwaysOnTop.cbSize = sizeof(MENUITEMINFO);
				miiAlwaysOnTop.fMask = MIIM_CHECKMARKS | MIIM_DATA | MIIM_ID | MIIM_STATE | MIIM_SUBMENU | MIIM_TYPE;
				miiAlwaysOnTop.fType = MFT_STRING;
				miiAlwaysOnTop.fState = MFS_ENABLED | MFS_UNCHECKED; 
				miiAlwaysOnTop.wID = IDM_ALWAYSONTOP; 
				miiAlwaysOnTop.hSubMenu = NULL; 
				miiAlwaysOnTop.hbmpChecked = NULL; 
				miiAlwaysOnTop.hbmpUnchecked = NULL; 
				miiAlwaysOnTop.dwItemData = 0; 
				miiAlwaysOnTop.dwTypeData = "Always On &Top"; 
				miiAlwaysOnTop.cch = sizeof("Always On &Top");

				//InsertMenuItem(hMenu, 6, TRUE, &miiAlwaysOnTop);
*/
				
				InsertMenu(hMenu, 5, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
				InsertMenu(hMenu, 6, MF_BYPOSITION|MF_STRING, IDM_ALWAYSONTOP, "Always On &Top");
				InsertMenu(hMenu, 7, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
				InsertMenu(hMenu, 8, MF_BYPOSITION|MF_STRING, IDM_SHOWINTRAY, "Show In T&ray");
				InsertMenu(hMenu, 9, MF_BYPOSITION|MF_STRING, IDM_ANIMATETRAYICON, "An&imate Tray Icon");
				InsertMenu(hMenu, 10, MF_BYPOSITION|MF_SEPARATOR, 0, NULL);
				InsertMenu(hMenu, 11, MF_BYPOSITION|MF_STRING, IDM_ABOUTBOX, "&About...");

				if (g_bAlwaysOnTop)
					CheckMenuItem(hMenu, IDM_ALWAYSONTOP, MF_CHECKED);
				else
					CheckMenuItem(hMenu, IDM_ALWAYSONTOP, MF_UNCHECKED);

				if (g_bShowInTray) {
					CheckMenuItem(hMenu, IDM_SHOWINTRAY, MF_CHECKED);
					EnableMenuItem(hMenu, IDM_ANIMATETRAYICON, MF_ENABLED);
				} else {
					CheckMenuItem(hMenu, IDM_SHOWINTRAY, MF_UNCHECKED);
					EnableMenuItem(hMenu, IDM_ANIMATETRAYICON, MF_GRAYED);
				}

				if (g_bAnimateTrayIcon)
					CheckMenuItem(hMenu, IDM_ANIMATETRAYICON, MF_CHECKED);
				else
					CheckMenuItem(hMenu, IDM_ANIMATETRAYICON, MF_UNCHECKED);

				

				
				//AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
				//AppendMenu(hMenu, MF_STRING, IDM_ABOUTBOX, "&About...");
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
			
			 // used for getting keyboard input
			SetTimer( hWnd, 1, 50, NULL );

			// tooltips
			// NOTE: if we wanted tooltips for the main dialog window we must create
			// another procedure. If we use DoCreateDialogTooltip() and then do a tab change
			// without having unhooked in the main dialog window we'll CRASH.
			// But we can never unhook the main window's tooltips as they must be present
			// at the same time as the tab dialog's tooltips
			// Sollution: create a separate tooltip procedure for the main window
			// BUT: we don't really need tooltips for the main window...
			//g_hinst = g_hInst;
			//g_hwndDlg = hWnd;	
			//DoCreateDialogTooltip();
			
			// MUST be done before doing a new DoCreateDialogTooltip()
			// tooltips unhook, unhook!!!
			//UnhookWindowsHookEx(g_hhk);

			return TRUE;
			break;
		/*case WM_LBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
 
			// Despite the documentation, the point is in client coordinates
			if ( ChildWindowFromPoint(hWnd, pt) == GetDlgItem(hWnd, IDC_STATICQQQ) )
				MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			return FALSE;
			break;*/
		case WM_NOTIFY:
			// You get this if the user wants to change the tab page
			if ( ((LPNMHDR)lParam)->code == TCN_SELCHANGE ) {
				TabChange(hWnd);
			}

			// don't need this: put info in help file
			// tooltips for the calibrate button
			//OnWMNotify(lParam);
			break;

		case WM_SYSCOMMAND:
			switch ( (DWORD)wParam )
			{
				case SC_CLOSE:	// also when Close button clicked
					if (g_bShowInTray) {
						ShowWindow(hWnd, SW_HIDE);
						return 0;
					}
						
					// not really necessary
					DeleteTaskBarIcon( hWnd );

					// write some reg values
					RegistryWrite2();

					// tell registry we are down
					g_hWndCalibration = (DWORD)NULL;
					RegistryWrite3();
					
					DestroyWindow(hWnd);    // this won't kill the app!! 
					PostQuitMessage(0);     // kill!!!
					break;

				case IDM_ABOUTBOX:
					DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd,
								(DLGPROC)AboutProc );
					break;

				case IDM_ALWAYSONTOP:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					g_bAlwaysOnTop = !g_bAlwaysOnTop;
					
					if (g_bAlwaysOnTop)
						SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );
					else
						SetWindowPos( hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );					
					break;

				case IDM_SHOWINTRAY:
					g_bShowInTray = !g_bShowInTray;

					if (g_bShowInTray)
						AddTaskBarIcon( hWnd, hIcon );
					else
						DeleteTaskBarIcon( hWnd );
					break;

				case IDM_ANIMATETRAYICON:
					g_bAnimateTrayIcon = !g_bAnimateTrayIcon;

					if (g_bAnimateTrayIcon)
						CheckMenuItem(hMenu, IDM_ANIMATETRAYICON, MF_CHECKED);
					else
						CheckMenuItem(hMenu, IDM_ANIMATETRAYICON, MF_UNCHECKED);
					break;
				
			}
			break;

		case WM_INITMENU:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			hMenu = (HMENU) wParam; //GetSystemMenu(hWnd, FALSE);

			if (g_bAlwaysOnTop) 
				CheckMenuItem(hMenu, IDM_ALWAYSONTOP, MF_CHECKED);
			else
				CheckMenuItem(hMenu, IDM_ALWAYSONTOP, MF_UNCHECKED);

			if (g_bShowInTray) {
				CheckMenuItem(hMenu, IDM_SHOWINTRAY, MF_CHECKED);
				EnableMenuItem(hMenu, IDM_ANIMATETRAYICON, MF_ENABLED);
			} else {
				CheckMenuItem(hMenu, IDM_SHOWINTRAY, MF_UNCHECKED);
				EnableMenuItem(hMenu, IDM_ANIMATETRAYICON, MF_GRAYED);
			}
			break;

		case MYWM_CLICKONTASKBARICON:	// Taskbar-Icon 
			switch (lParam)
			{
				case WM_LBUTTONUP:
					break;

				case WM_RBUTTONUP:
					GetCursorPos(&pt);
					DisplayContextMenu2(hWnd, pt);
					break;

				case WM_LBUTTONDBLCLK:
					//ShowWindow(hWnd, SW_SHOW);
					ShowWindow(hWnd, SW_RESTORE);
					SetForegroundWindow(hWnd);
					break;

				default:
					return FALSE;
			}
			break;

		case WM_MEASUREITEM:
			// do the ownnerdraw menu stuff
			if ( 0 == (UINT) wParam ) {
				lpmis = (LPMEASUREITEMSTRUCT) lParam;
				lpmis->itemWidth = 5;
				lpmis->itemHeight = 150; // if smaller than actual menu height this will have no effect
				
				return TRUE;
			}
			break;

		case WM_DRAWITEM:
			// do the ownnerdraw menu stuff
			if ( 0 == (UINT) wParam ) {
				lpdis = (LPDRAWITEMSTRUCT) lParam;				

				hdcDest = lpdis->hDC;

				hdcMemory = CreateCompatibleDC(hdcDest);

				hbmBack1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
				SelectObject(hdcMemory, hbmBack1);

				// Yyyeeeessss!!!!
				// blt to menu owner-drawn menu item
				BitBlt(hdcDest, 0, 14, 21, 150, hdcMemory, 0, 0, SRCCOPY);

				// patch
				BitBlt(hdcDest, 0, 0, 21, 30, hdcMemory, 0, 0, BLACKNESS);
				
				// Clean up
				DeleteDC( hdcMemory );

				// laat het geheugen niet vollopen...
				DeleteObject(hbmBack1);

				return TRUE;
			}
			break;

		case WM_COMMAND:
			switch ( LOWORD(wParam) )
			{
				case IDOK:
					if (g_bShowInTray) {
						ShowWindow(hWnd, SW_HIDE);
						return 0;
					}

					// not really necessary
					DeleteTaskBarIcon( hWnd );

					// write some reg values
					RegistryWrite2();

					// tell registry we are down
					g_hWndCalibration = (DWORD)NULL;
					RegistryWrite3();

					//EndDialog( hWnd, TRUE );  // this won't kill the app!!!
					DestroyWindow(hWnd);        // this won't kill the app!!!
					PostQuitMessage(0);         // kill!!!
					break;

				case IDCANCEL:
					if (g_bShowInTray) {
						ShowWindow(hWnd, SW_HIDE);
						return 0;
					}

					// not really necessary
					DeleteTaskBarIcon( hWnd );

					// write some reg values
					RegistryWrite2();

					// tell registry we are down
					g_hWndCalibration = (DWORD)NULL;
					RegistryWrite3();

					//EndDialog( hWnd, FALSE );
					DestroyWindow(hWnd);
					PostQuitMessage(0);
					break;

				case IDC_BUTTONHELP:
					//WinHelp( hWnd, "..\\Help\\Rcsim.hlp", HELP_CONTEXT, 3 );
					HtmlHelp( hWnd, "..\\HTMLHelp\\Rcsim.chm", HH_DISPLAY_TOPIC, NULL );
					break;

				case IDC_CALIBRATE:
					//GetDlgItemText()
					if (!g_Calibrate) {
						g_Calibrate = true;
						SetDlgItemText(hWnd,IDC_CALIBRATE,"&Done");
					} else {
						g_Calibrate = false;
						SetDlgItemText(hWnd,IDC_CALIBRATE,"&Calibrate");
					}				
					break;

				case ID_POPUP_CALIBRATE:
					//ShowWindow(hWnd, SW_SHOW);
					ShowWindow(hWnd, SW_RESTORE);
					SetForegroundWindow(hWnd);
					break;
				
				case ID_POPUP_RCSIMSIKORSKY:
					// TODO: check if rcsim is already running, if so restore/setforeground
					// DONE
					RegistryRead4();
					if ( g_hWndSikorsky ) 
					{
						if ( IsMinimized((HWND)g_hWndSikorsky) )
							ShowWindow( (HWND)g_hWndSikorsky, SW_RESTORE );
						else
							ShowWindow( (HWND)g_hWndSikorsky, SW_SHOW );
						SetForegroundWindow( (HWND)g_hWndSikorsky );
					}
					else
					{
#ifdef _DEBUG ///////////////
						SetCurrentDirectory("F:\\mssdk\\samples\\Multimedia\\D3DIM\\src\\XFile\\Debug");
						if ( -1 == _spawnl( _P_NOWAIT,
								"F:\\mssdk\\samples\\Multimedia\\D3DIM\\src\\XFile\\Debug\\xfile.exe",
								"xfile.exe", NULL ) )
						{
								MessageBox(hWnd, "Could not spawn R/C Sim.",
									"R/C Sim Sikorsky Error Message", MB_OK|MB_ICONERROR);
						}
#else ///////////////////////						
						if ( -1 == _spawnl( _P_NOWAIT, "..\\bin\\xfile.exe", "xfile.exe", NULL ) ) {
								MessageBox(hWnd, "Could not spawn R/C Sim.",
									"R/C Sim Sikorsky Error Message", MB_OK|MB_ICONERROR);
						}					
#endif //////////////////////
					}
					break;
					
				case ID_POPUP_GALLAND:
					break;

				case ID_POPUP_WEBSITE:
					//_spawnl( _P_NOWAIT,	"C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE",
					//	"Iexplore.exe", "http://home.zonnet.nl/blacksphere/", NULL );

					ShellExecute( NULL, "open", "http://home.zonnet.nl/blacksphere/", "",
						"c:\\", SW_SHOWNORMAL );    
					break;

				case ID_POPUP_HELP:
					//WinHelp( hWnd, "..\\Help\\Rcsim.hlp", HELP_CONTEXT, 3 );
					HtmlHelp( hWnd, "..\\HTMLHelp\\Rcsim.chm", HH_DISPLAY_TOPIC, NULL );
					break;

				case ID_POPUP_ABOUT:
					DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd,
								(DLGPROC)AboutProc );
					break;

				case ID_POPUP_EXIT:
					// not really necessary
					DeleteTaskBarIcon( hWnd );

					// write some reg values
					RegistryWrite2();

					// tell registry we are down
					g_hWndCalibration = (DWORD)NULL;
					RegistryWrite3();

					//EndDialog( hWnd, TRUE );  // this won't kill the app!!!
					DestroyWindow(hWnd);        // this won't kill the app!!!
					PostQuitMessage(0);         // kill!!!
					break;

			}
			break;

		/*case WM_ACTIVATE:
			//MessageBeep(-1);
			if ( g_bShowInTray ) {
				if (HIWORD(wParam)!= 0) {
					ShowWindow(hWnd, SW_HIDE);
				} 
				//else {
				//	ShowWindow(hWnd, SW_SHOW);
				//}					
			}
			break;*/


		/*case WM_QUIT: // krijgt dialog niet
			MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (g_bShowInTray) {
				ShowWindow(hWnd, SW_HIDE);
				return 0;
			} else {
				DeleteTaskBarIcon( hWnd );
			}
			break;*/

		case WM_KEYDOWN: // fuck, why not???
			  // because catching key input in dialogs takes special steps
			  // see: the froggie apps
			  //if (VK_HOME == wParam)
			  //MessageBox(NULL,"qqq!","QQQ!",MB_OK);

			  return 0;
			  break;

		/*case WM_PAINT:
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
			return TRUE;
			break;*/

		case WM_TIMER:	// gets keyboard input
			if (GetKeyState(VK_F1) & 0x80)
				//WinHelp( hWnd, "..\\Help\\Rcsim.hlp", HELP_CONTEXT, 3 );
				HtmlHelp( hWnd, "..\\HTMLHelp\\Rcsim.chm", HH_DISPLAY_TOPIC, NULL );


			// We are putting this in timerloop instead of gameloop to prevent fucky
			// system menu behaviour
			if (g_bShowInTray) {
				if (!g_bAnimateTrayIcon) 
				{
					switch ( iSignalStatus )
					{ 
						case 0:
							hIcon0 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON10) );
							ModifyTaskBarIcon( g_hWnd, hIcon0, szTip );
							break;
						case 1:
							hIcon1 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON11) );
							ModifyTaskBarIcon( g_hWnd, hIcon1, szTip );
							break;
						case 2:
							hIcon2 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1) );
							ModifyTaskBarIcon( g_hWnd, hIcon2, szTip );
							break;
					}
				}
			}

/*
			// animate tray icon
			hIcon2 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1+i) );
			i++;
			if (i > 3) i = 0;
			
			if (g_bShowInTray) {
				ModifyTaskBarIcon( g_hWnd, hIcon2 );
			}
*/			
/*
			// kludge
			// ownerdraw menu item has probably f*cked up no-dismissal area: right-clicking
			// just under menu will not dismiss it. Let's do that ourselves
			// Don't work...
			if (g_hmenuTrackPopupTray) {
				if ( (GetKeyState(VK_LBUTTON) & 0x80) || (GetKeyState(VK_RBUTTON) & 0x80) ) {
					CloseWindow( (HWND)g_hmenuTrackPopupTray );
					MessageBeep(-1);
				}
			}
*/
/*
			// show bitmap in tray popup menu
			// Does not work like this: got to create a ownner-draw menu item.
			// Then handle WM_MEASUREITEM and WM_DRAWITEM
			if (g_hmenuTrackPopupTray) {

				MessageBeep(-1);

				//hdcDest = GetDC( (HWND)NULL );
				hdcDest = GetDC( (HWND)g_hmenuTrackPopupTray );

				hdcMemory = CreateCompatibleDC(hdcDest);

				hbmBack1 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
				SelectObject(hdcMemory, hbmBack1);

				x = 0;
				y = 0;

				//GetWindowRect( (HWND)g_hmenuTrackPopupTray, &rc );

				// blt to window
				//BitBlt(hdcDest, 0, 0, 21, 20, hdcMemory, x, y, SRCCOPY);

				ReleaseDC( (HWND)g_hmenuTrackPopupTray, hdcDest );
				
				// Clean up
				DeleteDC( hdcMemory );

				// laat het geheugen niet vollopen...
				DeleteObject(hbmBack1);
			}
*/
			break;

		default:
			return FALSE;
	}
	return FALSE;
}




//-----------------------------------------------------------------------------
// Name: GameLoop()
// Desc: loop-the-loop
//-----------------------------------------------------------------------------
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


	
	// set channels to 0 when they are deactivated by mute button
	for (i=1; i<=10; i++) {
		if (g_bChMute[i]) RetInfo[i] = 0;
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
		sprintf( status2, "No Signal");
		//bSignalConnected = false;
		iSignalStatus = 0;
	} else {
		if (ChannelTotal == 0) {
			//SetTextColor( GetDC(GetDlgItem(g_hTabCurrent,IDC_STATICSTATUS)), COLOUR_RED);
			g_crColor = COLOR_RED;
			sprintf( status, "Signal Defective       ");
			sprintf( status2, "Signal Defective");
			//bSignalConnected = false;
			iSignalStatus = 1;
		} else {
			//SetTextColor( GetDC(GetDlgItem(g_hTabCurrent,IDC_STATICSTATUS)), COLOUR_BLACK);
			g_crColor = COLOR_CONNECTED;//COLOR_BLUE;
			sprintf( status, "Signal Connected       ");
			//sprintf( status2, "Signal Connected - Interrupt Count: %i", IntCount); // helaas, tray icon tooltips zijn maximaal [64]
			sprintf( status2, "Signal Connected");
			//bSignalConnected = true;
			iSignalStatus = 2;
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



	// animate tray icon
	// Yes!!! How cool a gameloop is: timerloops suck forever.
	// Artifacts: 
	// - Start Button flickers
	// - System menu won't always show when right-clicking on task-bar program icon
	//   (no probs when right-clicking program caption)
	// We'll give user option to not animate.
	// TODO: for Signal Defective: clock-like animation, "waiting for connection"
	//       bSignalConnected becomes iSignalStatus (0,1,2)
	// TODO: option colored tray icons: 
	//		 No Signal and Signal Defective: red rotor
	//       Signal Connected: blue rotor
	// TODO: het is beter een array van hIcon's te maken ipv elke keer Load/DestroyIcon

	// No Signal: tray icon
	hIcon0 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON10) );

	// Signal Defective: animated tray icon
	static k=0;
	//hIcon1 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1+k) );
	static count1=0;
	count1++;
	if (count1%32==0) {
		hIcon1 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON11) );
		//k=0;
	}
	if (count1%64==0) {
		hIcon1 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON12) );
		//k=4;
	}
	//if (count1%16==0) {	// delay a bit
	//	k++;
	//	if (k > 7) k = 0;
	//}

	
	// Signal Connected: animated tray icon
	//static count2=0;
	//count2++;
	//if (count2%2==0) {	// delay a bit
		static j=0;
		hIcon2 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1+j) );
		j++;
		if (j > 7) j = 0;
	//}

	strcpy(szTip, "R/C Sim Transmitter Calibration - ");
	strcat(szTip, status2);
	
	if (g_bShowInTray) {
		if (g_bAnimateTrayIcon)
		{
			switch ( iSignalStatus )
			{ 
				case 0:
					hIcon0 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON10) );
					ModifyTaskBarIcon( g_hWnd, hIcon0, szTip );
					break;
				case 1:
					ModifyTaskBarIcon( g_hWnd, hIcon1, szTip );
					break;
				case 2:
					ModifyTaskBarIcon( g_hWnd, hIcon2, szTip );
					break;
			}
		}
/*
		// Let's put this in timerloop to get rid of fucky system menu behaviour
		else 
		{
			switch ( iSignalStatus )
			{ 
				case 0:
					hIcon0 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON10) );
					ModifyTaskBarIcon( g_hWnd, hIcon0, szTip );
					break;
				case 1:
					hIcon1 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON11) );
					ModifyTaskBarIcon( g_hWnd, hIcon1, szTip );
					break;
				case 2:
					hIcon2 = LoadIcon( g_hInst, (LPCSTR)MAKEINTRESOURCE(IDI_ICON1) );
					ModifyTaskBarIcon( g_hWnd, hIcon2, szTip );
					break;
			}
		}
*/
	}		



	// laat het geheugen niet vollopen!!!
	DestroyIcon(hIcon0);
	DestroyIcon(hIcon1);
	DestroyIcon(hIcon2);


}







//-----------------------------------------------------------------------------
// Name: AboutProc()
// Desc:
//-----------------------------------------------------------------------------
BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM )
{
	switch (uMsg) {
		case WM_INITDIALOG:
			//CenterDialog(hWnd);
			break;
	}
		
	if( WM_COMMAND == uMsg )
		if( IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam) )
			EndDialog( hWnd, TRUE );
	
	 return WM_INITDIALOG == uMsg ? TRUE : FALSE;
}




//-----------------------------------------------------------------------------
// Name: CenterDialog()
// Desc:
//-----------------------------------------------------------------------------
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




//-----------------------------------------------------------------------------
// Name: CreateTabControl()
// Desc:
//-----------------------------------------------------------------------------
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





//-----------------------------------------------------------------------------
// Name: DrawBarEx()
// Desc:
//-----------------------------------------------------------------------------
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





//-----------------------------------------------------------------------------
// Name: DisplayContextMenu()
// Desc:
//-----------------------------------------------------------------------------
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

	// Destroy the menu. 
    DestroyMenu(hmenuTrackPopup); 
} 
 



//-----------------------------------------------------------------------------
// Name: DisplayContextMenu2()
// Desc:
//-----------------------------------------------------------------------------
VOID APIENTRY DisplayContextMenu2(HWND hwnd, POINT pt) 
{ 
    HMENU hmenu;            // top-level menu 
    HMENU hmenuTrackPopup;  // shortcut menu

	// ownerdraw menu item has probably f*cked up no-dismissal area: right-clicking
	// just under menu will not dismiss it. Let's do that ourselves
	// Don't work...
	RECT rc;
	rc.left   = 0;
    rc.top    = 0;
    rc.right  = 0;
    rc.bottom = 0;



 
    // Load the menu resource. 
     if ((hmenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU2))) == NULL) 
        return; 
 
    // TrackPopupMenu cannot display the menu bar so get 
    // the handle of the first shortcut menu. 
    hmenuTrackPopup = GetSubMenu(hmenu, 0);
	g_hmenuTrackPopupTray = hmenuTrackPopup;

	// add ownerdrawn item 
	//InsertMenu(hmenuTrackPopup, 0, MF_BYPOSITION|MF_STRING, 0, "Rrr");
	//InsertMenu(hmenuTrackPopup, 0, MF_BYPOSITION|MF_BITMAP, 0, NULL);
	InsertMenu(hmenuTrackPopup, 0, MF_BYPOSITION|MF_DISABLED|MF_OWNERDRAW, 0, NULL);
 
    // Display the shortcut menu. Track the right mouse 
    // button. 
    TrackPopupMenu(hmenuTrackPopup, 
            TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
            pt.x, pt.y, 0, hwnd, NULL);

	g_hmenuTrackPopupTray = NULL;
 
    // Destroy the menu. 
    DestroyMenu(hmenu); 

	// Destroy the menu. 
    DestroyMenu(hmenuTrackPopup); 
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



//-----------------------------------------------------------------------------
// Name: DoCreateDialogTooltip()
// Desc:
//-----------------------------------------------------------------------------
BOOL DoCreateDialogTooltip(void)
{
    // Ensure that the common control DLL is loaded, and create 
    // a ToolTip control.
	InitCommonControls();
	
    g_hwndTT = CreateWindowEx(0, TOOLTIPS_CLASS, (LPSTR) NULL, 
        TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
        CW_USEDEFAULT, g_hwndDlg, (HMENU) NULL, g_hinst, NULL);
	
    if (g_hwndTT == NULL)         
		return FALSE; 
	
    // Enumerate the child windows to register them with the ToolTip
    // control. 
    if (!EnumChildWindows(g_hwndDlg, (WNDENUMPROC)EnumChildProc, 0)) 
        return FALSE;
	
    // Install a hook procedure to monitor the message stream for mouse 
    // messages intended for the controls in the dialog box.
	// NOTE NOTE NOTE: the hook MUST be unhooked before doing a new
	// DoCreateDialogTooltip(void) or we'll CRASH
    g_hhk = SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, 
        (HINSTANCE)NULL, GetCurrentThreadId()); 
	
    if (g_hhk == (HHOOK) NULL)         
		return FALSE;
	
	return TRUE; 
}


//-----------------------------------------------------------------------------
// Name: EnumChildProc()
// Desc: registers control windows with a ToolTip control by
//     using the TTM_ADDTOOL message to pass the address of a 
//     TOOLINFO structure. 
// Returns TRUE if successful or FALSE otherwise. 
// hwndCtrl - handle of a control window 
// lParam - application-defined value (not used)
//-----------------------------------------------------------------------------
BOOL EnumChildProc(HWND hwndCtrl, LPARAM lParam) 
{     
	TOOLINFO ti; 
    char szClass[64];      
	
	// Skip static controls. 
    GetClassName(hwndCtrl, szClass, sizeof(szClass));
	
    if (lstrcmp(szClass, "STATIC")) {

		ti.cbSize = sizeof(TOOLINFO); 
        ti.uFlags = TTF_IDISHWND;
		ti.hwnd = g_hwndDlg; 
        ti.uId = (UINT) hwndCtrl;
		ti.hinst = 0; 
        ti.lpszText = LPSTR_TEXTCALLBACK;
		
        SendMessage(g_hwndTT, TTM_ADDTOOL, 0, 
            (LPARAM) (LPTOOLINFO) &ti);     
	}
	
	return TRUE; 
}





//-----------------------------------------------------------------------------
// Name: GetMsgProc()
// Desc: monitors the message stream for mouse messages intended 
//     for a control window in the dialog box. 
// Returns a message-dependent value. 
// nCode - hook code 
// wParam - message flag (not used)
// lParam - address of an MSG structure 
//-----------------------------------------------------------------------------
LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
    MSG *lpmsg;
	lpmsg = (MSG *) lParam;
	
    if (nCode < 0 || !(IsChild(g_hwndDlg, lpmsg->hwnd))) 
        return (CallNextHookEx(g_hhk, nCode, wParam, lParam));
	
    switch (lpmsg->message) {
		case WM_MOUSEMOVE: 
        case WM_LBUTTONDOWN:
		case WM_LBUTTONUP: 
        case WM_RBUTTONDOWN:
		case WM_RBUTTONUP: 
            if (g_hwndTT != NULL) {
				MSG msg;  
                msg.lParam  = lpmsg->lParam; 
                msg.wParam  = lpmsg->wParam; 
                msg.message = lpmsg->message;
				msg.hwnd    = lpmsg->hwnd; 
                SendMessage(g_hwndTT, TTM_RELAYEVENT, 0, 
                    (LPARAM) (LPMSG) &msg);
			}
			break; 
        default:
			break;
	} 
    return (CallNextHookEx(g_hhk, nCode, wParam, lParam));
}



//-----------------------------------------------------------------------------
// Name: OnWMNotify()
// Desc: provides the ToolTip control with the appropriate text 
//     to display for a control window. This function is called by 
//     the dialog box procedure in response to a WM_NOTIFY message. 
// lParam - second message parameter of the WM_NOTIFY message 
//-----------------------------------------------------------------------------
VOID OnWMNotify(LPARAM lParam)
{
	LPTOOLTIPTEXT lpttt;
	int idCtrl;

	// big tool..erm..tits
	//POINT pt;
	//GetCursorPos(&pt);
	//MoveWindow(g_hwndTT, pt.x, pt.y+GetSystemMetrics(SM_CYSMICON)+2, 500, 200, true);
	
	// multi-line tooltips: use TTM_SETMAXTIPWIDTH and \r\n or \n
	SendMessage(g_hwndTT, TTM_SETMAXTIPWIDTH, 0, 300);
	//SendMessage(g_hwndTT, TTM_SETTIPBKCOLOR, RGB(255,0,0), 0);
	//SendMessage(g_hwndTT, TTM_SETTIPTEXTCOLOR, RGB(255,255,255), 0);
	
	

    if ((((LPNMHDR) lParam)->code) == TTN_NEEDTEXT) { 
        idCtrl = GetDlgCtrlID((HWND) ((LPNMHDR) lParam)->idFrom); 
        lpttt = (LPTOOLTIPTEXT) lParam;
		
		switch (idCtrl)
		{/* 
            case ID_HORZSCROLL: 
                lpttt->lpszText = "A horizontal scroll bar."; 
                return;
			case ID_CHECK: 
                lpttt->lpszText = "A check box.";
				return;  
            case ID_EDIT:
				lpttt->lpszText = "An edit control."; 
                return;*/
		case IDC_CHECKPERCENT:
				lpttt->lpszText = "Show/Hide Values"; 
                return;
		case IDC_BUTTONCENTER:
				lpttt->lpszText = "Center Sliders"; 
                return;
		case IDC_BUTTONCOLOR:
				lpttt->lpszText = "Set Color"; 
                return;
		/*case IDC_PROGRESSVERT1:
		case IDC_PROGRESSVERT2:
		case IDC_PROGRESSVERT3:
		case IDC_PROGRESSVERT4:
		case IDC_PROGRESSVERT5:
		case IDC_PROGRESSVERT6:
		case IDC_PROGRESSVERT7:
		case IDC_PROGRESSVERT8:
		case IDC_PROGRESSVERT9:
		case IDC_PROGRESSVERT10:
				lpttt->lpszText = "Set Individual Color: rigth-click or double-click"; 
                return;*/

		// should be done in separate tooltip procedure for the main dialog window
		/*case IDC_CALIBRATE:
				lpttt->lpszText = "Calibrate the transmitter: press Calibrate, \n"
								  "move all the sticks through their extreme positions, \n" 
								  "press Done when ready"; 
                return;*/
        }
    }
	return;
}




//-----------------------------------------------------------------------------
// Name: LoadVxD()
// Desc: Loads the VxD for the parallel port transmitter interface
//-----------------------------------------------------------------------------
void LoadVxD()
{
	char msg[512];

	if ( !GetSystemDirectory(lpBuffer, MAX_PATH) ) {
		dwErrorCode = GetLastError();
		sprintf(msg, "Could not get Sys Dir, Error code: %ld\n", dwErrorCode);
		MessageBox(NULL,msg,"QQQ",MB_ICONWARNING);
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
			sprintf(msg, "VxD loaded but device does not support DeviceIOCTL, Error code: %ld\n", dwErrorCode);
			MessageBox(NULL,msg,"QQQ",MB_ICONWARNING);

			//printf("\nHit any key to quit\n");
			//while ( !_kbhit() );
		}
		if ( dwErrorCode == ERROR_FILE_NOT_FOUND )
		{
			sprintf(msg, "Could not find VxD or it is not dynamically loadable, Error code: %ld\n", dwErrorCode);
			//MessageBox(NULL,msg,"QQQ",MB_ICONWARNING);

			//printf("\nHit any key to quit\n");
			//while ( !_kbhit() );
		}
	}
}








//-----------------------------------------------------------------------------
// Name: RegistryRead()
// Desc: Read registry values such as window position and dimensions
// TODO: read in ALL reg values
//-----------------------------------------------------------------------------
void RegistryRead()
{
    HKEY  hKey;
	//BYTE  lpData[512];
	DWORD cbData;
	//DWORD dw;
	BOOL b;
	//FLOAT f;
	//LONG  lRet;

	// read registry
	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Blacksphere\\R/C Sim Sikorsky",
				0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS )
	{

		// read g_bChInv[1]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[1]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[1] = b;
		}

		// read g_bChInv[2]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[2]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[2] = b;
		}

		// read g_bChInv[3]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[3]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[3] = b;
		}

		// read g_bChInv[4]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[4]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[4] = b;
		}

		// read g_bChInv[5]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[5]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[5] = b;
		}

		// read g_bChInv[6]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[6]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[6] = b;
		}

		// read g_bChInv[7]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[7]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[7] = b;
		}

		// read g_bChInv[8]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[8]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[8] = b;
		}

		// read g_bChInv[9]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[9]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[9] = b;
		}

		// read g_bChInv[10]
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bChInv[10]", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bChInv[10] = b;
		}

/*
		// read g_bRegWindowSize
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bRegWindowSize", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bRegWindowSize = b;
		}

		// read g_bRegFullScreen
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bRegFullScreen", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bRegFullScreen = b;
		}

		// read g_bRegSplashScreen
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bRegSplashScreen", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bRegSplashScreen = b;
			SPLASHSCREEN = g_bRegSplashScreen;
			m_bSplashScreenShowing = g_bRegSplashScreen;
		}


		// read g_dwRegModeWidth
		cbData = sizeof(dw);
		if ( RegQueryValueEx( hKey, "g_dwRegModeWidth", NULL, NULL,
			(LPBYTE)&dw, &cbData ) == ERROR_SUCCESS )
		{
			g_dwRegModeWidth = dw;
		}

		// read g_dwRegModeHeight
		cbData = sizeof(dw);
		if ( RegQueryValueEx( hKey, "g_dwRegModeHeight", NULL, NULL,
			(LPBYTE)&dw, &cbData ) == ERROR_SUCCESS )
		{
			g_dwRegModeHeight = dw;
		}

		// read g_dwRegModeRGBBitCount
		cbData = sizeof(dw);
		if ( RegQueryValueEx( hKey, "g_dwRegModeRGBBitCount", NULL, NULL,
			(LPBYTE)&dw, &cbData ) == ERROR_SUCCESS )
		{
			g_dwRegModeRGBBitCount = dw;
		}


		// read g_bRegLoadVxD
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bRegLoadVxD", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bRegLoadVxD = b;
		}

		// read g_bRegWelcomeVxD
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bRegWelcomeVxD", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bRegWelcomeVxD = b;
		}

		// read g_bRegGoodbyeVxD
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bRegGoodbyeVxD", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bRegGoodbyeVxD = b;
		}


		// read m_bSound
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "m_bSound", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			m_bSound = b;
		}

		// read m_bShowPPMarks
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "m_bShowPPMarks", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			m_bShowPPMarks = b;
		}


		// read g_bUseKeyboard
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bUseKeyboard", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bUseKeyboard = b;
		}

		// read g_bUseMouse
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bUseMouse", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bUseMouse = b;
		}

		// read g_bUseJoystick
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bUseJoystick", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bUseJoystick = b;
		}

		// read g_bUseTransmitter
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bUseTransmitter", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bUseTransmitter = b;
		}


		// read INIT_X
		cbData = sizeof(f);
		if ( RegQueryValueEx( hKey, "INIT_X", NULL, NULL,
			(LPBYTE)&f, &cbData ) == ERROR_SUCCESS )
		{
			INIT_X = f;
		}

		// read INIT_Y
		cbData = sizeof(f);
		if ( RegQueryValueEx( hKey, "INIT_Y", NULL, NULL,
			(LPBYTE)&f, &cbData ) == ERROR_SUCCESS )
		{
			INIT_Y = f;
		}

		// read INIT_Z
		cbData = sizeof(f);
		if ( RegQueryValueEx( hKey, "INIT_Z", NULL, NULL,
			(LPBYTE)&f, &cbData ) == ERROR_SUCCESS )
		{
			INIT_Z = f;
		}

		// read INIT_RADS_Y
		cbData = sizeof(f);
		if ( RegQueryValueEx( hKey, "INIT_RADS_Y", NULL, NULL,
			(LPBYTE)&f, &cbData ) == ERROR_SUCCESS )
		{
			INIT_RADS_Y = f;
		}

		// read m_bShowChannels
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "m_bShowChannels", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			m_bShowChannels = b;
		}
*/




		RegCloseKey(hKey);
	}
}



//-----------------------------------------------------------------------------
// Name: RegistryWrite()
// Desc: Write registry values such as window position and dimensions
//-----------------------------------------------------------------------------
void RegistryWrite()
{
    HKEY  hKey;
	//BYTE  lpData[80];
	//DWORD cbData = sizeof(lpData);
	DWORD dwDisposition;
	//DWORD dw;
	BOOL b;
	//FLOAT f;

	

	if ( RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Blacksphere\\R/C Sim Sikorsky",
			0, "Class", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
			&hKey, &dwDisposition) == ERROR_SUCCESS )
	{
		//MessageBox(NULL,"Registry open","QQQ",MB_OK);

		
		// write g_bChInv[1]
		b = g_bChInv[1];
		RegSetValueEx(hKey, "g_bChInv[1]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bChInv[2]
		b = g_bChInv[2];
		RegSetValueEx(hKey, "g_bChInv[2]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bChInv[3]
		b = g_bChInv[3];
		RegSetValueEx(hKey, "g_bChInv[3]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bChInv[4]
		b = g_bChInv[4];
		RegSetValueEx(hKey, "g_bChInv[4]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bChInv[5]
		b = g_bChInv[5];
		RegSetValueEx(hKey, "g_bChInv[5]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bChInv[6]
		b = g_bChInv[6];
		RegSetValueEx(hKey, "g_bChInv[6]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bChInv[7]
		b = g_bChInv[7];
		RegSetValueEx(hKey, "g_bChInv[7]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bChInv[8]
		b = g_bChInv[8];
		RegSetValueEx(hKey, "g_bChInv[8]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bChInv[9]
		b = g_bChInv[9];
		RegSetValueEx(hKey, "g_bChInv[9]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bChInv[10]
		b = g_bChInv[10];
		RegSetValueEx(hKey, "g_bChInv[10]", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));


/*
		// write x
		_itoa(x, (char *)lpData, 10);
		if ( RegSetValueEx(hKey, "1_x", 0, REG_SZ, 
				(CONST BYTE*)lpData, sizeof(*lpData)) == ERROR_SUCCESS )
		{
			//MessageBox(NULL,"Registry write x","QQQ",MB_OK);
		}

		// write y
		_itoa(y, (char *)lpData, 10);
		RegSetValueEx(hKey, "1_y", 0, REG_SZ, 
				(CONST BYTE*)lpData, sizeof(*lpData));

		// write w
		_itoa(w, (char *)lpData, 10);
		RegSetValueEx(hKey, "1_w", 0, REG_SZ, 
				(CONST BYTE*)lpData, sizeof(*lpData));

		// write h
		_itoa(h, (char *)lpData, 10);
		RegSetValueEx(hKey, "1_h", 0, REG_SZ, 
				(CONST BYTE*)lpData, sizeof(*lpData) );
*/

/*	
		// Better use DWORD instead of strings here
		// write x
		dw = x;
		if ( RegSetValueEx(hKey, "1_x", 0, REG_DWORD, 
				(LPBYTE)&dw, sizeof(dw)) == ERROR_SUCCESS )
		{
			//MessageBox(NULL,"Registry write x","QQQ",MB_OK);
		}

		// write y
		dw = y;
		RegSetValueEx(hKey, "1_y", 0, REG_DWORD, (LPBYTE)&dw, sizeof(dw));

		// write w
		dw = w;
		RegSetValueEx(hKey, "1_w", 0, REG_DWORD, (LPBYTE)&dw, sizeof(dw));

		// write h
		dw = h;
		RegSetValueEx(hKey, "1_h", 0, REG_DWORD, (LPBYTE)&dw, sizeof(dw));


		// write g_bRegWindowSize
		b = g_bRegWindowSize;
		RegSetValueEx(hKey, "g_bRegWindowSize", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));
		
		// write g_bRegFullScreen
		b = g_bRegFullScreen;
		RegSetValueEx(hKey, "g_bRegFullScreen", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bRegSplashScreen
		b = g_bRegSplashScreen;
		RegSetValueEx(hKey, "g_bRegSplashScreen", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));


		// write g_dwRegModeWidth
		dw = g_dwRegModeWidth;
		RegSetValueEx(hKey, "g_dwRegModeWidth", 0, REG_DWORD, (LPBYTE)&dw, sizeof(dw));

		// write g_dwRegModeHeight
		dw = g_dwRegModeHeight;
		RegSetValueEx(hKey, "g_dwRegModeHeight", 0, REG_DWORD, (LPBYTE)&dw, sizeof(dw));

		// write g_dwRegModeRGBBitCount
		dw = g_dwRegModeRGBBitCount;
		RegSetValueEx(hKey, "g_dwRegModeRGBBitCount", 0, REG_DWORD, (LPBYTE)&dw, sizeof(dw));

 
		// write g_bRegLoadVxD
		b = g_bRegLoadVxD;
		RegSetValueEx(hKey, "g_bRegLoadVxD", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bRegWelcomeVxD
		b = g_bRegWelcomeVxD;
		RegSetValueEx(hKey, "g_bRegWelcomeVxD", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bRegGoodbyeVxD
		b = g_bRegGoodbyeVxD;
		RegSetValueEx(hKey, "g_bRegGoodbyeVxD", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));


		// write m_bSound
		b = m_bSound;
		RegSetValueEx(hKey, "m_bSound", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write m_bShowPPMarks
		b = m_bShowPPMarks;
		RegSetValueEx(hKey, "m_bShowPPMarks", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));


		// write g_bUseKeyboard
		b = g_bUseKeyboard;
		RegSetValueEx(hKey, "g_bUseKeyboard", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bUseMouse
		b = g_bUseMouse;
		RegSetValueEx(hKey, "g_bUseMouse", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bUseJoystick
		b = g_bUseJoystick;
		RegSetValueEx(hKey, "g_bUseJoystick", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bUseTransmitter
		b = g_bUseTransmitter;
		RegSetValueEx(hKey, "g_bUseTransmitter", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));


		// NOTE: must write REG_BINARY for floats!!!!!!
		// write INIT_X
		f = INIT_X;
		RegSetValueEx(hKey, "INIT_X", 0, REG_BINARY, (LPBYTE)&f, sizeof(f));

		// write INIT_Y
		f = INIT_Y;
		RegSetValueEx(hKey, "INIT_Y", 0, REG_BINARY, (LPBYTE)&f, sizeof(f));

		// write INIT_Z
		f = INIT_Z;
		RegSetValueEx(hKey, "INIT_Z", 0, REG_BINARY, (LPBYTE)&f, sizeof(f));

		// write INIT_RADS_Y
		f = INIT_RADS_Y;
		RegSetValueEx(hKey, "INIT_RADS_Y", 0, REG_BINARY, (LPBYTE)&f, sizeof(f));

		// write m_bShowChannels
		b = m_bShowChannels;
		RegSetValueEx(hKey, "m_bShowChannels", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));
*/



		RegCloseKey(hKey);
	}
}




//-----------------------------------------------------------------------------
// Name: RegistryRead2()
// Desc: Read registry values
//-----------------------------------------------------------------------------
void RegistryRead2()
{
    HKEY  hKey;
	//BYTE  lpData[512];
	DWORD cbData;
	//DWORD dw;
	BOOL b;
	//FLOAT f;
	//LONG  lRet;

	// read registry
	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Blacksphere\\R/C Sim Sikorsky",
				0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS )
	{

		// read g_bAlwaysOnTop
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bAlwaysOnTop", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bAlwaysOnTop = b;
		}

		// read g_bShowInTray
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bShowInTray", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bShowInTray = b;
		}

		// read g_bAnimateTrayIcon
		cbData = sizeof(b);
		if ( RegQueryValueEx( hKey, "g_bAnimateTrayIcon", NULL, NULL,
			(LPBYTE)&b, &cbData ) == ERROR_SUCCESS )
		{
			g_bAnimateTrayIcon = b;
		}


		RegCloseKey(hKey);
	}
}



//-----------------------------------------------------------------------------
// Name: RegistryWrite2()
// Desc: Write registry values
//-----------------------------------------------------------------------------
void RegistryWrite2()
{
    HKEY  hKey;
	//BYTE  lpData[80];
	//DWORD cbData = sizeof(lpData);
	DWORD dwDisposition;
	//DWORD dw;
	BOOL b;
	//FLOAT f;

	

	if ( RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Blacksphere\\R/C Sim Sikorsky",
			0, "Class", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
			&hKey, &dwDisposition) == ERROR_SUCCESS )
	{
	
		// write g_bAlwaysOnTop
		b = g_bAlwaysOnTop;
		RegSetValueEx(hKey, "g_bAlwaysOnTop", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bShowInTray
		b = g_bShowInTray;
		RegSetValueEx(hKey, "g_bShowInTray", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));

		// write g_bAnimateTrayIcon
		b = g_bAnimateTrayIcon;
		RegSetValueEx(hKey, "g_bAnimateTrayIcon", 0, REG_DWORD, (LPBYTE)&b, sizeof(b));



		RegCloseKey(hKey);
	}
}





//-----------------------------------------------------------------------------
// Name: RegistryRead3()
// Desc: Read registry values
//-----------------------------------------------------------------------------
void RegistryRead3()
{
    HKEY  hKey;
	//BYTE  lpData[512];
	DWORD cbData;
	DWORD dw;
	//BOOL b;
	//FLOAT f;
	//LONG  lRet;

	// read registry
	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Blacksphere\\R/C Sim Sikorsky",
				0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS )
	{

		// read g_hWndCalibration
		cbData = sizeof(dw);
		if ( RegQueryValueEx( hKey, "g_hWndCalibration", NULL, NULL,
			(LPBYTE)&dw, &cbData ) == ERROR_SUCCESS )
		{
			g_hWndCalibration = dw;
		}


		RegCloseKey(hKey);
	}
}



//-----------------------------------------------------------------------------
// Name: RegistryWrite3()
// Desc: Write registry values
//-----------------------------------------------------------------------------
void RegistryWrite3()
{
    HKEY  hKey;
	//BYTE  lpData[80];
	//DWORD cbData = sizeof(lpData);
	DWORD dwDisposition;
	DWORD dw;
	//BOOL b;
	//FLOAT f;

	

	if ( RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Blacksphere\\R/C Sim Sikorsky",
			0, "Class", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
			&hKey, &dwDisposition) == ERROR_SUCCESS )
	{
	
		// write g_hWndCalibration
		dw = g_hWndCalibration;
		RegSetValueEx(hKey, "g_hWndCalibration", 0, REG_DWORD, (LPBYTE)&dw, sizeof(dw));


		RegCloseKey(hKey);
	}
}



//-----------------------------------------------------------------------------
// Name: RegistryRead4()
// Desc: Read registry values
//-----------------------------------------------------------------------------
void RegistryRead4()
{
    HKEY  hKey;
	//BYTE  lpData[512];
	DWORD cbData;
	DWORD dw;
	//BOOL b;
	//FLOAT f;
	//LONG  lRet;

	// read registry
	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Blacksphere\\R/C Sim Sikorsky",
				0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS )
	{

		// read g_hWndSikorsky
		cbData = sizeof(dw);
		if ( RegQueryValueEx( hKey, "g_hWndSikorsky", NULL, NULL,
			(LPBYTE)&dw, &cbData ) == ERROR_SUCCESS )
		{
			g_hWndSikorsky = dw;
		}


		RegCloseKey(hKey);
	}
}



//-----------------------------------------------------------------------------
// Name: RegistryWrite4()
// Desc: Write registry values
//-----------------------------------------------------------------------------
void RegistryWrite4()
{
    HKEY  hKey;
	//BYTE  lpData[80];
	//DWORD cbData = sizeof(lpData);
	DWORD dwDisposition;
	DWORD dw;
	//BOOL b;
	//FLOAT f;

	

	if ( RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Blacksphere\\R/C Sim Sikorsky",
			0, "Class", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
			&hKey, &dwDisposition) == ERROR_SUCCESS )
	{

		// write g_hWndSikorsky
		dw = g_hWndSikorsky;
		RegSetValueEx(hKey, "g_hWndSikorsky", 0, REG_DWORD, (LPBYTE)&dw, sizeof(dw));		


		RegCloseKey(hKey);
	}
}






/*************************************************/
/*   fügt Icon in das Statusfeld (Taskbar) ein   */
/*************************************************/
void AddTaskBarIcon(HWND hWnd, HICON hIcon) 
{
	NOTIFYICONDATA	icondaten;

	icondaten.cbSize = sizeof(NOTIFYICONDATA);
	icondaten.hWnd = hWnd;
	icondaten.uID = IDI_ICON1;
	icondaten.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	icondaten.uCallbackMessage = MYWM_CLICKONTASKBARICON;
	icondaten.hIcon = hIcon;
	lstrcpyn(icondaten.szTip, "R/C Sim Transmitter Calibration", 
		sizeof("R/C Sim Transmitter Calibration"));

	Shell_NotifyIcon(NIM_ADD, &icondaten);
}


/****************************************/
/*   entfernt Icon aus dem Statusfeld   */
/****************************************/
void DeleteTaskBarIcon(HWND hWnd) 
{ 
	NOTIFYICONDATA icondaten;

	icondaten.cbSize = sizeof(NOTIFYICONDATA);
	icondaten.hWnd = hWnd;
	icondaten.uID = IDI_ICON1;

	Shell_NotifyIcon(NIM_DELETE, &icondaten);
}


/****************************************/
/*   modifiziert Icon im Statusfeld     */
/****************************************/
void ModifyTaskBarIcon(HWND hWnd, HICON hIcon, char* szTip) 
{ 
	NOTIFYICONDATA icondaten;

	icondaten.cbSize = sizeof(NOTIFYICONDATA);
	icondaten.hWnd = hWnd;
	icondaten.uID = IDI_ICON1;
	icondaten.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	icondaten.uCallbackMessage = MYWM_CLICKONTASKBARICON;
	icondaten.hIcon = hIcon;
	lstrcpyn(icondaten.szTip, szTip, 64);

	Shell_NotifyIcon(NIM_MODIFY, &icondaten);
}


