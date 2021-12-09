PAGE 58,132
TITLE VMYXD - Transmitter VxD

		.386p

		.XLIST
		INCLUDE vmm.inc
		INCLUDE debug.inc
		INCLUDE vpicd.inc
		INCLUDE vtd.inc
		INCLUDE vwin32.inc
		.LIST

Declare_Virtual_Device VMYXD, 1, 0, VMYXD_Control, Undefined_Device_ID,,
;Declare_Virtual_Device VMYXD, 1, 0, VMYXD_Control, Undefined_Device_ID,\
;						VMYXD_API_Proc, VMYXD_API_Proc

;==============================================================================
VxD_IDATA_SEG

;VMYXD_IRQ_Desc VPICD_IRQ_Descriptor <7,,OFFSET32 VMYXD_Hw_Int,,\
;						OFFSET32 VMYXD_EOI>
VMYXD_IRQ_Desc VPICD_IRQ_Descriptor <7, VPICD_OPT_CAN_SHARE, OFFSET32 VMYXD_Hw_Int,,\
						OFFSET32 VMYXD_EOI>

VxD_IDATA_ENDS

;==============================================================================
VxD_DATA_SEG
VxD_DATA_ENDS

;==============================================================================
VxD_LOCKED_DATA_SEG

PrevIntTime	dword   0
GotIntTime      dword   0
PulseLength	dword   0
GotSync         dword   0
ChannelCount    dword   0
SyncPulse       dword   0
Ch1             dword   0
Ch2             dword   0
Ch3             dword   0
Ch4             dword   0
Ch5             dword   0
Ch6             dword   0
Ch7             dword   0
Ch8             dword   0
Ch9             dword   0
Ch10            dword   0

VxD_LOCKED_DATA_ENDS


;==============================================================================
VxD_ICODE_SEG

BeginProc VMYXD_Sys_Critical_Init

		clc                             ;no error - continue
		ret

EndProc VMYXD_Sys_Critical_Init


BeginProc VMYXD_Device_Init

		clc                             ;no error - continue
		ret

EndProc VMYXD_Device_Init


BeginProc VMYXD_Init_Complete

		clc                             ;no error - continue
		ret

EndProc VMYXD_Init_Complete

;------------------------------------------------------------------------------
BeginProc VMYXD_Sys_Dynamic_Device_Init

		mov 	edi, OFFSET32 VMYXD_IRQ_Desc
		VxDcall VPICD_Virtualize_IRQ

		mov 	eax, VxD_SUCCESS        ;werkt ook zonder?   ; VxD_SUCCESS equ 1
		clc                             ;no error - continue
		ret

EndProc VMYXD_Sys_Dynamic_Device_Init

VxD_ICODE_ENDS

;==============================================================================
VxD_CODE_SEG
VxD_CODE_ENDS

;==============================================================================
VxD_LOCKED_CODE_SEG

;BeginProc VMYXD_Control
;
; The Big 5 (static and dynamic)
;Control_Dispatch  Sys_Critical_Init, VMYXD_Sys_Critical_Init
;Control_Dispatch  Device_Init, VMYXD_Device_Init
;Control_Dispatch  Init_Complete, VMYXD_Init_Complete
;Control_Dispatch  Sys_Dynamic_Device_Init, VMYXD_Sys_Dynamic_Device_Init
;Control_Dispatch  Sys_Dynamic_Device_Exit, VMYXD_Sys_Dynamic_Device_Exit
;
;Control_Dispatch  W32_DeviceIoControl, VMYXD_W32_DeviceIoControl
;
;        	clc
;       	ret
;
;EndProc VMYXD_Control


Begin_Control_Dispatch VMYXD

; The Big 5 (static and dynamic)
Control_Dispatch Sys_Critical_Init, VMYXD_Sys_Critical_Init
Control_Dispatch Device_Init, VMYXD_Device_Init
Control_Dispatch Init_Complete, VMYXD_Init_Complete
Control_Dispatch Sys_Dynamic_Device_Init, VMYXD_Sys_Dynamic_Device_Init
Control_Dispatch Sys_Dynamic_Device_Exit, VMYXD_Sys_Dynamic_Device_Exit

; The procedure which handles DeviceIoControl() calls coming from apps
Control_Dispatch W32_DeviceIoControl, VMYXD_W32_DeviceIoControl

End_Control_Dispatch VMYXD


;------------------------------------------------------------------------------
BeginProc VMYXD_Sys_Dynamic_Device_Exit

		clc                             ;no error - continue
		ret

EndProc VMYXD_Sys_Dynamic_Device_Exit


;------------------------------------------------------------------------------
; This procedure is the starting point for all DeviceIoControl() calls from the Win32 App.
; EBX contains the handle of the VM in which the calling Win32 app executes.
; ESI contains the address of a DIOCParams structure. We should examine
; the dwIoControlCode member of the DIOCParams structure to determine the
; action to perform.
; Voor dynaloads zijn DIOC_OPEN and DIOC_CLOSEHANDLE standaard waardes voor dwIoControlCode.
; Andere waardes bepalen we zelf.
;
BeginProc VMYXD_W32_DeviceIoControl

;		myDIOCParams DIOCParams <>
;
;		mov	eax, [esi+0]
;		mov	myDIOCParams.Internal1, eax
;		mov	eax, [esi+4]
;		mov	myDIOCParams.VMHandle, eax
;		mov	eax, [esi+8]
;		mov	myDIOCParams.Internal2, eax
;		mov	eax, [esi+12]
;		mov	myDIOCParams.dwIoControlCode, eax
;		mov	eax, [esi+16]
;		mov	myDIOCParams.lpvInBuffer, eax
;		mov	eax, [esi+20]
;		mov	myDIOCParams.cbInBuffer, eax
;		mov	eax, [esi+24]
;		mov	myDIOCParams.lpvOutBuffer, eax
;		mov	eax, [esi+28]
;		mov	myDIOCParams.cbOutBuffer, eax
;		mov	eax, [esi+32]
;		mov	myDIOCParams.lpcbBytesReturned, eax
;		mov	eax, [esi+36]
;		mov	myDIOCParams.lpoOverlapped, eax
;		mov	eax, [esi+40]
;		mov	myDIOCParams.hDevice, eax
;		mov	eax, [esi+44]
;		mov	myDIOCParams.tagProcess, eax
;
;		
		cmp	[esi+12], DIOC_OPEN
		je	Support	

;		;mov	eax, PulseLength
;		mov	eax, 777				; test value
;		mov	[myDIOCParams.lpvOutBuffer], eax	; put Pulse Length in OutBuffer for use by Win32 App
;		mov	myDIOCParams.cbOutBuffer, 4

		mov	edx, [esi+24]
		mov	[edx], 777
		mov	[esi+28], 4	 


Support:	mov 	eax, 0                  ; return EAX = 0 to report that we support DeviceIOCTL
		clc                             ; no error - continue
		ret

EndProc VMYXD_W32_DeviceIoControl


;------------------------------------------------------------------------------
; TODO: check interrupts enabled on par. port
;	check interrupts enabled on PIC
; 	check are we sending an EOI
BeginProc VMYXD_Hw_Int

		VxDcall VTD_Get_Real_Time	; time since Windows start in edx:eax
		
		cmp	GotIntTime, 1
		je	StartCompare
		mov 	PrevIntTime, eax	; time stamp first interrupt (we are only interested in eax)
		mov	GotIntTime, 1
		jmp	Exit

; Note: compare is different from DOS because timer in EAX goes from 0000h to FFFFh, not backwards				

StartCompare:	mov	ebx, PrevIntTime
		mov	PrevIntTime, eax	; save present int time for next compare
		sub	eax, ebx		; subtract previous interrupt time from present one 
		mov	PulseLength, eax	;  to get the pulse length

		
		cmp	PulseLength, 0		; als timer van FFFFh naar 0000h is gegaan krijgen we een
		jge	Positive		;  grote negatieve waarde
		add	PulseLength, 65536	; maak waarde correct
			
		
Positive:	cmp	PulseLength, 15000 	; is it the sync pulse?
		ja	Sync
		cmp	GotSync, 1		; if we have the first sync go to channels
		je	Channels
		jmp	Exit			; not sync, exit and try again
		
Sync:		mov 	eax, PulseLength
		mov     SyncPulse, eax
		mov	GotSync, 1
		mov	ChannelCount, 0
		jmp	Exit
		
Channels:	inc	ChannelCount
		cmp	ChannelCount, 1
		je	Chan1
		cmp	ChannelCount, 2
		je	Chan2
		cmp	ChannelCount, 3
		je	Chan3
		cmp	ChannelCount, 4
		je	Chan4
		cmp	ChannelCount, 5
		je	Chan5
		cmp	ChannelCount, 6
		je	Chan6
		cmp	ChannelCount, 7
		je	Chan7
		cmp	ChannelCount, 8
		je	Chan8
		cmp	ChannelCount, 9
		je	Chan9		
		cmp	ChannelCount, 10
		je	Chan10
		cmp	ChannelCount, 11
		jae	Exit

Chan1:		mov 	eax, PulseLength
		mov     Ch1, eax		
		jmp	Exit
Chan2:		mov 	eax, PulseLength
		mov     Ch2, eax
		jmp	Exit
Chan3:		mov 	eax, PulseLength
		mov     Ch3, eax
		jmp	Exit
Chan4:		mov 	eax, PulseLength
		mov     Ch4, eax
		jmp	Exit
Chan5:		mov 	eax, PulseLength
		mov     Ch5, eax
		jmp	Exit
Chan6:		mov 	eax, PulseLength
		mov     Ch6, eax
		jmp	Exit
Chan7:		mov 	eax, PulseLength
		mov     Ch7, eax
		jmp	Exit
Chan8:		mov 	eax, PulseLength
		mov     Ch8, eax
		jmp	Exit
Chan9:		mov 	eax, PulseLength
		mov     Ch9, eax
		jmp	Exit
Chan10:		mov 	eax, PulseLength
		mov     Ch10, eax
		jmp	Exit


Exit:		clc                             ;no error - continue
		ret

EndProc VMYXD_Hw_Int

BeginProc VMYXD_EOI

		clc                             ;no error - continue
		ret

EndProc VMYXD_EOI

;------------------------------------------------------------------------------
; A vxd API proc is called as follows:
;
; An application or other software running in a virtual machine retrieves an entry-point address
; for an API procedure for a given virtual machine by calling the Get Device Entry Point Address function
; (Interrupt 2Fh Function 1684h) and setting the BX register to the identifier for the virtual device.
; The VMM returns an address to the application to indirectly enter the API procedure. 
BeginProc VMYXD_API_Proc

		mov 	eax, 0                  ;return 0 in EAX == success (Note: this is not the return value of DeviceIoControl())
		clc                             ;no error - continue
		ret

EndProc VMYXD_API_Proc

VxD_LOCKED_CODE_ENDS


		END
