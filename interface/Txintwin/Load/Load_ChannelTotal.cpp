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


// prototypes
void cls( HANDLE hConsole );


int main()
{
	CONSOLE_CURSOR_INFO cci;// = {50,TRUE}; // struct init only compulsary in C
	cci.dwSize = 50; //99; buggy bugger 100==invisible
	cci.bVisible = FALSE; //TRUE;

	COORD coord = {0,0};
	//DWORD NumberOfAttrsWritten;

	//HANDLE hConsoleOutput = CreateConsoleScreenBuffer( 
	//								GENERIC_READ|GENERIC_WRITE,
	//								FILE_SHARE_READ|FILE_SHARE_WRITE,
	//								NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	//
	//if(hConsoleOutput==INVALID_HANDLE_VALUE)
	//	printf("Could not get hConsoleOutput");
	
	//SetConsoleActiveScreenBuffer(hConsoleOutput);
	
	HANDLE hConsoleOutput= GetStdHandle(STD_OUTPUT_HANDLE);
	
	// No cursor
	SetConsoleCursorInfo(hConsoleOutput, &cci);

	// Color
	//FillConsoleOutputAttribute(hConsoleOutput, BACKGROUND_RED|BACKGROUND_BLUE,
	//										80*25, coord, &NumberOfAttrsWritten);
	//SetConsoleTextAttribute(hConsoleOutput, BACKGROUND_RED|BACKGROUND_BLUE); 





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


				
				char* ChL[11];	// We don't use ChL[0] to keep channel numbers the same
								// Watch out for wild pointers
				// safety first
				// Can't do that like this: we must initialize to same size as L ?????
				ChL[0] = NULL;
				ChL[1] = NULL;
				ChL[2] = NULL;
				ChL[3] = NULL;
				ChL[4] = NULL;
				ChL[5] = NULL;
				ChL[6] = NULL;
				ChL[7] = NULL;
				ChL[8] = NULL;
				ChL[9] = NULL;
				ChL[10]= NULL;


				char* L[21];

				L[0] = "\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0"; 
				L[1] = "\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[2] = "\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[3] = "\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[4] = "\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[5] = "\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[6] = "\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[7] = "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[8] = "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[9] = "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[10]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[11]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[12]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[13]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0\xB0";
				L[14]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0\xB0";
				L[15]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0\xB0";
				L[16]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0\xB0";
				L[17]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0\xB0";
				L[18]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0\xB0";
				L[19]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB0";
				L[20]= "\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB";

			

				for (int i=1; i<=10; i++) {
					if		(RetInfo[i]<1300) ChL[i]= L[0] ;
					else if (RetInfo[i]<1350) ChL[i]= L[1] ;
					else if (RetInfo[i]<1400) ChL[i]= L[2] ;
					else if (RetInfo[i]<1450) ChL[i]= L[3] ;
					else if (RetInfo[i]<1500) ChL[i]= L[4] ;
					else if (RetInfo[i]<1550) ChL[i]= L[5] ;
					else if (RetInfo[i]<1600) ChL[i]= L[6] ;
					else if (RetInfo[i]<1650) ChL[i]= L[7] ;
					else if (RetInfo[i]<1700) ChL[i]= L[8] ;
					else if (RetInfo[i]<1750) ChL[i]= L[9] ;
					else if (RetInfo[i]<1800) ChL[i]= L[10];
					else if (RetInfo[i]<1850) ChL[i]= L[11];
					else if (RetInfo[i]<1900) ChL[i]= L[12];
					else if (RetInfo[i]<1950) ChL[i]= L[13];
					else if (RetInfo[i]<2000) ChL[i]= L[14];
					else if (RetInfo[i]<2050) ChL[i]= L[15];
					else if (RetInfo[i]<2100) ChL[i]= L[16];
					else if (RetInfo[i]<2150) ChL[i]= L[17];
					else if (RetInfo[i]<2200) ChL[i]= L[18];
					else if (RetInfo[i]<2250) ChL[i]= L[19];
					else					  ChL[i]= L[20];
				}
				
				// pas waarde een beetje aan zodat we 1000-2000 uS printen
				for (i=0; i<=10; i++) {
					if (RetInfo[i] != 0) RetInfo[i]-=250; // leave unused channels alone
					// RetInfo[i] must be signed here!!!!!!!!!!!!!!!!!!
					// We've changed it from DWORD to int so it's OK now
					if (RetInfo[i] < 0)  RetInfo[i] =  0; // never print negative values
				}	
				
				
				
				// NOTE: DWORD is unsigned and causes that we never get 777
				//for (i=0; i<=10; i++) {
				//	RetInfo[i] = -666; // minus is prob
				//
				//	// Huh, why not???
				//	if ( RetInfo[i] < 0 ) RetInfo[i]=777; // we never get 777
				//}
				//
				//Instead do this:
				//for (i=0; i<=10; i++) {
				//	(int)RetInfo[i] = 666;
				//	if ( RetInfo[i] < 667 ) RetInfo[i]=777; // now we get 777
				//}
				/////////////////////////////////////////////////////////////////////////
				
				
				// TODO: handle the occasional Ch9 and Ch10 printf caused bij interference signals
				// Je zou kunnen checken of de waarde gelijk is met die van vorige keer. Dat is een
				// redelijk betrouwbare indicatie dat het een "stray pulse" was.
				// Probleem is dat je dan ook gebruikte channels die toevallig net gelijk zijn aan
				// de vorige puls vangt.
				// Het beste zou zijn als we van de VxD de hoogste ChannelCount zouden lezen en op 
				// basis daarvan de ongebruikte kanalen op nul te houden.
				// Het probleem hierbij is dat we bij de stray pulse naar 9 schieten
				// We zouden na een aantal recursions de ChannelTotal moeten accepteren en beschouwen
				// als de definitieve total.
				// En dan maar hopen dat er geen stray pulse binnen die tijd zich voordoet die de count
				// hoger zou zetten dan het correcte aantal.
				// Meevaller: dit is de enige juiste manier om de total channel count te achterhalen.
				// En het blijkt bovendien dat stray pulses de total channel count niet beinvloeden.
				// Hij blijft op 8, ook al hebben we een printf op 9 of 10.
				// Maar die kunnen we dus weer op 0 zetten op basis van de total channel count.
				// Als volgt...

				// get total number of channels
				if ( RetInfo[13] > ChannelTotal )
					ChannelTotal = RetInfo[13];

				// set unused channels at length 0
				for (i=10; i>ChannelTotal; i--) {
					ChL[i] = L[0];
				}




				printf("                     Blacksphere VxD Utilities (C) 2000\n\n");
				
				printf(" Sync Pulse: %ld   Channel Total: %ld\n\n", RetInfo[0], ChannelTotal);

				printf(" Channel  1: %s %ld\n\n", ChL[1],  RetInfo[1] );
				printf(" Channel  2: %s %ld\n\n", ChL[2],  RetInfo[2] );
				printf(" Channel  3: %s %ld\n\n", ChL[3],  RetInfo[3] );
				printf(" Channel  4: %s %ld\n\n", ChL[4],  RetInfo[4] );
				printf(" Channel  5: %s %ld\n\n", ChL[5],  RetInfo[5] );
				printf(" Channel  6: %s %ld\n\n", ChL[6],  RetInfo[6] );
				printf(" Channel  7: %s %ld\n\n", ChL[7],  RetInfo[7] );
				printf(" Channel  8: %s %ld\n\n", ChL[8],  RetInfo[8] );
				printf(" Channel  9: %s %ld\n\n", ChL[9],  RetInfo[9] );
				printf(" Channel 10: %s %ld\n\n", ChL[10], RetInfo[10]);
				
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
				//SetConsoleCursorInfo(hConsoleOutput, &cci);

				// Clear screen
				Sleep(50);
				//system("CLS");
				cls(hConsoleOutput);
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
void cls( HANDLE hConsole )
{
    COORD coordScreen = { 0, 0 };    /* here's where we'll home the cursor */
	COORD coordScreen2 = { 18, 0 };
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
    //bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
    //PERR( bSuccess, "FillConsoleOutputAttribute" );
	FillConsoleOutputAttribute( hConsole, BACKGROUND_BLUE, 40, coordScreen2, &cCharsWritten );
    
	/* put the cursor at (0, 0) */ 
    bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
	//PERR( bSuccess, "SetConsoleCursorPosition" );    
	
	return;
} 

