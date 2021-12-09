;****************************************************************************
;									    *
; THIS CODE AND	INFORMATION IS PROVIDED	"AS IS"	WITHOUT	WARRANTY OF ANY	    *
; KIND,	EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE	    *
; IMPLIED WARRANTIES OF	MERCHANTABILITY	AND/OR FITNESS FOR A PARTICULAR	    *
; PURPOSE.								    *
;									    *
; Copyright (C)	1993-95	 Microsoft Corporation.	 All Rights Reserved.	    *
;									    *
;****************************************************************************

PAGE 58,132
;******************************************************************************
TITLE GENERIC -	Generic	VxD
;******************************************************************************
;
;   Title:	GENERIC.ASM - Generic VxD
;
;   Version:	1.00
;
;==============================================================================

	.386p

;******************************************************************************
;			      I	N C L U	D E S
;******************************************************************************

	.XLIST
	INCLUDE	VMM.Inc
	INCLUDE	Debug.Inc
	.LIST


;******************************************************************************
;	       V I R T U A L   D E V I C E   D E C L A R A T I O N
;------------------------------------------------------------------------------
; The VxD declaration statement	defines	the VxD	name, version number,
; control proc entry point, VxD	ID, initialization order, and VM API 
; entry	points.	What follows is	a minimal VxD declaration, defining 
; only the name, version, control proc,	and an undefined ID. Depending
; on the requirements of the VxD, the following	may be added:
;
; - Defined VxD	ID: See	VXDID.TXT for more information
; - Init order:	If your	Vxd MUST load before or	after a	specific VxD,
;		the init order can be defined. See VMM.INC for the
;		definition of the init order of	the standard devices.
; - V86,PM API:	You may	wish to	allow application or library code running
;		in a virtual machine to	communicate with your VxD directly.
;		See the	chapter	entitled "VxD APIs (Call-Ins)" in the
;		Virtual	Device Adaptation Guide.
;		
;******************************************************************************

Declare_Virtual_Device VMYXD, 1, 0, VMYXD_Control, Undefined_Device_ID ,,,


;******************************************************************************
;				 D A T A
;******************************************************************************

VXD_IDATA_SEG
;	Initialization data here - discarded after Init_Complete
VXD_IDATA_ENDS


VXD_DATA_SEG
;	Normal Data here
VXD_DATA_ENDS


VXD_LOCKED_DATA_SEG
;	Pagelocked data	here - try to keep this	to a minimum.
VXD_LOCKED_DATA_ENDS



;******************************************************************************
;		   I N I T I A L I Z A T I O N	 C O D E
;------------------------------------------------------------------------------
;    Code in the initialization	segment	is discarded after Init_Complete
;******************************************************************************

VXD_ICODE_SEG

;******************************************************************************
;
;   VXD_Device_Init
;
;   DESCRIPTION:
;	This is	a shell	for a routine that is called at	system BOOT.
;	Typically, a VxD would do its initialization in	this routine.
;
;   ENTRY:
;	EBX = System VM	handle
;
;   EXIT:
;	Carry clear to indicate	load success
;	Carry set to abort loading this	VxD
;
;   USES:
;	flags
;
;==============================================================================

;BeginProc VMYXD_Device_Init
;
;	clc				;no error - load VxD 
;	 ret
;
;EndProc VMYXD_Device_Init


VXD_ICODE_ENDS



;******************************************************************************
;				C O D E
;------------------------------------------------------------------------------
; The 'body' of	the VxD	would typically	be in the standard code	segment.
;******************************************************************************

VXD_LOCKED_CODE_SEG


;******************************************************************************
;
;   VXD_Create_VM
;
;   DESCRIPTION:
;	This is	a shell	for a routine that is called when a virtual
;	machine	is created. A typical VxD might	perform	some action
;	here to	prepare	for handling the new VM	in the system.
;
;   ENTRY:
;	EBX = VM handle
;
;   EXIT:
;	Carry clear to continue	creating VM
;	Carry set to abort the creation	of the VM
;
;   USES:
;	flags
;
;==============================================================================

;BeginProc VMYXD_Create_VM
;
;	clc				;no error - continue
;	 ret
;
;EndProc VMYXD_Create_VM


BeginProc VMYXD_Sys_Critical_Init

	clc				;no error - continue
	ret

EndProc	VMYXD_Sys_Critical_Init


BeginProc VMYXD_Device_Init

	clc				;no error - continue
	ret

EndProc	VMYXD_Device_Init


BeginProc VMYXD_Init_Complete

	clc				;no error - continue
	ret

EndProc	VMYXD_Init_Complete



BeginProc VMYXD_Sys_Dynamic_Device_Exit

	clc				;no error - continue
	ret

EndProc	VMYXD_Sys_Dynamic_Device_Exit


BeginProc VMYXD_W32_DeviceIoControl

	mov eax, 0	;werkt ook zonder
	clc				;no error - continue
	ret

EndProc	VMYXD_W32_DeviceIoControl



VXD_LOCKED_CODE_ENDS


;******************************************************************************
;		   I N I T I A L I Z A T I O N	 C O D E
;******************************************************************************

VxD_ICODE_SEG

BeginProc VMYXD_Sys_Dynamic_Device_Init

	mov eax, VXD_SUCCESS	;werkt ook zonder	; VXD_SUCCESS equ 1
	clc				;no error - continue
	ret

EndProc	VMYXD_Sys_Dynamic_Device_Init

VxD_ICODE_ENDS



;******************************************************************************
;		       P A G E	 L O C K E D   C O D E
;------------------------------------------------------------------------------
;	Memory is a scarce resource. Use this only where necessary.
;******************************************************************************
VXD_LOCKED_CODE_SEG

;******************************************************************************
;
;   VXD_Control
;
;   DESCRIPTION:
;
;	This is	a call-back routine to handle the messages that	are sent
;	to VxD's to control system operation. Every VxD	needs this function
;	regardless if messages are processed or	not. The control proc must
;	be in the LOCKED code segment.
;
;	The Control_Dispatch macro used	in this	procedure simplifies
;	the handling of	messages. To handle a particular message, add
;	a Control_Dispatch statement with the message name, followed
;	by the procedure that should handle the	message. 
;
;	The two	messages handled in this sample	control	proc, Device_Init
;	and Create_VM, are done	only to	illustrate how messages	are
;	typically handled by a VxD. A VxD is not required to handle any
;	messages.
;
;   ENTRY:
;	EAX = Message number
;	EBX = VM Handle
;
;==============================================================================

;BeginProc VMYXD_Control
;
;	 Control_Dispatch Device_Init, VMYXD_Device_Init
;	 Control_Dispatch Create_VM, VMYXD_Create_VM
;	 clc
;	 ret
;
;EndProc VMYXD_Control

Begin_Control_Dispatch VMYXD
;Control_Dispatch  Device_Init,	VMYXD_Device_Init
;Control_Dispatch  Create_VM, VMYXD_Create_VM

; The Big 5 (static and	dynamic)
Control_Dispatch  Sys_Critical_Init, VMYXD_Sys_Critical_Init
Control_Dispatch  Device_Init, VMYXD_Device_Init
Control_Dispatch  Init_Complete, VMYXD_Init_Complete
Control_Dispatch  Sys_Dynamic_Device_Init, VMYXD_Sys_Dynamic_Device_Init
Control_Dispatch  Sys_Dynamic_Device_Exit, VMYXD_Sys_Dynamic_Device_Exit

Control_Dispatch  W32_DeviceIoControl, VMYXD_W32_DeviceIoControl

End_Control_Dispatch VMYXD

VXD_LOCKED_CODE_ENDS



;******************************************************************************
;			R E A L	  M O D	E   C O	D E
;******************************************************************************

;******************************************************************************
;
;	Real mode initialization code
;
;   DESCRIPTION:
;	This code is called when the system is still in	real mode, and
;	the VxDs are being loaded.
;
;	This routine as	coded shows how	a VxD (with a defined VxD ID)
;	could check to see if it was being loaded twice, and abort the 
;	second without an error	message. Note that this	would require
;	that the VxD have an ID	other than Undefined_Device_ID.	See
;	the file VXDID.TXT more	details.
;
;   ENTRY:
;	AX = VMM Version
;	BX = Flags
;		Bit 0: duplicate device	ID already loaded 
;		Bit 1: duplicate ID was	from the INT 2F	device list
;		Bit 2: this device is from the INT 2F device list
;	EDX = Reference	data from INT 2F response, or 0
;	SI = Environment segment, passed from MS-DOS
;
;   EXIT:
;	BX = ptr to list of pages to exclude (0, if none)
;	SI = ptr to list of instance data items	(0, if none)
;	EDX = DWORD of reference data to be passed to protect mode init
;
;==============================================================================
;
;VXD_REAL_INIT_SEG
;
;BeginProc VXD_Real_Init_Proc
;
;	test	bx, Duplicate_Device_ID	; check	for already loaded
;	jnz	short duplicate		; jump if so
;
;	xor	bx, bx			; no exclusion table
;	xor	si, si			; no instance data table
;	xor	edx, edx		; no reference data
;
;	mov	ax, Device_Load_Ok
;	ret
;
;duplicate:
;	mov	ax, Abort_Device_Load +	No_Fail_Message
;	ret
;
;EndProc VXD_Real_Init_Proc
;
;
;VXD_REAL_INIT_ENDS


	END
