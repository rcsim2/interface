

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