/****************************************************************************
*                                                                           *
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY     *
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE       *
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR     *
* PURPOSE.                                                                  *
*                                                                           *
* Copyright 1993-95  Microsoft Corporation.  All Rights Reserved.           *
*                                                                           *
****************************************************************************/

/****************************************************************************
*
* PROGRAM: LOAD.C
*
* PURPOSE: Simple Win32 console application for calling dynaload VMYXD VxD.
*		   VMYXD will return values to this application through the
*	       DeviceIoControl interface.
*
****************************************************************************/

#include <stdio.h>
#include <windows.h>
#include <conio.h>
//#include <vmm.h>
//#include <vxdldr.h>

#define VMYXD_APIFUNC_1 1
#define VMYXD_APIFUNC_2 2



int main()
{
	CONSOLE_CURSOR_INFO cci;// = {50,TRUE};
	cci.dwSize = 50;
	cci.bVisible = TRUE;

	HANDLE hConsoleOutput = CreateConsoleScreenBuffer( 
									GENERIC_READ|GENERIC_WRITE,
									FILE_SHARE_READ|FILE_SHARE_WRITE,
									NULL, CONSOLE_TEXTMODE_BUFFER, NULL);


	HANDLE  hVxD = 0;
	DWORD   cbBytesReturned;
	DWORD   dwErrorCode;
	//DWORD   RetInfo[13];	// 1 Sync Pulse + 10 Channels + 1 IntCount + 1 PulseLength
	DWORD   RetInfo[88]; 	// big sucker to test 
	
	DWORD	IntCount = 0;

	
	CHAR*   strVxDFileName = "Vmyxd.vxd";
	CHAR    lpBuffer[MAX_PATH];
	CHAR    strVxDFilePath[MAX_PATH];
	
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
			
			printf("\nHit any key to quit\n");
			while ( !_kbhit() );
		}
		if ( dwErrorCode == ERROR_FILE_NOT_FOUND )
		{
			printf("Could not find VxD or it is not dynamically loadable, Error code: %ld\n", dwErrorCode);
			
			printf("\nHit any key to quit\n");
			while ( !_kbhit() );
		}
	}
	else
	{
		printf("VxD loaded and DeviceIOCTL ready to go!\n\n");
		
		RetInfo[0]=666; // test value, the vxd should change this value

		//MessageBox(NULL, "Yo!", "Blacksphere", MB_OK);
		
		// Make 1st VxD call here
		if ( DeviceIoControl(hVxD, VMYXD_APIFUNC_1,
				(LPVOID)NULL, 0,
				(LPVOID)RetInfo, sizeof(RetInfo),
				&cbBytesReturned, NULL) )
		{
			printf("Yo! VxD call 1\n");
			while ( !_kbhit() ) {
				
				// Poll the VxD values
				DeviceIoControl(hVxD, VMYXD_APIFUNC_1,
					(LPVOID)NULL, 0,
					(LPVOID)RetInfo, sizeof(RetInfo),
					&cbBytesReturned, NULL);
					
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
				
				if ( RetInfo[11] > IntCount ) {
					printf("Blacksphere VxD Utilities (C) 2000\n\n");
					
					printf("Sync Pulse: %ld\n\n", RetInfo[0]);
					printf("Channel  1: %ld\n\n", RetInfo[1]);
					printf("Channel  2: %ld\n\n", RetInfo[2]);
					printf("Channel  3: %ld\n\n", RetInfo[3]);
					printf("Channel  4: %ld\n\n", RetInfo[4]);
					printf("Channel  5: %ld\n\n", RetInfo[5]);
					printf("Channel  6: %ld\n\n", RetInfo[6]);
					printf("Channel  7: %ld\n\n", RetInfo[7]);
					printf("Channel  8: %ld\n\n", RetInfo[8]);
					printf("Channel  9: %ld\n\n", RetInfo[9]);
					printf("Channel 10: %ld\n\n", RetInfo[10]);
					
					//printf("Pulse Length: %ld\n\n", RetInfo[12]);
					
					/* Can't call ints from Win32 App!
					__asm {
						mov	ah, 1	; make cursor disappear
						mov	ch, 20h
						int	10h
						
						mov	ah, 2	; move cursor ah=2
						mov	bh, 0
						mov	dh, 0
						mov	dl, 0
						int	10h
					}
					*/
					
					// Only for GUI apps
					//ShowCursor(FALSE);
					 
					//SetConsoleCursorPosition();
					SetConsoleCursorInfo(hConsoleOutput, &cci);


					// Clear screen
					Sleep(100);
					system("CLS");

				}
				IntCount = RetInfo[11];
			}
		}
		else
		{
			printf("Device does not support the requested API\n");
		}

		// Make 2nd VxD call here
		if ( DeviceIoControl(hVxD, VMYXD_APIFUNC_2,
				(LPVOID)NULL, 0,
				(LPVOID)RetInfo, sizeof(RetInfo),
				&cbBytesReturned, NULL) )
		{
			//printf("System VM Handle: %lx\n", RetInfo[0]);
			//printf("VKD_Get_Kbd_Owner: %lx\n", RetInfo[1]);
			printf("Yo! VxD call 2\n");
		}
		else
		{
			printf("Device does not support the requested API\n");
		}


		_getch();
		printf("\nHit any key to start unloading VxD with CloseHandle()\n");
		while ( !_kbhit() );
		
		// Dynamically UNLOAD the Virtual Device
		if ( CloseHandle(hVxD) ) {
			printf("\nVxD handle closed successfully, \
				\nVXDLDR will now try to unload the VxD but may still fail\n");
		} else {
			printf("\nVxD handle close failed\n");
		}

/*      
		// This should only be necessary in rare cases
		// Hell, this always returns success, even when vxd isn't loaded!!!
		// Note: DeleteFile() is buggy when unloading VXDs:
		// It returns:
		// 0 when successful
		// nonzero when it fails
		if ( DeleteFile("\\\\.\\VMYXD") ) {
			//printf("\nDeleteFile() successful\n");
			printf("\nDeleteFile() failed\n");
		} else {
			//printf("\nDeleteFile() failed\n");
			printf("\nDeleteFile() successful\n");
		}
*/      

		// Use _getch to throw key away and test again for keystroke
		_getch(); 
		printf("\nHit any key to quit\n");
		while ( !_kbhit() );
		  
	}

	return(0);
}


// Clears screen in Win32 console app 
void cls( HANDLE hConsole ) {

    COORD coordScreen = { 0, 0 };    /* here's where we'll home the cursor */
    BOOL bSuccess;
	DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */ 
    DWORD dwConSize;                /* number of character cells in the current buffer */
	
    /* get the number of character cells in the current buffer */
	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	//PERR( bSuccess, "GetConsoleScreenBufferInfo" );
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    
	/* fill the entire screen with blanks */ 
    bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );
    //PERR( bSuccess, "FillConsoleOutputCharacter" );
    
	/* get the current text attribute */ 
    bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	//PERR( bSuccess, "ConsoleScreenBufferInfo" );
    
	/* now set the buffer's attributes accordingly */ 
    bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
    //PERR( bSuccess, "FillConsoleOutputAttribute" );
    
	/* put the cursor at (0, 0) */ 
    bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
	//PERR( bSuccess, "SetConsoleCursorPosition" );    
	
	return;
} 

