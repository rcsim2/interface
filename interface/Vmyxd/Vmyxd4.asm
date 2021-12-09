PAGE 58,132
TITLE VMYXD - Transmitter VxD

		.386p

		.XLIST
		INCLUDE vmm.inc
		INCLUDE debug.inc
		INCLUDE vpicd.inc
		INCLUDE vtd.inc
		.LIST

Declare_Virtual_Device VMYXD, 1, 0, VMYXD_Control, Undefined_Device_ID,,\
						VMYXD_PMProc

;==============================================================================
VxD_IDATA_SEG

VMYXD_IRQ_Desc VPICD_IRQ_Descriptor <7,,OFFSET32 VMYXD_Hw_Int,,\
						OFFSET32 VMYXD_EOI>
VxD_IDATA_ENDS

;==============================================================================
VxD_DATA_SEG
VxD_DATA_ENDS

;==============================================================================
VxD_LOCKED_DATA_SEG

PrevIntTime	dword   0
GotIntTime      dword   0
PulseLength     dword   0
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

Control_Dispatch W32_DeviceIoControl, VMYXD_W32_DeviceIoControl

End_Control_Dispatch VMYXD


;------------------------------------------------------------------------------
BeginProc VMYXD_Sys_Dynamic_Device_Exit

		clc                             ;no error - continue
		ret

EndProc VMYXD_Sys_Dynamic_Device_Exit


BeginProc VMYXD_W32_DeviceIoControl

		mov 	eax, 0                  ;report that we support IOCTL - werkt ook zonder?
		clc                             ;no error - continue
		ret

EndProc VMYXD_W32_DeviceIoControl


;------------------------------------------------------------------------------
BeginProc VMYXD_Hw_Int

		VxDcall VTD_Get_Real_Time
		
		cmp	GotIntTime, 1
		je	StartCompare
		mov 	PrevIntTime, eax	; time stamp first interrupt (we are only interested in EAX)
		mov	GotIntTime, 1
		jmp	Exit

StartCompare:	mov	ebx, PrevIntTime
		sub	ebx, eax		; subtract present interrupt time from previous one
		mov	PulseLength, ebx	;  to get the pulse length
		mov	PrevIntTime, eax	; save present int time
		
		cmp	PulseLength, 0		; als timer van 0000h naar FFFFh is gegaan krijgen we een
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
; This procedure handles API calls made from protected mode programs.
; ESI contains the address of a DIOCParams structure. We should examine
; the dwIoControlCode member of the DIOCParams structure to determine the
; action to perform (i.e. what procedure to call). If we only need one API
; procedure we can dispense with checking dwIoControlCode and calling
; other procedures: we can handle everything in here.  
BeginProc VMYXD_PMProc

		mov 	eax, 0                  ;report success
		clc                             ;no error - continue
		ret

EndProc VMYXD_PMProc

VxD_LOCKED_CODE_ENDS


		END
