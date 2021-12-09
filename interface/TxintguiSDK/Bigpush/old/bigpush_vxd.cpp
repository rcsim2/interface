// the raw Win32 API
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"

char g_szClassName[] = "MyWindowClass";
HINSTANCE g_hInst = NULL;
HWND g_hWnd = NULL;

HICON hIcon = NULL;;
HMENU hMenu = NULL;

DLGTEMPLATE* pTabPage[5];
HWND g_hTabControl = NULL;
HWND g_hTabCurrent = NULL;

bool g_bHorizontal = false;


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

void DrawBar(HWND hwnd,int percent);









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

	
	
	BOOL bGotMsg;
	BOOL g_bReady = TRUE;
	BOOL g_bActive = TRUE; 
	
	// Special message pump 
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
			TranslateMessage( &msg );
			DispatchMessage( &msg );
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

			// Set focus to OK button
			// Note: Kan niet met SetFocus()
			SendMessage(hWnd,WM_NEXTDLGCTL,0,FALSE);

			// TODO: (misschien) dotted line focus on first tab so user kan tab
			// to next tab right away: see GetRight Config

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


// This is the dialog procedure of tab page 2
BOOL CALLBACK Tab2DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)
	{
	case WM_INITDIALOG:
        TabCenter(hWnd);
		//MessageBox(NULL,"QQQ","xxx",MB_OK);
		SetFocus(hWnd);
		break;
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
	switch(uMsg)
	{
		case WM_INITDIALOG:
			TabCenter(hWnd);
			SetFocus(hWnd);
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
			}
		break;
 
		default:
			return(FALSE);
  }
 
  return(0);
}



void GameLoop()
{
	static int nPos = 0;

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
		if (RetInfo[i] != 0) RetInfo[i]-=300; // leave unused channels alone
		// RetInfo[i] must be signed here!!!!!!!!!!!!!!!!!!
		// We've changed it from DWORD to int so it's OK now
		if (RetInfo[i] < 0)  RetInfo[i] =  0; // never print negative values
	}

	//DrawBar( GetDlgItem(g_hTabCurrent,IDC_PROGRESSHORZ1), int((RetInfo[1]-1000)/10) );
	
	// use sprintf and a buffer
	//SetDlgItemText(g_hTabCurrent ,IDC_STATIC1, "QQQ");
	SetDlgItemInt(g_hTabCurrent ,IDC_STATIC1, RetInfo[1], TRUE); 

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
}




#define COLOUR_BAR		RGB(0,0,255)
#define COLOUR_TEXT		RGB(255,255,255)
 
void DrawBar(HWND hwnd,int percent)
{
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