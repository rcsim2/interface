//-----------------------------------------------------------------------------
// File: Bigpush.h
//
// Desc: R/C Sim Transmitter Calibration
//		 Bigpush.h is the master header for the project.
//
//
// Copyright (c) 2000 Black Sphere Corp.
//-----------------------------------------------------------------------------
#pragma once
#ifndef __BIGPUSH_H
#define __BIGPUSH_H

// the raw Win32 API

// NOTE NOTE NOTE: not defining STRICT can cause LNK2001 Errors!!!
//
// From the docs: Enabling STRICT Type Checking 
// Note: If you are writing a C++ application, you don't have the option of applying
// STRICT to only some of your source files. Because of the way C++ "type safe linking"
// works, mixing STRICT and non-STRICT source files in your application can cause 
// linking errors.
//
// So instead of in every source file, it would be better to define STRICT in 
// Project|Settings|C/C++|General|Preprocessor Definitions

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <process.h>
#include <htmlhelp.h>

#include "resource.h"
#include "tabdlg.h"
#include "LimitSingleInstance.h" 




// globals (are good for you)
extern char g_szClassName[];
extern HINSTANCE g_hInst;
extern HWND g_hWnd;

extern HICON hIcon;
extern HMENU hMenu;

extern DLGTEMPLATE* pTabPage[5];
extern HWND g_hTabControl;
extern HWND g_hTabCurrent;

extern bool g_bHorizontal;
extern bool g_bStop;

extern BOOL g_bReady;
extern BOOL g_bActive; 


// VxD variables
#define VMYXD_APIFUNC_1 1
#define VMYXD_APIFUNC_2 2

extern HANDLE  hVxD;
extern DWORD   cbBytesReturned;
extern DWORD   dwErrorCode;
//DWORD   RetInfo[13];	// 1 Sync Pulse + 10 Channels + 1 IntCount + 1 PulseLength

// ints rule!!!!!
// Note: better use int because DWORD is unsigned!!!!!!!!!!!
extern int		RetInfo[88]; 	// big sucker to test
//int	PrevRetInfo[88]; 

extern int		IntCount;
extern int		ChannelTotal;

extern CHAR*   strVxDFileName;
extern CHAR    lpBuffer[MAX_PATH];
extern CHAR    strVxDFilePath[MAX_PATH];


#define COLOR_BLUE 	RGB(0,0,255)
#define COLOR_BLACK	RGB(0,0,0)
#define COLOR_RED	RGB(255,0,0)
#define COLOR_WHITE	RGB(255,255,255)

extern COLORREF g_crColor;

extern COLORREF COLOUR_TEXT;
extern COLORREF COLOUR_BAR;
extern COLORREF COLOR_CONNECTED;

extern COLORREF g_crColorBar[11];
 


#define STRICT


//#ifdef STRICT
extern WNDPROC wpOrigProgressBar1Proc;
extern WNDPROC wpOrigProgressBar2Proc;
extern WNDPROC wpOrigProgressBar3Proc;
extern WNDPROC wpOrigProgressBar4Proc;
extern WNDPROC wpOrigProgressBar5Proc;
extern WNDPROC wpOrigProgressBar6Proc;
extern WNDPROC wpOrigProgressBar7Proc;
extern WNDPROC wpOrigProgressBar8Proc;
extern WNDPROC wpOrigProgressBar9Proc;
extern WNDPROC wpOrigProgressBar10Proc;
/*#else   
extern FARPROC wpOrigProgressBar1Proc;
extern FARPROC wpOrigProgressBar2Proc;
extern FARPROC wpOrigProgressBar3Proc;
extern FARPROC wpOrigProgressBar4Proc;
extern FARPROC wpOrigProgressBar5Proc;
extern FARPROC wpOrigProgressBar6Proc;
extern FARPROC wpOrigProgressBar7Proc;
extern FARPROC wpOrigProgressBar8Proc;
extern FARPROC wpOrigProgressBar9Proc;
extern FARPROC wpOrigProgressBar10Proc;
#endif
*/


//#ifdef STRICT
extern WNDPROC wpOrigTrackBar1Proc;
extern WNDPROC wpOrigTrackBar2Proc;
extern WNDPROC wpOrigTrackBar3Proc;
extern WNDPROC wpOrigTrackBar4Proc;
extern WNDPROC wpOrigTrackBar5Proc;
extern WNDPROC wpOrigTrackBar6Proc;
extern WNDPROC wpOrigTrackBar7Proc;
extern WNDPROC wpOrigTrackBar8Proc;
extern WNDPROC wpOrigTrackBar9Proc;
extern WNDPROC wpOrigTrackBar10Proc;
/*#else   
extern FARPROC wpOrigTrackBar1Proc;
extern FARPROC wpOrigTrackBar2Proc;
extern FARPROC wpOrigTrackBar3Proc;
extern FARPROC wpOrigTrackBar4Proc;
extern FARPROC wpOrigTrackBar5Proc;
extern FARPROC wpOrigTrackBar6Proc;
extern FARPROC wpOrigTrackBar7Proc;
extern FARPROC wpOrigTrackBar8Proc;
extern FARPROC wpOrigTrackBar9Proc;
extern FARPROC wpOrigTrackBar10Proc;
#endif
*/


extern HHOOK hHook;


extern bool g_bChMute[11];

// Let's make it a DWORD instead of bool to prevent C4800 warnings
// when reading from registry
extern DWORD g_bChInv[11];
extern bool g_bChExp[11];

extern int g_iChSens[11];

extern bool g_bShowPercent;


extern bool g_bItemFree[12];

extern int g_iChMap[11];


extern HINSTANCE g_hinst;	// handle of the application instance 
extern HWND g_hwndTT;		// handle of the ToolTip control 
extern HWND g_hwndDlg;		// handle of the dialog box 
extern HHOOK g_hhk;		// handle of the hook procedure 





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
extern bool g_Calibrate;


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
extern DWORD g_bAlwaysOnTop;
extern DWORD g_bShowInTray;


// tray icon
//#define MYWM_CLICKONTASKBARICON     (WM_APP+100) /* ein paar selbstgebaute */
#define MYWM_CLICKONTASKBARICON     (WM_USER+1)
void AddTaskBarIcon(HWND hWnd, HICON hIcon);
void DeleteTaskBarIcon(HWND hWnd);
void ModifyTaskBarIcon(HWND hWnd, HICON hIcon, char* szTip);

VOID APIENTRY DisplayContextMenu2(HWND hwnd, POINT pt);


// menu bitmap
extern HDC hdcMemory, hdcBuffer;
extern HDC hdcDest, hdcSrc, hdcMask;
extern HWND hBscWnd;

extern HBITMAP hbmBack1;
extern HBITMAP hbmBack2;

extern HBITMAP hbmSrc, hbmMask;

extern int x, y;




#endif // __BIGPUSH_H