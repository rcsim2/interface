PAGE 58,132
TITLE VMYXD - Transmitter VxD

		.386p

		.XLIST
		INCLUDE vmm.inc
		INCLUDE debug.inc
		INCLUDE vpicd.inc
		INCLUDE vtd.inc
		INCLUDE vwin32.inc
		INCLUDE shell.inc
		.LIST

Declare_Virtual_Device VMYXD, 1, 0, VMYXD_Control, Undefined_Device_ID,,
;Declare_Virtual_Device VMYXD, 1, 0, VMYXD_Control, Undefined_Device_ID,\
;						VMYXD_API_Proc, VMYXD_API_Proc

;==============================================================================
VxD_IDATA_SEG

; IRQ descriptor used to virtualize the LPT IRQ 7

;VMYXD_IRQ_Desc VPICD_IRQ_Descriptor <7,,OFFSET32 VMYXD_Hw_Int,,\
;						OFFSET32 VMYXD_EOI>
;VMYXD_IRQ_Desc VPICD_IRQ_Descriptor <7, VPICD_OPT_CAN_SHARE, OFFSET32 VMYXD_Hw_Int,,\
;						OFFSET32 VMYXD_EOI>
;VMYXD_IRQ_Desc VPICD_IRQ_Descriptor <7, VPICD_OPT_READ_HW_IRR, OFFSET32 VMYXD_Hw_Int,,\
;						OFFSET32 VMYXD_EOI>
VMYXD_IRQ_Desc VPICD_IRQ_Descriptor <7, VPICD_OPT_CAN_SHARE, OFFSET32 VMYXD_Hw_Int,\
						0,0,0,0,500,0>

VxD_IDATA_ENDS

;==============================================================================
VxD_DATA_SEG
VxD_DATA_ENDS

;==============================================================================
VxD_LOCKED_DATA_SEG

; Use LF only for ugly white message box. CR LF for nice gray box.
;Message	db	"Hi, mom!",10,"This is a message from my VXD.", 0
Message		db	"Hi, mom!",13,10,"This is a message from my VXD.    ", 0
Message2	db	"Hello!",13,10,"This is message 2.                  ", 0
Message3	db	"Hello!",13,10,"This is message 3.                  ", 0

Caption		db	"Blacksphere VxD Message", 0

OutBuf		db	256 DUP (?)
;Format		db	"This is a formatted string. Interrupt Count: %ld   ", 0
Format		db	"Device Exit. Interrupt Count: %ld    ", 0
Format2		db	"Error Number: %ld                    ", 0
Format3		db	"This one takes two dwords: %ld %ld   ", 0

myDIOCParams 	DIOCParams <>

ErrorNumber	dword	0

IRQHandle	dword	0
myVMHandle	dword	0

IntCount	dword	0
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

		clc                             ; no error - continue
		ret

EndProc VMYXD_Sys_Critical_Init


BeginProc VMYXD_Device_Init

		clc                             ; no error - continue
		ret

EndProc VMYXD_Device_Init


BeginProc VMYXD_Init_Complete

		clc                             ; no error - continue
		ret

EndProc VMYXD_Init_Complete

;------------------------------------------------------------------------------
; Start me up
BeginProc VMYXD_Sys_Dynamic_Device_Init

; Note: de eerste maal dat we de vxd laden lukt VPICD_Virtualize_IRQ
; Daarna krijgen we een error omdat de IRQ al is gevirtualizeerd...
; Oplossing: unvirtualize in Device_Exit: VPICD_Force_Default_Behavior

		mov 	edi, OFFSET32 VMYXD_IRQ_Desc
		VxDCall VPICD_Virtualize_IRQ
		jc	ErrorHandler		; carry set means error
		mov     IRQHandle, eax		; save IRQHandle
		
; This doesn't seem necessary...
; Seems like VPICD_Virtualize_IRQ has already taken care off enabling IRQ on the PIC 

		; Enable IRQ 7 on PIC???
		mov     eax, IRQHandle
		VxDCall VPICD_Physically_Unmask
		
; This is absolutely NECESSARY!!!!!!!!!!
; When we start our DOS app before this VXD, the DOS app will have enabled the IRQ.
; If we then start this VXD the IRQ will already be enabled
; But when we start the VXD first without enabling we'll get no interrupts
; Note that we can use the normal port addresses: no virtualization there.
; Shouldn't hard-code of course...

		; Enable IRQ on Par. Port!!!!!!!!!!
		mov	dx, 378h+2		; got to use dx for ports > 255 (dec)
		in	al, dx			; read par. port Control register (378h + 2)
		or 	al, 10h			; enable interrupts on line 10 (ACK) (set bit 4)
		out	dx, al			; write back to Control register
		
		jmp	Success


ErrorHandler:	mov	ErrorNumber, 1		; Hell, cannot virtualize IRQ 7
		
; Messagebox with formatted string
		VMMCall _Sprintf, <OFFSET32 OutBuf, OFFSET32 Format2, ErrorNumber>
		
		mov	eax, MB_ICONASTERISK
		mov 	ecx, OFFSET32 OutBuf
		mov 	edi, OFFSET32 Caption
		xor     esi, esi                ; no callback
		VxDCall SHELL_Message


Success:	mov 	eax, VxD_SUCCESS        ; werkt ook zonder?   ; VxD_SUCCESS equ 1
		clc                             ; no error - continue
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
; Clean up time
BeginProc VMYXD_Sys_Dynamic_Device_Exit

; This allows us to show a dirty white old ugly messagebox
; When using SHELL_SYSMODAL_Message we'll get a blue screen showing the message
; Also with MB_NOWINDOW
; We'll get a nice proper gray messagebox with bitwise-OR: MB_OK OR MB_ICONASTERISK
; Or with MB_ICONASTERISK alone.
; MB_OK alone gives the dirty white...
; Note: must zero out ESI if no callback!!!

;		mov	ebx, myVMHandle			; virtual machine handle (can be zero or anything)
;		;mov	eax, MB_OK			; message box flags
;		;mov	eax, MB_ICONASTERISK
;		mov	eax, MB_OK OR MB_ICONEXCLAMATION
;		mov 	ecx, OFFSET32 Message  		; address of message text
;		mov 	edi, OFFSET32 Caption   	; address of caption text
;		;xor     edi, edi			; default caption
;		xor     esi, esi                        ; no callback MUST be zero!!!  
;		;mov 	esi, OFFSET32 Callback  	; address of callback
;		;mov 	edx, ReferenceData      	; reference data for callback
;		VxDCall SHELL_Message

;		mov	eax, MB_ICONASTERISK
;		mov 	ecx, OFFSET32 Message
;		mov 	edi, OFFSET32 Caption
;		xor     esi, esi                        ; no callback
;		;VxDCall SHELL_Message
		
; Messagebox with formatted string
		VMMCall _Sprintf, <OFFSET32 OutBuf, OFFSET32 Format, IntCount>
		
		mov	eax, MB_ICONASTERISK
		mov 	ecx, OFFSET32 OutBuf
		mov 	edi, OFFSET32 Caption
		xor     esi, esi                        ; no callback
		VxDCall SHELL_Message


; Got to check if VPICD_Get_Virtualization_Count == 1 before doing this...
		
;		mov     eax, IRQHandle
;		VxDCall VPICD_Physically_Mask

; Unvirtualize IRQ (i.e. unhook our interrupt handler)

;		mov     eax, IRQHandle
;		VxDCall VPICD_Force_Default_Behavior

		clc                             	; no error - continue
		ret

EndProc VMYXD_Sys_Dynamic_Device_Exit


;------------------------------------------------------------------------------
; This procedure is the starting point for all DeviceIoControl() calls from the Win32 App.
; 
; EBX contains the handle of the VM in which the calling Win32 app executes (this is the DDB)
; ESI contains the address of a DIOCParams structure
; ECX dwIoControlCode
; EDX hDevice
;
; We should examine the dwIoControlCode member of the DIOCParams structure to determine the
; action to perform.
; Voor dynaloads zijn DIOC_OPEN and DIOC_CLOSEHANDLE standaard waardes voor dwIoControlCode.
; Andere waardes bepalen we zelf.
;
BeginProc VMYXD_W32_DeviceIoControl

; Store VMHandle
		mov	myVMHandle, ebx

; Process standard Control Codes!!!!!!!!!!!
; Check standard dwIoControlCode
		
		cmp	[esi+12], DIOC_OPEN
		je	Support
		cmp	[esi+12], DIOC_CLOSEHANDLE	; forget this caused the [ebx+35] bug
		je	Support
		
; Process your own dwIoControlCode below


; WRONG: variable in a CODE section!!! You dickwad.
; Put it in DATA section
;
;		myDIOCParams DIOCParams <>

		mov	eax, [esi+0]
		mov	myDIOCParams.Internal1, eax
		mov	eax, [esi+4]
		mov	myDIOCParams.VMHandle, eax
		mov	eax, [esi+8]
		mov	myDIOCParams.Internal2, eax

		mov	eax, [esi+12]
		mov	myDIOCParams.dwIoControlCode, eax

		mov	eax, [esi+16]
		mov	myDIOCParams.lpvInBuffer, eax
		mov	eax, [esi+20]
		mov	myDIOCParams.cbInBuffer, eax

		mov	eax, [esi+24]
		mov	myDIOCParams.lpvOutBuffer, eax
		mov	eax, [esi+28]
		mov	myDIOCParams.cbOutBuffer, eax

		mov	eax, [esi+32]
		mov	myDIOCParams.lpcbBytesReturned, eax
		mov	eax, [esi+36]
		mov	myDIOCParams.lpoOverlapped, eax
		mov	eax, [esi+40]
		mov	myDIOCParams.hDevice, eax
		mov	eax, [esi+44]
		mov	myDIOCParams.tagProcess, eax

		
; Why does this not work?
; Because myDIOCParams was created in a CODE section
;		;mov	eax, IntCount
;		mov	eax, 777				; test value
;		mov	[myDIOCParams.lpvOutBuffer], eax	; put IntCount in OutBuffer for use by Win32 App
;		mov	myDIOCParams.cbOutBuffer, 4
		

; Whereas this works fine!
; Hell, output buffer has prefered size of 4 bytes or less???
;
; If an output buffer is given (meaning non-null address and non-zero size),
; then the last byte in the buffer and the first byte in the buffer and every byte
; at multiples of 1000h bytes into the buffer must be writable.
;
; Is this dangerous? We are using ebx to point to a memory location. But there is 
; also a ISR running which might change ebx. If so this code may thrash memory...
; Better use a variable instead of a register.
; Is this a race condition in progress?
;
; Nee, want als we ISR niet toepassen crashen we nog steeds op [ebx+32] (i.e. [DIOCParams.lpvOutBuffer+32])


		mov	ebx, [esi+24]		; EBX = DIOCParams.lpvOutBuffer
		
		mov	eax, SyncPulse
		mov	[ebx+0], eax
		mov	eax, Ch1
		mov	[ebx+4], eax
		mov	eax, Ch2
		mov	[ebx+8], eax
		mov	eax, Ch3
		mov	[ebx+12], eax
		mov	eax, Ch4
		mov	[ebx+16], eax
		mov	eax, Ch5
		mov	[ebx+20], eax
		mov	eax, Ch6
		mov	[ebx+24], eax
		mov	eax, Ch7
		mov	[ebx+28], eax

; Phase of the Moon...
; Zit hier na 8 dwords gevoelig geheugen???
; En wel op [ebx+35]!!!!!!!!!!!!!!		
		mov	eax, Ch8
		mov	[ebx+32], 666

;		mov	al, 69
;		mov	[ebx+35], al	; Nuke me!!!

		mov	eax, Ch9
		mov	[ebx+36], eax
		mov	eax, Ch10
		mov	[ebx+40], eax		
		mov	eax, IntCount
		mov	[ebx+44], eax
		mov	eax, PulseLength
		mov	[ebx+48], eax



; Dit mag niet!!?? Je bent hier aan het schrijven over
; info die aan de VXD gegeven is via DeviceIoControl()
; De VXD hoeft en mag hier zelf geen info in zetten!!!???
 
		;mov	myDIOCParams.cbOutBuffer, 52		; myDIOCParams.cbOutBuffer = 13 dwords * 4 = 52 bytes
		
; Wat je wel aan de app moet returnen is cbBytesReturned!!!
		
		mov	ebx, [esi+32]		; EBX = DIOCParams.lpcbBytesReturned
		mov	[ebx], 52

		
Support:	mov 	eax, 0                  ; return EAX = 0 to report that we support DeviceIOCTL
		clc                             ; no error - continue
		ret

EndProc VMYXD_W32_DeviceIoControl


;------------------------------------------------------------------------------
; This is our IRQ7 interrupt handler
;
; TODO: Check interrupts enabled on par. port
;	Check IRQ 7 enabled on PIC
; 	Check are we sending an EOI
BeginProc VMYXD_Hw_Int

		inc	IntCount
		
		VxDCall VTD_Get_Real_Time	; time since Windows start in EDX:EAX (increments every 0.8 uS)
		
		cmp	GotIntTime, 1
		je	StartCompare
		mov 	PrevIntTime, eax	; time stamp first interrupt (we are only interested in EAX)
		mov	GotIntTime, 1
		jmp	Exit

; Note: compare is different from DOS because timer in EAX goes from 00000000h to FFFFFFFFh, not backwards
; and it fills a dword				

StartCompare:	mov	ebx, PrevIntTime
		mov	PrevIntTime, eax	; save present int time for next compare
		sub	eax, ebx		; subtract previous interrupt time from present one 
		mov	PulseLength, eax	;  to get the pulse length

		
		cmp	PulseLength, 0		; als timer van FFFFFFFFh naar 00000000h is gegaan krijgen we een
		jge	Positive		;  grote negatieve waarde
		add	PulseLength, 0FFFFFFFFh	; maak waarde correct
			
		
Positive:	cmp	PulseLength, 7000 	; is it the sync pulse?
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
		ja	Exit

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
		
; Hell, we were jumping over EOI

Exit:		
		mov     eax, IRQHandle
		VxDCall VPICD_Phys_EOI		; send EOI

		clc                             ; no error - continue
		ret

EndProc VMYXD_Hw_Int

;------------------------------------------------------------------------------
BeginProc VMYXD_EOI

		mov	IntCount, 999

		mov     eax, IRQHandle
		VxDCall VPICD_Physically_Unmask
		VxDCall VPICD_Phys_EOI		; send EOI

		clc                             ; no error - continue
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
		clc                             ; no error - continue
		ret

EndProc VMYXD_API_Proc

VxD_LOCKED_CODE_ENDS


		END
