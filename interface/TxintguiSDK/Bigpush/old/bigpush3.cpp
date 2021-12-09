// the raw Win32 API
#include <windows.h>
#include "resource.h"

static char g_szClassName[] = "MyWindowClass";
static HINSTANCE g_hInst = NULL;



LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
		case WM_INITDIALOG:
			return TRUE;
			break;
        case WM_SYSCOMMAND:
            switch( (DWORD) wParam )
            {
                case SC_CLOSE:
                    DestroyWindow(hWnd);	// this won't kill the app!! 
                    PostQuitMessage(0);		// kill!!!
            }
            break;
		case WM_COMMAND:
			switch( LOWORD(wParam) )
            {
				case IDOK:
					//EndDialog( hWnd, TRUE );	// this won't kill the app!!!
					DestroyWindow(hWnd);		// this won't kill the app!!!
					PostQuitMessage(0);			// kill!!!
					break;
				case IDCANCEL:
					//EndDialog( hWnd, FALSE );
					DestroyWindow(hWnd);
					PostQuitMessage(0);
					break;
			}
			break;
		//default:
		//	return( DefWindowProc( hWnd, Message, wParam, lParam )); // don't do this!!!
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
   WndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
   WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
   WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
   WndClass.lpszMenuName  = NULL;
   WndClass.lpszClassName = g_szClassName;
   WndClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

   RegisterClassEx(&WndClass);


   // Dialog-based app: hWnd = NULL
   // Main Window Proc becomes the Dialog's Callback Proc
   hWnd = CreateDialog( hInstance, MAKEINTRESOURCE(IDD_CONSOLE), (HWND)NULL,
							(DLGPROC)WndProc );



   while(GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   return msg.wParam;
}

