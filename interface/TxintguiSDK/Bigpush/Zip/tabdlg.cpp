//-----------------------------------------------------------------------------
// File: tabdlg.cpp
//
// Desc: tab control procedures for R/C Sim Transmitter Calibration
//
//
// Copyright (c) 2000 Black Sphere Corp.
//------------------------------------------------------------------------------
#define STRICT


#include "bigpush.h"

#include "tabdlg.h"





//-----------------------------------------------------------------------------
// Name: Tab1DlgProc()
// Desc: This is the dialog procedure of tab page 1
//       Resources
//-----------------------------------------------------------------------------
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
	  /*case WM_LBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			// works only with static controls, not progress bars
			// and works only if static control is not in group box
			if ( ChildWindowFromPoint(hWnd, pt) == GetDlgItem(hWnd, IDC_STATICQQQ) ) {
				MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			}
			break;*/
	  case WM_CTLCOLORSTATIC:
		  if ((HWND)lParam == GetDlgItem(hWnd,IDC_STATICSTATUS))
			  SetTextColor( (HDC)wParam, g_crColor );

		  SetBkColor( (HDC)wParam, GetSysColor(COLOR_MENU) ); // zet alleen background van characters niet van entire window
		  hBrushDlg = (HBRUSH)GetStockObject(NULL_BRUSH) ;
          return (UINT)hBrushDlg ;
		  break;
	  //case WM_PAINT:
			//PAINTSTRUCT ps;
			//BeginPaint(hWnd, &ps);
			
			//EndPaint(hWnd, &ps);
			//return 0;
	      //break;
	  /*case WM_DESTROY:
		  UnhookWindowsHookEx(hHook);
		  break;*/
 
      default:
          return(FALSE);
  }
 
  return(0);
}



//-----------------------------------------------------------------------------
// Name: Tab2DlgProc()
// Desc: This is the dialog procedure of tab page 2
//       Channels (Vertical)
//-----------------------------------------------------------------------------
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
	
	static bool toggle = true;


	
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
			// dat komt natuurlijk omdat je de Class style van progress bars verandert
			// Als je SetWindowLong() zou gebruiken moest je dat voor alle windows afzonderlijk doen
			// Echter, er bestaat geen WS_DBLCLKS...
			// make the fucker respond to double-clicks
			SetClassLong( GetDlgItem(hWnd, IDC_PROGRESSVERT1), GCL_STYLE, (LONG)CS_DBLCLKS );
			//SetWindowLong( GetDlgItem(hWnd, IDC_PROGRESSVERT1), GWL_STYLE, (LONG)(LONG)WS_DBLCLKS );

			
			// read registry
			RegistryRead();


			// set invert buttons according to channel invert state
			for (i=1; i<=10; i++) {
				if (g_bChInv[i])
					SendMessage( GetDlgItem(hWnd, IDC_CHECK11+(i-1)), BM_SETCHECK, BST_CHECKED, 0 );
				else
					SendMessage( GetDlgItem(hWnd, IDC_CHECK11+(i-1)), BM_SETCHECK, BST_UNCHECKED, 0 );
			}

			// set mute buttons according to channel mute state
			for (i=1; i<=10; i++) {
				if (g_bChMute[i])
					SendMessage( GetDlgItem(hWnd, IDC_CHECK21+(i-1)), BM_SETCHECK, BST_CHECKED, 0 );
				else
					SendMessage( GetDlgItem(hWnd, IDC_CHECK21+(i-1)), BM_SETCHECK, BST_UNCHECKED, 0 );
			}


			// tooltips
			g_hinst = g_hInst;
			g_hwndDlg = hWnd;	
			DoCreateDialogTooltip();
			 

			break;
		case WM_KEYDOWN: // fuck, why not???
			// because catching key input in dialogs takes special steps
			// see: the froggie apps
			//if (VK_HOME == wParam) {
				  MessageBox(NULL,"qqq!","QQQ!",MB_OK);
				  //return 0;
			//}
			break;
		/*case WM_LBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			// works only with static controls, not progress bars
			// and works only if static control is not in group box
			if ( ChildWindowFromPoint(hWnd, pt) == GetDlgItem(hWnd, IDC_STATICEXP) ) {
				MessageBox(NULL,"qqq!","QQQ!",MB_OK);
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

				// channel mute
				case IDC_CHECK21: g_bChMute[1] = !g_bChMute[1]; break;
				case IDC_CHECK22: g_bChMute[2] = !g_bChMute[2]; break;
				case IDC_CHECK23: g_bChMute[3] = !g_bChMute[3]; break;
				case IDC_CHECK24: g_bChMute[4] = !g_bChMute[4]; break;
				case IDC_CHECK25: g_bChMute[5] = !g_bChMute[5]; break;
				case IDC_CHECK26: g_bChMute[6] = !g_bChMute[6]; break;
				case IDC_CHECK27: g_bChMute[7] = !g_bChMute[7]; break;
				case IDC_CHECK28: g_bChMute[8] = !g_bChMute[8]; break;
				case IDC_CHECK29: g_bChMute[9] = !g_bChMute[9]; break;
				case IDC_CHECK30: g_bChMute[10] = !g_bChMute[10]; break;

				case IDC_BUTTONMASTER:					
					toggle = !toggle;

					for (i=1; i<=10; i++) {
						if (toggle) {
							g_bChMute[i] = false;
							SendMessage( GetDlgItem(hWnd, IDC_CHECK21+(i-1)), BM_SETCHECK, BST_UNCHECKED, 0 );
						} else {
							g_bChMute[i] = true;
							SendMessage( GetDlgItem(hWnd, IDC_CHECK21+(i-1)), BM_SETCHECK, BST_CHECKED, 0 );
						}
					}
					break;

				
				// channel invert
				case IDC_CHECK11: g_bChInv[1] = !g_bChInv[1]; break;
				case IDC_CHECK12: g_bChInv[2] = !g_bChInv[2]; break;
				case IDC_CHECK13: g_bChInv[3] = !g_bChInv[3]; break;
				case IDC_CHECK14: g_bChInv[4] = !g_bChInv[4]; break;
				case IDC_CHECK15: g_bChInv[5] = !g_bChInv[5]; break;
				case IDC_CHECK16: g_bChInv[6] = !g_bChInv[6]; break;
				case IDC_CHECK17: g_bChInv[7] = !g_bChInv[7]; break;
				case IDC_CHECK18: g_bChInv[8] = !g_bChInv[8]; break;
				case IDC_CHECK19: g_bChInv[9] = !g_bChInv[9]; break;
				case IDC_CHECK20: g_bChInv[10] = !g_bChInv[10]; break;
			}

			// write to registry
			RegistryWrite();
			break;

		case WM_NOTIFY:
			// tooltips
			OnWMNotify(lParam);
			break;

		case WM_DESTROY:
			// tooltips unhook, unhook!!!
			UnhookWindowsHookEx(g_hhk);
			break;

		default:
			return(FALSE);
	}

	return(0);
}



//-----------------------------------------------------------------------------
// Name: Tab3DlgProc()
// Desc: This is the dialog procedure of tab page 3
//       Mapping
//-----------------------------------------------------------------------------
BOOL CALLBACK Tab3DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//int dIndex;
	//PAINTSTRUCT ps;
	int i;
	int idComboBox;
	static int iOldItem = 0;
	static int iNewItem = 0;

	char Buffer[128];

	static bool bfirsttime = true;
	int ichannel;


	switch(uMsg)
	{
		case WM_INITDIALOG:
			TabCenter(hWnd);
			//SetFocus(hWnd); // no focus to get rid of blue selected first combo box

			for (i=0; i<10; i++) {
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Throttle");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Roll");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Nick");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Tail");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Pitch");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Func6");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Func7");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Func8");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Func9");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Func10");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Func11");
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_ADDSTRING, 0, (LPARAM)(LPCSTR)"Func12");

				//SendMessage( GetDlgItem(hWnd, IDC_COMBO1+i), CB_SETCURSEL, 0, 0);
			}

			
			// mc-20 specific
			// should only do this the very first time the app starts
			// after that we should read settings from the Registry
			// set combo boxes according to present channel mapping
			if (bfirsttime) {
				bfirsttime = false;
				SendMessage( GetDlgItem(hWnd, IDC_COMBO1), CB_SETCURSEL, 0, 0); g_bItemFree[0] = false; g_iChMap[1] = 0;
				SendMessage( GetDlgItem(hWnd, IDC_COMBO2), CB_SETCURSEL, 1, 0); g_bItemFree[1] = false; g_iChMap[2] = 1;
				SendMessage( GetDlgItem(hWnd, IDC_COMBO3), CB_SETCURSEL, 2, 0); g_bItemFree[2] = false; g_iChMap[3] = 2;
				SendMessage( GetDlgItem(hWnd, IDC_COMBO4), CB_SETCURSEL, 3, 0); g_bItemFree[3] = false; g_iChMap[4] = 3;
				SendMessage( GetDlgItem(hWnd, IDC_COMBO6), CB_SETCURSEL, 4, 0); g_bItemFree[4] = false; g_iChMap[6] = 4;
			} else {
				for (i=1; i<=10; i++) {
					SendMessage( GetDlgItem(hWnd, IDC_COMBO1+(i-1)), CB_SETCURSEL, g_iChMap[i], 0 );
				}
			}
			
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

			// track bar stuff
			for (i=0; i<10; i++) {
				SendMessage( GetDlgItem(hWnd, IDC_SLIDER1+i), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 200) );  
				SendMessage( GetDlgItem(hWnd, IDC_SLIDER1+i), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)100 );
				
				//SendMessage( GetDlgItem(hWnd, IDC_SLIDER1+i), TBM_SETTIC, (WPARAM)0, (LPARAM)(LONG)0 );
				//SendMessage( GetDlgItem(hWnd, IDC_SLIDER1+i), TBM_SETTIC, (WPARAM)0, (LPARAM)(LONG)100 );
				//SendMessage( GetDlgItem(hWnd, IDC_SLIDER1+i), TBM_SETTIC, (WPARAM)0, (LPARAM)(LONG)200 );
			}

			
			// should only do this the very first time the app starts
			// after that we should read settings from the Registry
			// set track bar position according to channel sensitivity
			// and set percentage
			for (i=1; i<=10; i++) {
				SendMessage( GetDlgItem(hWnd, IDC_SLIDER1+(i-1)), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)g_iChSens[i] );

				char sens[128];
				sprintf( sens, "%i%%", g_iChSens[i]);
				SetDlgItemText( hWnd, IDC_STATICSENS1+(i-1), sens );
			}

			
			// set percent check button
			if (g_bShowPercent)
				SendMessage( GetDlgItem(hWnd, IDC_CHECKPERCENT), BM_SETCHECK, BST_CHECKED, 0 );
			else
				SendMessage( GetDlgItem(hWnd, IDC_CHECKPERCENT), BM_SETCHECK, BST_UNCHECKED, 0 );


			// set percent static text
			for (i=1; i<=10; i++) {
				if (g_bShowPercent)
					ShowWindow( GetDlgItem(hWnd, IDC_STATICSENS1+(i-1)), TRUE);
				else
					ShowWindow( GetDlgItem(hWnd, IDC_STATICSENS1+(i-1)), FALSE);
			}


			// subclass 'em
			wpOrigTrackBar1Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER1), GWL_WNDPROC, (LONG)TrackBar1SubclassProc );
			wpOrigTrackBar2Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER2), GWL_WNDPROC, (LONG)TrackBar2SubclassProc );
			wpOrigTrackBar3Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER3), GWL_WNDPROC, (LONG)TrackBar3SubclassProc );
			wpOrigTrackBar4Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER4), GWL_WNDPROC, (LONG)TrackBar4SubclassProc );
			wpOrigTrackBar5Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER5), GWL_WNDPROC, (LONG)TrackBar5SubclassProc );
			wpOrigTrackBar6Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER6), GWL_WNDPROC, (LONG)TrackBar6SubclassProc );
			wpOrigTrackBar7Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER7), GWL_WNDPROC, (LONG)TrackBar7SubclassProc );
			wpOrigTrackBar8Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER8), GWL_WNDPROC, (LONG)TrackBar8SubclassProc );
			wpOrigTrackBar9Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER9), GWL_WNDPROC, (LONG)TrackBar9SubclassProc );
			wpOrigTrackBar10Proc = (WNDPROC)SetWindowLong( GetDlgItem(hWnd, IDC_SLIDER10), GWL_WNDPROC, (LONG)TrackBar10SubclassProc );
			
			
			// tooltips
			g_hinst = g_hInst;
			g_hwndDlg = hWnd;	
			DoCreateDialogTooltip();

			return TRUE;
			break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
			{
				case IDC_BUTTONCENTER:
					for (i=1; i<=10; i++) {
						SendMessage( GetDlgItem(hWnd, IDC_SLIDER1+(i-1)), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)100 );
						g_iChSens[i] = 100;
						SetDlgItemText( hWnd, IDC_STATICSENS1+(i-1), "100%" );
					}
					break;
				case IDC_CHECKPERCENT:
					g_bShowPercent = !g_bShowPercent;
					for (i=1; i<=10; i++) {
						if (g_bShowPercent)
							ShowWindow( GetDlgItem(hWnd, IDC_STATICSENS1+(i-1)), TRUE);
						else
							ShowWindow( GetDlgItem(hWnd, IDC_STATICSENS1+(i-1)), FALSE);
					}
					break;
			}

			switch( HIWORD(wParam) )
			{
				case CBN_DROPDOWN:
					idComboBox = (int) LOWORD(wParam);
					iOldItem = SendMessage( (HWND)lParam, CB_GETCURSEL, 0, 0 );
					Buffer[128];

					// append an asterisk to the items if already assigned
					// get rid of the asterisk if free
					for (i=0; i<12; i++) {
						if ( iOldItem != i ) {
							if ( !g_bItemFree[i] ) {
								SendMessage( (HWND)lParam, CB_GETLBTEXT, i, (LPARAM)&Buffer );
								if ( !strchr(Buffer, '*') )
									strcat(Buffer, "*");
								SendMessage( (HWND)lParam, CB_DELETESTRING, i, 0 );
								SendMessage( (HWND)lParam, CB_INSERTSTRING, i, (LPARAM)&Buffer );
							} else {
								SendMessage( (HWND)lParam, CB_GETLBTEXT, i, (LPARAM)&Buffer );
								char* p = strchr(Buffer, '*');
								if (p != NULL)
									*p = '\0';
								SendMessage( (HWND)lParam, CB_DELETESTRING, i, 0 );
								SendMessage( (HWND)lParam, CB_INSERTSTRING, i, (LPARAM)&Buffer );
							}
						}
					}
					SendMessage( (HWND)lParam, CB_SETCURSEL, iOldItem, 0 );

		
					break;
				//case CBN_SELCHANGE:
				//case CBN_CLOSEUP:
				case CBN_SELENDOK:
					//SetCapture( hWnd );
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					idComboBox = (int) LOWORD(wParam);
					iNewItem = SendMessage( (HWND)lParam, CB_GETCURSEL, 0, 0 );
					Buffer[128];

					
					// clear the item if it was selected in another combo box
					// and clear that channel mapping
					for (i=1; i<=10; i++) {
						if ( idComboBox != (IDC_COMBO1+(i-1)) && iNewItem != CB_ERR ) {
	
							if ( iNewItem == SendMessage( GetDlgItem(hWnd, IDC_COMBO1+(i-1)), CB_GETCURSEL, 0, 0 ) ) {
								SendMessage( GetDlgItem(hWnd, IDC_COMBO1+(i-1)), CB_SETCURSEL, -1, 0 );
								g_iChMap[i] = -1;
							}
						}
					}

					// get rid of asterisk in this combo box's selection
					if ( iNewItem != CB_ERR ) {
						SendMessage( (HWND)lParam, CB_GETLBTEXT, iNewItem, (LPARAM)&Buffer );
						char* p = strchr(Buffer, '*');
						if (p != NULL)
							*p = '\0';

						SendMessage( (HWND)lParam, CB_DELETESTRING, iNewItem, 0 );
						SendMessage( (HWND)lParam, CB_INSERTSTRING, iNewItem, (LPARAM)&Buffer );
						SendMessage( (HWND)lParam, CB_SETCURSEL, iNewItem, 0 );
					}

					// flag new item assigned
					g_bItemFree[iNewItem] = false;

					// set channel mapping to new item
					ichannel = (idComboBox - IDC_COMBO1)+1;
					g_iChMap[ichannel] = iNewItem;

					// flag old item free if a new one is chosen
					if ( iNewItem != iOldItem )
						g_bItemFree[iOldItem] = true;

						
					break;
				/*case CBN_SELCHANGE:
					//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
					break;*/
				case CBN_SETFOCUS:
					//SetCapture( GetDlgItem(hWnd, IDC_COMBO1) );
					break;
			}
			break;
		/*case WM_PAINT:
			BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
			break;*/
		/*case WM_LBUTTONDOWN:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			// works only with static controls, not progress bars
			// and works only if static control is not in group box
			if ( ChildWindowFromPoint(hWnd, pt) == GetDlgItem(hWnd, IDC_STATICMAX) ) {
				MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			}
			break;*/
		case WM_HSCROLL:			
			for (i=1; i<=10; i++) {
				if ( lParam == (LPARAM)GetDlgItem(hWnd, IDC_SLIDER1+(i-1)) ) {
					g_iChSens[i] = SendMessage( GetDlgItem(hWnd, IDC_SLIDER1+(i-1)), TBM_GETPOS, 0, 0 );
					int iconnect = g_iChSens[i];
					// pressing Control key while sliding connects all sliders
					if ( GetKeyState(VK_CONTROL) & 0x80 ) {
						for (i=1; i<=10; i++) {
							SendMessage( GetDlgItem(hWnd, IDC_SLIDER1+(i-1)), TBM_SETPOS, TRUE, iconnect );
							char sens[128];
							sprintf( sens, "%i%%", iconnect);
							SetDlgItemText( hWnd, IDC_STATICSENS1+(i-1), sens );
						}
					} else {
						char sens[128];
						sprintf( sens, "%i%%", g_iChSens[i]);
						SetDlgItemText( hWnd, IDC_STATICSENS1+(i-1), sens );
						//SetDlgItemText( hWnd, IDC_EDIT1, sens );
					}
				}
			}
			break;
		case WM_NOTIFY:
			// tooltips
			OnWMNotify(lParam);
			break;
		case WM_DESTROY:
			// tooltip unhook, unhook!!!
			UnhookWindowsHookEx(g_hhk);
			break;
		default:
			return(FALSE);
	}

	return(0);
}




//-----------------------------------------------------------------------------
// Name: Tab4DlgProc()
// Desc: This is the dialog procedure of tab page 4
//       
//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
// Name: Tab5DlgProc()
// Desc: This is the dialog procedure of tab page 5
//       Channels (Horizontal)
//-----------------------------------------------------------------------------
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
			

			// tooltips
			g_hinst = g_hInst;
			g_hwndDlg = hWnd;	
			DoCreateDialogTooltip();

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
						for (i=1; i<=10; i++) {
							g_crColorBar[i] = cc.rgbResult;
						}
						COLOR_CONNECTED = cc.rgbResult;
					}
					break;
			}
			break;
		case WM_NOTIFY:
			// tooltips
			OnWMNotify(lParam);
			break;
		case WM_DESTROY:
			// tooltips unhook, unhook!!!
			UnhookWindowsHookEx(g_hhk);
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
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

    return CallWindowProc((WNDPROC)wpOrigProgressBar1Proc, hWnd, uMsg, wParam, lParam); 
}






// Subclass procedure 1
LRESULT APIENTRY TrackBar1SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // This was so evil: it prevented us from tabbing to the next dialog item
	// not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS1), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS1), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar1Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 2
LRESULT APIENTRY TrackBar2SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS2), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS2), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar2Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 3
LRESULT APIENTRY TrackBar3SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS3), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS3), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar3Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 4
LRESULT APIENTRY TrackBar4SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS4), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS4), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar4Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 5
LRESULT APIENTRY TrackBar5SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS5), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS5), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar5Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 6
LRESULT APIENTRY TrackBar6SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS6), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS6), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar6Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 7
LRESULT APIENTRY TrackBar7SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS7), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS7), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar7Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 8
LRESULT APIENTRY TrackBar8SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS8), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS8), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar8Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 9
LRESULT APIENTRY TrackBar9SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS9), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS9), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar9Proc, hWnd, uMsg, wParam, lParam); 
}


// Subclass procedure 10
LRESULT APIENTRY TrackBar10SubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
    // not necessary???
	//if (uMsg == WM_GETDLGCODE) 
    //    return DLGC_WANTALLKEYS|DLGC_BUTTON|DLGC_STATIC;
	
	switch (uMsg)
	{
		case WM_SETFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS10), TRUE);
			break;
		case WM_KILLFOCUS:
			//MessageBox(NULL,"qqq!","QQQ!",MB_OK);
			if (!g_bShowPercent) ShowWindow( GetDlgItem(g_hTabCurrent, IDC_STATICSENS10), FALSE);
			break;

	}

    return CallWindowProc((WNDPROC)wpOrigTrackBar10Proc, hWnd, uMsg, wParam, lParam); 
}