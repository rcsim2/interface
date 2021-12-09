; This program measures the latency of an INT 08 ISR.
; It works by reading the timer chip immediately upon entering
; the INT 08 ISR  By averaging this value for some number of
; executions, we can determine the average latency for this
; code.


		.xlist
		.386
		option		segment:use16
		include 	stdlib.a
		includelib	stdlib.lib
		.list


cseg		segment	para public 'code'
		assume	cs:cseg, ds:nothing

; All the variables are in the code segment in order to reduce ISR
; latency (we don't have to push and set up DS, saving a few instructions
; at the beginning of the ISR).

IntVectSave	dword	?
SumLatency	dword	0
Executions	dword	0
Average		dword	0
PrevIntTime	dword   0
GotIntTime	dword   0
PulseLength	dword	0
GotSync		dword	0
ChannelCount	dword	0
SyncPulse	dword   0
Ch1		dword	0
Ch2		dword	0
Ch3		dword	0
Ch4		dword	0
Ch5		dword	0
Ch6		dword	0
Ch7		dword	0
Ch8		dword	0
Ch9		dword	0
Ch10		dword	0

M1	word	0
M2	word	0
M3	word	0
M4	word	0
M5	word	0
M6	word	0
M7	word	0
M8	word	0
M9	word	0
M10	word	0


L0	db	176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176, 0
L1	db	219,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176, 0
L2	db	219,219,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176, 0
L3	db	219,219,219,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176, 0
L4	db	219,219,219,219,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176, 0
L5	db	219,219,219,219,219,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176, 0
L6	db	219,219,219,219,219,219,176,176,176,176,176,176,176,176,176,176,176,176,176,176, 0
L7	db	219,219,219,219,219,219,219,176,176,176,176,176,176,176,176,176,176,176,176,176, 0
L8	db	219,219,219,219,219,219,219,219,176,176,176,176,176,176,176,176,176,176,176,176, 0
L9	db	219,219,219,219,219,219,219,219,219,176,176,176,176,176,176,176,176,176,176,176, 0
L10	db	219,219,219,219,219,219,219,219,219,219,176,176,176,176,176,176,176,176,176,176, 0
L11	db	219,219,219,219,219,219,219,219,219,219,219,176,176,176,176,176,176,176,176,176, 0
L12	db	219,219,219,219,219,219,219,219,219,219,219,219,176,176,176,176,176,176,176,176, 0
L13	db	219,219,219,219,219,219,219,219,219,219,219,219,219,176,176,176,176,176,176,176, 0
L14	db	219,219,219,219,219,219,219,219,219,219,219,219,219,219,176,176,176,176,176,176, 0
L15	db	219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,176,176,176,176,176, 0
L16	db	219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,176,176,176,176, 0
L17	db	219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,176,176,176, 0
L18	db	219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,176,176, 0
L19	db	219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,176, 0
L20	db	219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219,219, 0




; This program reads the 8254 timer chip.  This chip counts from
; 0FFFFh down to zero and then generates an interrupt.  It wraps
; around from 0 to 0FFFFh and continues counting down once it
; generates the interrupt.
;
; 8254 Timer Chip port addresses:

Timer0_8254	equ	40h
Cntrl_8254	equ	43h

; Par. Port IRQ and port address
PORTADDRESS	equ	378h
IRQ		equ	 07h

DATAREG 	equ	PORTADDRESS+0
STATUSREG	equ	PORTADDRESS+1
CONTROLREG	equ	PORTADDRESS+2

INTNUMBER	equ	IRQ+08h




; The following ISR reads the 8254 timer chip, negates the result
; (because the timer counts backwards), adds the result to the
; SumLatency variable, and then increments the Executions variable
; that counts the number of times we execute this code.  In the
; mean time, the main program is busy computing and displaying the
; average latency time for this ISR.
;
; To read the 16 bit 8254 counter value, this code needs to
; write a zero to the 8254 control port and then read the
; timer port twice (reads the L.O. then H.O. bytes).  There
; needs to be a short delay between reading the two bytes
; from the same port address.

; We are using the par. port INT 0Fh to generate the interrupt
; which calls this ISR
; But why does the par. port not generate interrupts???
; That was because of the dratted old ISR not sending an EOI
; Now it works!!

NewISR		proc	near
		push	eax
		push	ebx
		
		mov	eax, 0		;Ch 0, latch & read data.
		out	Cntrl_8254, al	;Output to 8253 cmd register.
		in	al, Timer0_8254	;Read latch #0 (LSB) & ignore.
		mov	ah, al
		jmp	SettleDelay	;Settling delay for 8254 chip.
SettleDelay:	in	al, Timer0_8254	;Read latch #0 (MSB)
		xchg	ah, al
		;neg	ax		;Fix, 'cause timer counts down.
		;add    cseg:SumLatency, eax
		;inc	cseg:Executions
		
		cmp	cseg:GotIntTime, 1
		je	StartCompare
		mov     cseg:PrevIntTime, eax	; time stamp first interrupt
		mov	cseg:GotIntTime, 1

StartCompare:	mov	ebx, cseg:PrevIntTime
		sub	ebx, eax		; subtract present interrupt time from previous one
		mov	cseg:PulseLength, ebx	;  to get the pulse length
		mov	cseg:PrevIntTime, eax	; save present int time
		
		cmp	cseg:PulseLength, 0	; als timer van 0000h naar FFFFh is gegaan krijgen we een grote negatieve waarde
		jge	Positive
		add	cseg:PulseLength, 65536	; maak waarde correct
			
		
Positive:
		;printf				; print in ISR instead of in Main 
		;byte	"Pulse Length: %ld\n",0
		;dword	PulseLength
		
		
		;cmp	cseg:PulseLength, 15000 ; is it the sync pulse?
		sub	cseg:PulseLength, 500	; waarde een beetje aanpassen zodat we 1000<-1500->2000 uS hebben
		ror	cseg:PulseLength, 1	; deel door 2^1
		cmp	cseg:PulseLength, 8000	; sync pulse is ongeveer 11000
		ja	Sync
		cmp	cseg:GotSync, 1		; if we have the first sync go to channels
		je	Channels
		jmp	Exit			; not sync, exit and try again
		
Sync:		mov 	eax, cseg:PulseLength
		mov     cseg:SyncPulse, eax
		mov	cseg:GotSync, 1
		mov	cseg:ChannelCount, 0
		jmp	Exit
		
Channels:	inc	cseg:ChannelCount
		cmp	cseg:ChannelCount, 1
		je	Chan1
		cmp	cseg:ChannelCount, 2
		je	Chan2
		cmp	cseg:ChannelCount, 3
		je	Chan3
		cmp	cseg:ChannelCount, 4
		je	Chan4
		cmp	cseg:ChannelCount, 5
		je	Chan5
		cmp	cseg:ChannelCount, 6
		je	Chan6
		cmp	cseg:ChannelCount, 7
		je	Chan7
		cmp	cseg:ChannelCount, 8
		je	Chan8
		cmp	cseg:ChannelCount, 9
		je	Chan9		
		cmp	cseg:ChannelCount, 10
		je	Chan10
		cmp	cseg:ChannelCount, 11
		jae	Exit

Chan1:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch1, eax		
		jmp	Exit
Chan2:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch2, eax
		jmp	Exit
Chan3:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch3, eax
		jmp	Exit
Chan4:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch4, eax
		jmp	Exit
Chan5:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch5, eax
		jmp	Exit
Chan6:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch6, eax
		jmp	Exit
Chan7:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch7, eax
		jmp	Exit
Chan8:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch8, eax
		jmp	Exit
Chan9:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch9, eax
		jmp	Exit
Chan10:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch10, eax
		jmp	Exit


Exit:		mov al, 20h
		out 20h, al		; send EOI (end of interrupt)!!!!!!!!!!!!!!!
					; Hell, the old handler wasn't sending EOI
					; that's why we only got one interrupt
					; Cause is the old ISR which probably doesn't even
					; exist for the par. port

		; Forget about the old handler!!!!!!
		;jmp	cseg:OldISRh	;chains to the old interrupt
					; this will generate the IRET for you
					; but does not send an EOI !!!!!!!!!

		pop	ebx
		pop	eax					
		iret
NewISR		endp




ShortDelay	proc
		mov	cx, 0FFFFh
loopx:		loop	loopx
		mov	cx, 0FFFFh
loopx2:		loop	loopx2
		mov	cx, 0FFFFh
loopx3:		loop	loopx3
		mov	cx, 0FFFFh
loopx4:		loop	loopx4
		mov	cx, 0FFFFh
loopx5:		loop	loopx5
		ret
ShortDelay	endp



Main		proc
		meminit

; Begin by patching in the address of our ISR into int 8's vector.
; Note that we must turn off the interrupts while actually patching
; the interrupt vector and we must ensure that interrupts are turned
; back on afterwards;  hence the cli and sti instructions.  These are
; required because a timer interrupt could come along between the two
; instructions that write to the int 8 interrupt vector.  Since the
; interrupt vector is in an inconsistent state at that point, this
; could cause the system to crash.


; let's hook the parallel port interrupt: IRQ 7  INT 0Fh  I/O 378h
; Shouldn't hard code this...


		mov	ax, 0
		mov	es, ax
		mov	ax, es:[INTNUMBER*4]
		mov	word ptr IntVectSave, ax
		mov	ax, es:[INTNUMBER*4 + 2]
		mov	word ptr IntVectSave+2, ax

		cli
		mov	word ptr es:[INTNUMBER*4], offset NewISR
		mov	es:[INTNUMBER*4 + 2], cs
		sti

		
; Got to turn on IRQ 7 on the PIC (will probably be on already)
		in 	al, 21h 	;Read existing bits.
		;and 	al, 7Fh 	;Turn on IRQ 7 (clear bit 7) (don't hard code)
		mov	bl, 01h
		shl	bl, IRQ
		not	bl
		and	al, bl
		out 	21h, al 	;Write result back to PIC.
		
; And got to turn on interrupts on parallel port!!!!!!!!!!!!!
		mov	dx, CONTROLREG	; got to use dx for ports > 255 (dec)
		in	al, dx		; read par. port Control register (378h + 2)
		or 	al, 10h		; enable interrupts on line 10 (ACK) (set bit 4)
		out	dx, al		; write back to Control register



; This is only necessary when generating its with the data lines...
; We are using a data line e.g line 9 to output 5V directly to line 10
; so data register must be in output mode		
;		mov	dx, 37Ah	; got to use dx for ports > 255 (dec)
;		in	al, dx		; read par. port Control register (378h + 2)
;		and 	al, 0DFh	; make sure port is in forward direction (output only, no bi-direct.) (clear bit 5)
;		out	dx, al		; write back to Control register				
		


; First, wait for the first call to the ISR above.  Since we will be dividing
; by the value in the Executions variable, we need to make sure that it is
; greater than zero before we do anything.

;Wait4Non0:	cmp	cseg:Executions, 0
;		je	Wait4Non0



; Okay, start displaying the good values until the user presses a key at
; the keyboard to stop everything:

DisplayLp:	;int	0Fh

		; let's try to generate interrupts
		; Note: Connect a Data Line {2-9) to Line 10 on par. port
;		mov	dx, 378h
;		mov	al, 0FFh
;		out	dx, al
;
;		call	ShortDelay		
;		
;		mov	dx, 378h
;		mov	al, 00h
;		out	dx, al
		
		
		printf
		byte	"Blacksphere Utilities (C) 2000\n\n"
		byte	"Sync Pulse: %ld          \n\n",0
		dword	SyncPulse

		cmp	Ch1, 1000
		jle	Ch1Length0
		cmp	Ch1, 1050
		jle	Ch1Length1
		cmp	Ch1, 1100
		jle	Ch1Length2
		cmp	Ch1, 1150
		jle	Ch1Length3
		cmp	Ch1, 1200
		jle	Ch1Length4
		cmp	Ch1, 1250
		jle	Ch1Length5
		cmp	Ch1, 1300
		jle	Ch1Length6
		cmp	Ch1, 1350
		jle	Ch1Length7
		cmp	Ch1, 1400
		jle	Ch1Length8		
		cmp	Ch1, 1450
		jle	Ch1Length9
		cmp	Ch1, 1500
		jle	Ch1Length10
		cmp	Ch1, 1550
		jle	Ch1Length11
		cmp	Ch1, 1600
		jle	Ch1Length12
		cmp	Ch1, 1650
		jle	Ch1Length13
		cmp	Ch1, 1700
		jle	Ch1Length14
		cmp	Ch1, 1750
		jle	Ch1Length15
		cmp	Ch1, 1800
		jle	Ch1Length16
		cmp	Ch1, 1850
		jle	Ch1Length17
		cmp	Ch1, 1900
		jle	Ch1Length18
		cmp	Ch1, 1950
		jle	Ch1Length19
		cmp	Ch1, 6969
		jle	Ch1Length20
	
Ch1Length0:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L0,Ch1
		jmp	DoneCh1
Ch1Length1:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L1,Ch1
		jmp	DoneCh1
Ch1Length2:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L2,Ch1
		jmp	DoneCh1
Ch1Length3:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L3,Ch1
		jmp	DoneCh1
Ch1Length4:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L4,Ch1
		jmp	DoneCh1
Ch1Length5:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L5,Ch1
		jmp	DoneCh1
Ch1Length6:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L6,Ch1
		jmp	DoneCh1
Ch1Length7:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L7,Ch1
		jmp	DoneCh1
Ch1Length8:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L8,Ch1
		jmp	DoneCh1
Ch1Length9:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L9,Ch1
		jmp	DoneCh1
Ch1Length10:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L10,Ch1
		jmp	DoneCh1
Ch1Length11:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L11,Ch1
		jmp	DoneCh1
Ch1Length12:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L12,Ch1
		jmp	DoneCh1
Ch1Length13:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L13,Ch1
		jmp	DoneCh1
Ch1Length14:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L14,Ch1
		jmp	DoneCh1
Ch1Length15:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L15,Ch1
		jmp	DoneCh1
Ch1Length16:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L16,Ch1
		jmp	DoneCh1
Ch1Length17:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L17,Ch1
		jmp	DoneCh1
Ch1Length18:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L18,Ch1
		jmp	DoneCh1
Ch1Length19:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L19,Ch1
		jmp	DoneCh1
Ch1Length20:	printf
		byte	"Channel  1: %s %ld       \n\n",0
		dword	L20,Ch1
		jmp	DoneCh1

DoneCh1:


		cmp	Ch2, 1000
		jle	Ch2Length0
		cmp	Ch2, 1050
		jle	Ch2Length1
		cmp	Ch2, 1100
		jle	Ch2Length2
		cmp	Ch2, 1150
		jle	Ch2Length3
		cmp	Ch2, 1200
		jle	Ch2Length4
		cmp	Ch2, 1250
		jle	Ch2Length5
		cmp	Ch2, 1300
		jle	Ch2Length6
		cmp	Ch2, 1350
		jle	Ch2Length7
		cmp	Ch2, 1400
		jle	Ch2Length8		
		cmp	Ch2, 1450
		jle	Ch2Length9
		cmp	Ch2, 1500
		jle	Ch2Length10
		cmp	Ch2, 1550
		jle	Ch2Length11
		cmp	Ch2, 1600
		jle	Ch2Length12
		cmp	Ch2, 1650
		jle	Ch2Length13
		cmp	Ch2, 1700
		jle	Ch2Length14
		cmp	Ch2, 1750
		jle	Ch2Length15
		cmp	Ch2, 1800
		jle	Ch2Length16
		cmp	Ch2, 1850
		jle	Ch2Length17
		cmp	Ch2, 1900
		jle	Ch2Length18
		cmp	Ch2, 1950
		jle	Ch2Length19
		cmp	Ch2, 6969
		jle	Ch2Length20
	
Ch2Length0:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L0,Ch2
		jmp	DoneCh2
Ch2Length1:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L1,Ch2
		jmp	DoneCh2
Ch2Length2:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L2,Ch2
		jmp	DoneCh2
Ch2Length3:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L3,Ch2
		jmp	DoneCh2
Ch2Length4:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L4,Ch2
		jmp	DoneCh2
Ch2Length5:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L5,Ch2
		jmp	DoneCh2
Ch2Length6:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L6,Ch2
		jmp	DoneCh2
Ch2Length7:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L7,Ch2
		jmp	DoneCh2
Ch2Length8:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L8,Ch2
		jmp	DoneCh2
Ch2Length9:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L9,Ch2
		jmp	DoneCh2
Ch2Length10:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L10,Ch2
		jmp	DoneCh2
Ch2Length11:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L11,Ch2
		jmp	DoneCh2
Ch2Length12:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L12,Ch2
		jmp	DoneCh2
Ch2Length13:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L13,Ch2
		jmp	DoneCh2
Ch2Length14:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L14,Ch2
		jmp	DoneCh2
Ch2Length15:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L15,Ch2
		jmp	DoneCh2
Ch2Length16:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L16,Ch2
		jmp	DoneCh2
Ch2Length17:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L17,Ch2
		jmp	DoneCh2
Ch2Length18:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L18,Ch2
		jmp	DoneCh2
Ch2Length19:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L19,Ch2
		jmp	DoneCh2
Ch2Length20:	printf
		byte	"Channel  2: %s %ld       \n\n",0
		dword	L20,Ch2
		jmp	DoneCh2
		
DoneCh2:


		cmp	Ch3, 1000
		jle	Ch3Length0
		cmp	Ch3, 1050
		jle	Ch3Length1
		cmp	Ch3, 1100
		jle	Ch3Length2
		cmp	Ch3, 1150
		jle	Ch3Length3
		cmp	Ch3, 1200
		jle	Ch3Length4
		cmp	Ch3, 1250
		jle	Ch3Length5
		cmp	Ch3, 1300
		jle	Ch3Length6
		cmp	Ch3, 1350
		jle	Ch3Length7
		cmp	Ch3, 1400
		jle	Ch3Length8		
		cmp	Ch3, 1450
		jle	Ch3Length9
		cmp	Ch3, 1500
		jle	Ch3Length10
		cmp	Ch3, 1550
		jle	Ch3Length11
		cmp	Ch3, 1600
		jle	Ch3Length12
		cmp	Ch3, 1650
		jle	Ch3Length13
		cmp	Ch3, 1700
		jle	Ch3Length14
		cmp	Ch3, 1750
		jle	Ch3Length15
		cmp	Ch3, 1800
		jle	Ch3Length16
		cmp	Ch3, 1850
		jle	Ch3Length17
		cmp	Ch3, 1900
		jle	Ch3Length18
		cmp	Ch3, 1950
		jle	Ch3Length19
		cmp	Ch3, 6969
		jle	Ch3Length20
	
Ch3Length0:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L0,Ch3
		jmp	DoneCh3
Ch3Length1:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L1,Ch3
		jmp	DoneCh3
Ch3Length2:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L2,Ch3
		jmp	DoneCh3
Ch3Length3:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L3,Ch3
		jmp	DoneCh3
Ch3Length4:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L4,Ch3
		jmp	DoneCh3
Ch3Length5:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L5,Ch3
		jmp	DoneCh3
Ch3Length6:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L6,Ch3
		jmp	DoneCh3
Ch3Length7:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L7,Ch3
		jmp	DoneCh3
Ch3Length8:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L8,Ch3
		jmp	DoneCh3
Ch3Length9:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L9,Ch3
		jmp	DoneCh3
Ch3Length10:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L10,Ch3
		jmp	DoneCh3
Ch3Length11:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L11,Ch3
		jmp	DoneCh3
Ch3Length12:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L12,Ch3
		jmp	DoneCh3
Ch3Length13:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L13,Ch3
		jmp	DoneCh3
Ch3Length14:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L14,Ch3
		jmp	DoneCh3
Ch3Length15:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L15,Ch3
		jmp	DoneCh3
Ch3Length16:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L16,Ch3
		jmp	DoneCh3
Ch3Length17:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L17,Ch3
		jmp	DoneCh3
Ch3Length18:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L18,Ch3
		jmp	DoneCh3
Ch3Length19:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L19,Ch3
		jmp	DoneCh3
Ch3Length20:	printf
		byte	"Channel  3: %s %ld       \n\n",0
		dword	L20,Ch3
		jmp	DoneCh3

DoneCh3:


		cmp	Ch4, 1000
		jle	Ch4Length0
		cmp	Ch4, 1050
		jle	Ch4Length1
		cmp	Ch4, 1100
		jle	Ch4Length2
		cmp	Ch4, 1150
		jle	Ch4Length3
		cmp	Ch4, 1200
		jle	Ch4Length4
		cmp	Ch4, 1250
		jle	Ch4Length5
		cmp	Ch4, 1300
		jle	Ch4Length6
		cmp	Ch4, 1350
		jle	Ch4Length7
		cmp	Ch4, 1400
		jle	Ch4Length8		
		cmp	Ch4, 1450
		jle	Ch4Length9
		cmp	Ch4, 1500
		jle	Ch4Length10
		cmp	Ch4, 1550
		jle	Ch4Length11
		cmp	Ch4, 1600
		jle	Ch4Length12
		cmp	Ch4, 1650
		jle	Ch4Length13
		cmp	Ch4, 1700
		jle	Ch4Length14
		cmp	Ch4, 1750
		jle	Ch4Length15
		cmp	Ch4, 1800
		jle	Ch4Length16
		cmp	Ch4, 1850
		jle	Ch4Length17
		cmp	Ch4, 1900
		jle	Ch4Length18
		cmp	Ch4, 1950
		jle	Ch4Length19
		cmp	Ch4, 6969
		jle	Ch4Length20
	
Ch4Length0:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L0,Ch4
		jmp	DoneCh4
Ch4Length1:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L1,Ch4
		jmp	DoneCh4
Ch4Length2:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L2,Ch4
		jmp	DoneCh4
Ch4Length3:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L3,Ch4
		jmp	DoneCh4
Ch4Length4:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L4,Ch4
		jmp	DoneCh4
Ch4Length5:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L5,Ch4
		jmp	DoneCh4
Ch4Length6:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L6,Ch4
		jmp	DoneCh4
Ch4Length7:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L7,Ch4
		jmp	DoneCh4
Ch4Length8:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L8,Ch4
		jmp	DoneCh4
Ch4Length9:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L9,Ch4
		jmp	DoneCh4
Ch4Length10:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L10,Ch4
		jmp	DoneCh4
Ch4Length11:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L11,Ch4
		jmp	DoneCh4
Ch4Length12:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L12,Ch4
		jmp	DoneCh4
Ch4Length13:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L13,Ch4
		jmp	DoneCh4
Ch4Length14:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L14,Ch4
		jmp	DoneCh4
Ch4Length15:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L15,Ch4
		jmp	DoneCh4
Ch4Length16:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L16,Ch4
		jmp	DoneCh4
Ch4Length17:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L17,Ch4
		jmp	DoneCh4
Ch4Length18:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L18,Ch4
		jmp	DoneCh4
Ch4Length19:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L19,Ch4
		jmp	DoneCh4
Ch4Length20:	printf
		byte	"Channel  4: %s %ld       \n\n",0
		dword	L20,Ch4
		jmp	DoneCh4

DoneCh4:


		cmp	Ch5, 1000
		jle	Ch5Length0
		cmp	Ch5, 1050
		jle	Ch5Length1
		cmp	Ch5, 1100
		jle	Ch5Length2
		cmp	Ch5, 1150
		jle	Ch5Length3
		cmp	Ch5, 1200
		jle	Ch5Length4
		cmp	Ch5, 1250
		jle	Ch5Length5
		cmp	Ch5, 1300
		jle	Ch5Length6
		cmp	Ch5, 1350
		jle	Ch5Length7
		cmp	Ch5, 1400
		jle	Ch5Length8		
		cmp	Ch5, 1450
		jle	Ch5Length9
		cmp	Ch5, 1500
		jle	Ch5Length10
		cmp	Ch5, 1550
		jle	Ch5Length11
		cmp	Ch5, 1600
		jle	Ch5Length12
		cmp	Ch5, 1650
		jle	Ch5Length13
		cmp	Ch5, 1700
		jle	Ch5Length14
		cmp	Ch5, 1750
		jle	Ch5Length15
		cmp	Ch5, 1800
		jle	Ch5Length16
		cmp	Ch5, 1850
		jle	Ch5Length17
		cmp	Ch5, 1900
		jle	Ch5Length18
		cmp	Ch5, 1950
		jle	Ch5Length19
		cmp	Ch5, 6969
		jle	Ch5Length20
	
Ch5Length0:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L0,Ch5
		jmp	DoneCh5
Ch5Length1:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L1,Ch5
		jmp	DoneCh5
Ch5Length2:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L2,Ch5
		jmp	DoneCh5
Ch5Length3:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L3,Ch5
		jmp	DoneCh5
Ch5Length4:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L4,Ch5
		jmp	DoneCh5
Ch5Length5:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L5,Ch5
		jmp	DoneCh5
Ch5Length6:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L6,Ch5
		jmp	DoneCh5
Ch5Length7:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L7,Ch5
		jmp	DoneCh5
Ch5Length8:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L8,Ch5
		jmp	DoneCh5
Ch5Length9:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L9,Ch5
		jmp	DoneCh5
Ch5Length10:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L10,Ch5
		jmp	DoneCh5
Ch5Length11:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L11,Ch5
		jmp	DoneCh5
Ch5Length12:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L12,Ch5
		jmp	DoneCh5
Ch5Length13:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L13,Ch5
		jmp	DoneCh5
Ch5Length14:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L14,Ch5
		jmp	DoneCh5
Ch5Length15:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L15,Ch5
		jmp	DoneCh5
Ch5Length16:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L16,Ch5
		jmp	DoneCh5
Ch5Length17:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L17,Ch5
		jmp	DoneCh5
Ch5Length18:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L18,Ch5
		jmp	DoneCh5
Ch5Length19:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L19,Ch5
		jmp	DoneCh5
Ch5Length20:	printf
		byte	"Channel  5: %s %ld       \n\n",0
		dword	L20,Ch5
		jmp	DoneCh5

DoneCh5:


		cmp	Ch6, 1000
		jle	Ch6Length0
		cmp	Ch6, 1050
		jle	Ch6Length1
		cmp	Ch6, 1100
		jle	Ch6Length2
		cmp	Ch6, 1150
		jle	Ch6Length3
		cmp	Ch6, 1200
		jle	Ch6Length4
		cmp	Ch6, 1250
		jle	Ch6Length5
		cmp	Ch6, 1300
		jle	Ch6Length6
		cmp	Ch6, 1350
		jle	Ch6Length7
		cmp	Ch6, 1400
		jle	Ch6Length8		
		cmp	Ch6, 1450
		jle	Ch6Length9
		cmp	Ch6, 1500
		jle	Ch6Length10
		cmp	Ch6, 1550
		jle	Ch6Length11
		cmp	Ch6, 1600
		jle	Ch6Length12
		cmp	Ch6, 1650
		jle	Ch6Length13
		cmp	Ch6, 1700
		jle	Ch6Length14
		cmp	Ch6, 1750
		jle	Ch6Length15
		cmp	Ch6, 1800
		jle	Ch6Length16
		cmp	Ch6, 1850
		jle	Ch6Length17
		cmp	Ch6, 1900
		jle	Ch6Length18
		cmp	Ch6, 1950
		jle	Ch6Length19
		cmp	Ch6, 6969
		jle	Ch6Length20
	
Ch6Length0:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L0,Ch6
		jmp	DoneCh6
Ch6Length1:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L1,Ch6
		jmp	DoneCh6
Ch6Length2:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L2,Ch6
		jmp	DoneCh6
Ch6Length3:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L3,Ch6
		jmp	DoneCh6
Ch6Length4:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L4,Ch6
		jmp	DoneCh6
Ch6Length5:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L5,Ch6
		jmp	DoneCh6
Ch6Length6:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L6,Ch6
		jmp	DoneCh6
Ch6Length7:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L7,Ch6
		jmp	DoneCh6
Ch6Length8:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L8,Ch6
		jmp	DoneCh6
Ch6Length9:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L9,Ch6
		jmp	DoneCh6
Ch6Length10:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L10,Ch6
		jmp	DoneCh6
Ch6Length11:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L11,Ch6
		jmp	DoneCh6
Ch6Length12:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L12,Ch6
		jmp	DoneCh6
Ch6Length13:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L13,Ch6
		jmp	DoneCh6
Ch6Length14:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L14,Ch6
		jmp	DoneCh6
Ch6Length15:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L15,Ch6
		jmp	DoneCh6
Ch6Length16:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L16,Ch6
		jmp	DoneCh6
Ch6Length17:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L17,Ch6
		jmp	DoneCh6
Ch6Length18:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L18,Ch6
		jmp	DoneCh6
Ch6Length19:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L19,Ch6
		jmp	DoneCh6
Ch6Length20:	printf
		byte	"Channel  6: %s %ld       \n\n",0
		dword	L20,Ch6
		jmp	DoneCh6

DoneCh6:


		cmp	Ch7, 1000
		jle	Ch7Length0
		cmp	Ch7, 1050
		jle	Ch7Length1
		cmp	Ch7, 1100
		jle	Ch7Length2
		cmp	Ch7, 1150
		jle	Ch7Length3
		cmp	Ch7, 1200
		jle	Ch7Length4
		cmp	Ch7, 1250
		jle	Ch7Length5
		cmp	Ch7, 1300
		jle	Ch7Length6
		cmp	Ch7, 1350
		jle	Ch7Length7
		cmp	Ch7, 1400
		jle	Ch7Length8		
		cmp	Ch7, 1450
		jle	Ch7Length9
		cmp	Ch7, 1500
		jle	Ch7Length10
		cmp	Ch7, 1550
		jle	Ch7Length11
		cmp	Ch7, 1600
		jle	Ch7Length12
		cmp	Ch7, 1650
		jle	Ch7Length13
		cmp	Ch7, 1700
		jle	Ch7Length14
		cmp	Ch7, 1750
		jle	Ch7Length15
		cmp	Ch7, 1800
		jle	Ch7Length16
		cmp	Ch7, 1850
		jle	Ch7Length17
		cmp	Ch7, 1900
		jle	Ch7Length18
		cmp	Ch7, 1950
		jle	Ch7Length19
		cmp	Ch7, 6969
		jle	Ch7Length20
	
Ch7Length0:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L0,Ch7
		jmp	DoneCh7
Ch7Length1:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L1,Ch7
		jmp	DoneCh7
Ch7Length2:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L2,Ch7
		jmp	DoneCh7
Ch7Length3:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L3,Ch7
		jmp	DoneCh7
Ch7Length4:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L4,Ch7
		jmp	DoneCh7
Ch7Length5:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L5,Ch7
		jmp	DoneCh7
Ch7Length6:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L6,Ch7
		jmp	DoneCh7
Ch7Length7:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L7,Ch7
		jmp	DoneCh7
Ch7Length8:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L8,Ch7
		jmp	DoneCh7
Ch7Length9:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L9,Ch7
		jmp	DoneCh7
Ch7Length10:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L10,Ch7
		jmp	DoneCh7
Ch7Length11:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L11,Ch7
		jmp	DoneCh7
Ch7Length12:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L12,Ch7
		jmp	DoneCh7
Ch7Length13:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L13,Ch7
		jmp	DoneCh7
Ch7Length14:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L14,Ch7
		jmp	DoneCh7
Ch7Length15:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L15,Ch7
		jmp	DoneCh7
Ch7Length16:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L16,Ch7
		jmp	DoneCh7
Ch7Length17:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L17,Ch7
		jmp	DoneCh7
Ch7Length18:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L18,Ch7
		jmp	DoneCh7
Ch7Length19:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L19,Ch7
		jmp	DoneCh7
Ch7Length20:	printf
		byte	"Channel  7: %s %ld       \n\n",0
		dword	L20,Ch7
		jmp	DoneCh7

DoneCh7:


		cmp	Ch8, 1000
		jle	Ch8Length0
		cmp	Ch8, 1050
		jle	Ch8Length1
		cmp	Ch8, 1100
		jle	Ch8Length2
		cmp	Ch8, 1150
		jle	Ch8Length3
		cmp	Ch8, 1200
		jle	Ch8Length4
		cmp	Ch8, 1250
		jle	Ch8Length5
		cmp	Ch8, 1300
		jle	Ch8Length6
		cmp	Ch8, 1350
		jle	Ch8Length7
		cmp	Ch8, 1400
		jle	Ch8Length8		
		cmp	Ch8, 1450
		jle	Ch8Length9
		cmp	Ch8, 1500
		jle	Ch8Length10
		cmp	Ch8, 1550
		jle	Ch8Length11
		cmp	Ch8, 1600
		jle	Ch8Length12
		cmp	Ch8, 1650
		jle	Ch8Length13
		cmp	Ch8, 1700
		jle	Ch8Length14
		cmp	Ch8, 1750
		jle	Ch8Length15
		cmp	Ch8, 1800
		jle	Ch8Length16
		cmp	Ch8, 1850
		jle	Ch8Length17
		cmp	Ch8, 1900
		jle	Ch8Length18
		cmp	Ch8, 1950
		jle	Ch8Length19
		cmp	Ch8, 6969
		jle	Ch8Length20
	
Ch8Length0:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L0,Ch8
		jmp	DoneCh8
Ch8Length1:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L1,Ch8
		jmp	DoneCh8
Ch8Length2:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L2,Ch8
		jmp	DoneCh8
Ch8Length3:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L3,Ch8
		jmp	DoneCh8
Ch8Length4:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L4,Ch8
		jmp	DoneCh8
Ch8Length5:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L5,Ch8
		jmp	DoneCh8
Ch8Length6:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L6,Ch8
		jmp	DoneCh8
Ch8Length7:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L7,Ch8
		jmp	DoneCh8
Ch8Length8:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L8,Ch8
		jmp	DoneCh8
Ch8Length9:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L9,Ch8
		jmp	DoneCh8
Ch8Length10:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L10,Ch8
		jmp	DoneCh8
Ch8Length11:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L11,Ch8
		jmp	DoneCh8
Ch8Length12:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L12,Ch8
		jmp	DoneCh8
Ch8Length13:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L13,Ch8
		jmp	DoneCh8
Ch8Length14:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L14,Ch8
		jmp	DoneCh8
Ch8Length15:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L15,Ch8
		jmp	DoneCh8
Ch8Length16:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L16,Ch8
		jmp	DoneCh8
Ch8Length17:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L17,Ch8
		jmp	DoneCh8
Ch8Length18:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L18,Ch8
		jmp	DoneCh8
Ch8Length19:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L19,Ch8
		jmp	DoneCh8
Ch8Length20:	printf
		byte	"Channel  8: %s %ld       \n\n",0
		dword	L20,Ch8
		jmp	DoneCh8

DoneCh8:


		cmp	Ch9, 1000
		jle	Ch9Length0
		cmp	Ch9, 1050
		jle	Ch9Length1
		cmp	Ch9, 1100
		jle	Ch9Length2
		cmp	Ch9, 1150
		jle	Ch9Length3
		cmp	Ch9, 1200
		jle	Ch9Length4
		cmp	Ch9, 1250
		jle	Ch9Length5
		cmp	Ch9, 1300
		jle	Ch9Length6
		cmp	Ch9, 1350
		jle	Ch9Length7
		cmp	Ch9, 1400
		jle	Ch9Length8		
		cmp	Ch9, 1450
		jle	Ch9Length9
		cmp	Ch9, 1500
		jle	Ch9Length10
		cmp	Ch9, 1550
		jle	Ch9Length11
		cmp	Ch9, 1600
		jle	Ch9Length12
		cmp	Ch9, 1650
		jle	Ch9Length13
		cmp	Ch9, 1700
		jle	Ch9Length14
		cmp	Ch9, 1750
		jle	Ch9Length15
		cmp	Ch9, 1800
		jle	Ch9Length16
		cmp	Ch9, 1850
		jle	Ch9Length17
		cmp	Ch9, 1900
		jle	Ch9Length18
		cmp	Ch9, 1950
		jle	Ch9Length19
		cmp	Ch9, 6969
		jle	Ch9Length20
	
Ch9Length0:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L0,Ch9
		jmp	DoneCh9
Ch9Length1:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L1,Ch9
		jmp	DoneCh9
Ch9Length2:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L2,Ch9
		jmp	DoneCh9
Ch9Length3:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L3,Ch9
		jmp	DoneCh9
Ch9Length4:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L4,Ch9
		jmp	DoneCh9
Ch9Length5:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L5,Ch9
		jmp	DoneCh9
Ch9Length6:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L6,Ch9
		jmp	DoneCh9
Ch9Length7:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L7,Ch9
		jmp	DoneCh9
Ch9Length8:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L8,Ch9
		jmp	DoneCh9
Ch9Length9:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L9,Ch9
		jmp	DoneCh9
Ch9Length10:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L10,Ch9
		jmp	DoneCh9
Ch9Length11:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L11,Ch9
		jmp	DoneCh9
Ch9Length12:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L12,Ch9
		jmp	DoneCh9
Ch9Length13:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L13,Ch9
		jmp	DoneCh9
Ch9Length14:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L14,Ch9
		jmp	DoneCh9
Ch9Length15:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L15,Ch9
		jmp	DoneCh9
Ch9Length16:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L16,Ch9
		jmp	DoneCh9
Ch9Length17:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L17,Ch9
		jmp	DoneCh9
Ch9Length18:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L18,Ch9
		jmp	DoneCh9
Ch9Length19:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L19,Ch9
		jmp	DoneCh9
Ch9Length20:	printf
		byte	"Channel  9: %s %ld       \n\n",0
		dword	L20,Ch9
		jmp	DoneCh9

DoneCh9:


		cmp	Ch10, 1000
		jle	Ch10Length0
		cmp	Ch10, 1050
		jle	Ch10Length1
		cmp	Ch10, 1100
		jle	Ch10Length2
		cmp	Ch10, 1150
		jle	Ch10Length3
		cmp	Ch10, 1200
		jle	Ch10Length4
		cmp	Ch10, 1250
		jle	Ch10Length5
		cmp	Ch10, 1300
		jle	Ch10Length6
		cmp	Ch10, 1350
		jle	Ch10Length7
		cmp	Ch10, 1400
		jle	Ch10Length8		
		cmp	Ch10, 1450
		jle	Ch10Length9
		cmp	Ch10, 1500
		jle	Ch10Length10
		cmp	Ch10, 1550
		jle	Ch10Length11
		cmp	Ch10, 1600
		jle	Ch10Length12
		cmp	Ch10, 1650
		jle	Ch10Length13
		cmp	Ch10, 1700
		jle	Ch10Length14
		cmp	Ch10, 1750
		jle	Ch10Length15
		cmp	Ch10, 1800
		jle	Ch10Length16
		cmp	Ch10, 1850
		jle	Ch10Length17
		cmp	Ch10, 1900
		jle	Ch10Length18
		cmp	Ch10, 1950
		jle	Ch10Length19
		cmp	Ch10, 6969
		jle	Ch10Length20
	
Ch10Length0:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L0,Ch10
		jmp	DoneCh10
Ch10Length1:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L1,Ch10
		jmp	DoneCh10
Ch10Length2:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L2,Ch10
		jmp	DoneCh10
Ch10Length3:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L3,Ch10
		jmp	DoneCh10
Ch10Length4:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L4,Ch10
		jmp	DoneCh10
Ch10Length5:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L5,Ch10
		jmp	DoneCh10
Ch10Length6:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L6,Ch10
		jmp	DoneCh10
Ch10Length7:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L7,Ch10
		jmp	DoneCh10
Ch10Length8:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L8,Ch10
		jmp	DoneCh10
Ch10Length9:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L9,Ch10
		jmp	DoneCh10
Ch10Length10:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L10,Ch10
		jmp	DoneCh10
Ch10Length11:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L11,Ch10
		jmp	DoneCh10
Ch10Length12:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L12,Ch10
		jmp	DoneCh10
Ch10Length13:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L13,Ch10
		jmp	DoneCh10
Ch10Length14:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L14,Ch10
		jmp	DoneCh10
Ch10Length15:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L15,Ch10
		jmp	DoneCh10
Ch10Length16:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L16,Ch10
		jmp	DoneCh10
Ch10Length17:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L17,Ch10
		jmp	DoneCh10
Ch10Length18:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L18,Ch10
		jmp	DoneCh10
Ch10Length19:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L19,Ch10
		jmp	DoneCh10
Ch10Length20:	printf
		byte	"Channel 10: %s %ld       \n\n",0
		dword	L20,Ch10
		jmp	DoneCh10

DoneCh10:




		
; Crude clear screen via BIOS
; Hell, no: just reset video mode and we no longer have the scroll!!!
; int 10 ah=0 al=mode
; This very much depends on the delay??????
;		mov	ah, 0
;		mov	al, 6h ; quite OK video mode
;		int	10h

; Don't use clean screen for getting rid of scrolling
;		mov	ah, 6	; clear screen ah=6
;		mov	al, 0
;		mov	cl, 0
;		mov	ch, 0
;		mov	dl, 25
;		mov	dh, 80
;		int	10h

		mov	ah, 1	; make cursor disappear
		mov	ch, 20h
		int	10h
		
		mov	ah, 2	; move cursor ah=2
		mov	bh, 0
		mov	dh, 0
		mov	dl, 0
		int	10h
		
		;mov	ah, 0Bh	; color???
		;mov	al, 01h
		;mov	bh, 00h
		;mov	bl, 71h
		;int	10h
		;include bios.inc
		;@SetColor 3h
		



		mov	ah, 1			;Test for keystroke.
		int	16h
		je	DisplayLp
		mov	ah, 0			;Read that keystroke.
		int	16h



; Okay, restore the interrupt vector.  We need the interrupts off
; here for the same reason as above.

		mov	ax, 0
		mov	es, ax
		cli
		mov	ax, word ptr IntVectSave
		mov	es:[INTNUMBER*4], ax
		mov	ax, word ptr IntVectSave+2
		mov	es:[INTNUMBER*4+2], ax
		sti

Quit:		ExitPgm			;DOS macro to quit program.
Main		endp

cseg		ends

sseg		segment	para stack 'stack'
stk		db	1024 dup ("stack   ")
sseg		ends

zzzzzzseg	segment	para public 'zzzzzz'
LastBytes	db	16 dup (?)
zzzzzzseg	ends
		end	Main
