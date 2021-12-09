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

OldInt0Fh	dword	?
SumLatency	dword	0
Executions	dword	0
Average		dword	0
PrevIntTime	dword   0
GotIntTime	dword   0
PulseLength	dword	0
GotSync		dword	0
ChannelCount	dword	1
SyncPulse	dword   0
Ch1		dword	0
Ch2		dword	0
Ch3		dword	0
Ch4		dword	0
Ch5		dword	0
Ch6		dword	0
Ch7		dword	0
Ch8		dword	0


; This program reads the 8254 timer chip.  This chip counts from
; 0FFFFh down to zero and then generates an interrupt.  It wraps
; around from 0 to 0FFFFh and continues counting down once it
; generates the interrupt.
;
; 8254 Timer Chip port addresses:

Timer0_8254	equ	40h
Cntrl_8254	equ	43h


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

TimerISR	proc	near
		push	ax
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
		mov     cseg:PrevIntTime, eax ; time stamp first interrupt
		mov	cseg:GotIntTime, 1

StartCompare:	mov	ebx, cseg:PrevIntTime
		sub	ebx, eax		; subtract present interrupt time from previous one
		mov	cseg:PulseLength, ebx	;  to get the pulse length
		mov	cseg:PrevIntTime, eax	; save present int time
		
		cmp	cseg:PulseLength, 0	; als timer van 0000h naar FFFFh is gegaan krijgen we een negatieve waarde
		jge	Positive
		add	cseg:PulseLength, 65536	; maak waarde correct
			
		
Positive:
		;printf				; print in ISR instead of in Main 
		;byte	"Pulse Length: %ld\n",0
		;dword	PulseLength
		
		
		cmp	cseg:PulseLength, 15000 ; is it the sync pulse?
		ja	Sync
		cmp	cseg:GotSync, 1		; if we have the first sync go to channels
		je	Channels
		jmp	Exit			; not sync, exit and try again
		
Sync:		mov 	eax, cseg:PulseLength
		mov     cseg:SyncPulse, eax
		mov	cseg:GotSync, 1
		jmp	Exit
		
Channels:	cmp	cseg:ChannelCount, 1
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

Chan1:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch1, eax
		inc	cseg:ChannelCount
		jmp	Exit
Chan2:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch2, eax
		inc	cseg:ChannelCount
		jmp	Exit
Chan3:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch3, eax
		inc	cseg:ChannelCount
		jmp	Exit
Chan4:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch4, eax
		inc	cseg:ChannelCount
		jmp	Exit
Chan5:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch5, eax
		inc	cseg:ChannelCount
		jmp	Exit
Chan6:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch6, eax
		inc	cseg:ChannelCount
		jmp	Exit
Chan7:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch7, eax
		inc	cseg:ChannelCount
		jmp	Exit
Chan8:		mov 	eax, cseg:PulseLength
		mov     cseg:Ch8, eax
		mov	cseg:ChannelCount, 1
		;jmp	Exit




		


Exit:		mov al, 20h
		out 20h, al		; send EOI (end of interrupt)!!!!!!!!!!!!!!!
					; Hell, the old handler wasn't sending EOI
					; that's why we only got one interrupt
					; Cause is the old ISR which probably doesn't even
					; exist for the par. port

		; Forget about the old handler!!!!!!
		;jmp	cseg:OldInt0Fh	;chains to the old interrupt
					; this will generate the IRET for you
					; but does not send an EOI !!!!!!!!!

		pop	ebx
		pop	ax					
		iret
TimerISR	endp




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


; let's hook the parallel port interrupt: IRQ 7  INT 0Fh

		mov	ax, 0
		mov	es, ax
		mov	ax, es:[0Fh*4]
		mov	word ptr OldInt0Fh, ax
		mov	ax, es:[0Fh*4 + 2]
		mov	word ptr OldInt0Fh+2, ax

		cli
		mov	word ptr es:[0Fh*4], offset TimerISR
		mov	es:[0Fh*4 + 2], cs
		sti

		
; Got to turn on IRQ 7 on the PIC (will probably be on already)
		in 	al, 21h 	;Read existing bits.
		and 	al, 7Fh 	;Turn on IRQ 7
		out 	21h, al 	;Write result back to PIC.
		
; And got to turn on interrupts on parallel port!!!!!!!!!!!!!
		mov	dx, 37Ah	; got to use dx for ports > 255 (dec)
		in	al, dx		; read par. port Control register (378h + 2)
		or 	al, 10h		; enable interrupts on line 10 (ACK) (set bit 4)
		out	dx, al		; write back to Control register

; We are using a data line e.g line 9 to output 5V directly to line 10
; so data register must be in output mode		
		mov	dx, 37Ah	; got to use dx for ports > 255 (dec)
		in	al, dx		; read par. port Control register (378h + 2)
		and 	al, 0DFh	; make sure port is in forward direction (output only, no bi-direct.) (clear bit 5)
		out	dx, al		; write back to Control register				
		


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
		
		
		
		
		;mov	eax, SumLatency
		;cdq				;Extends eax->edx.
		;div	Executions
		;mov     Average, eax
		;printf
		;byte	"Count: %ld, average: %ld\n",0
		;dword	Executions, Average
		;printf
		;byte	"Pulse Length: %ld\n",0
		;dword	PulseLength
		
		printf
		byte	"SyncPulse: %ld\n"
		byte	"Channel 1: %ld\n"
		byte	"Channel 2: %ld\n"
		byte	"Channel 3: %ld\n"
		byte	"Channel 4: %ld\n"
		byte	"Channel 5: %ld\n"
		byte	"Channel 6: %ld\n"
		byte	"Channel 7: %ld\n"
		byte	"Channel 8: %ld\n\n",0
		dword	SyncPulse, Ch1, Ch2, Ch3, Ch3, Ch4, Ch5, Ch6, Ch7, Ch8
		
		call ShortDelay
		call ShortDelay
		call ShortDelay
		
; Crude clear screen via BIOS
; Hell, no: just reset video mode and we no longer have the scroll!!!
; int 10 ah=0 al=mode
; This very much depends on the delay??????
		mov	ah, 0
		mov	al, 6h
		;mov	cl, 0
		;mov	ch, 0
		;mov	dl, 25
		;mov	dh, 80
		int	10h
		



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
		mov	ax, word ptr OldInt0Fh
		mov	es:[0Fh*4], ax
		mov	ax, word ptr OldInt0Fh+2
		mov	es:[0Fh*4+2], ax
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
