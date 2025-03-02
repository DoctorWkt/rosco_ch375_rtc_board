; Code to send commands and send/receive
; data to a DS1302 real-time clock.
; Copyright (c) 2025 Warren Toomey, GPL3

RTCCLK_LO equ	$F78001
RTCCLK_HI equ	$F78003
RTCCE_LO  equ	$F7C001
RTCCE_HI  equ	$F7C003

    section .text

; NOTE: I really should disable interrupts at the start of
; each routine and re-enable them at the end, as we are
; sending/reading serial bits to/from the DS1302. If we take
; an interrupt in the middle, it might throw the timing off.
; 

; Send a 16-bit cmd+data to the RTC. d1 gets destroyed.
; void rtcwrite(uint16_t value)
;
rtcwrite::	move.w  6(a7),d0	; Get the command and data
	  	move.w  #15,d1		; Send 16 bits (loop 15 ... 0)
          	move.b  d0,RTCCE_HI	; Turn on CE
L1:       	move.b  d0,RTCCLK_HI	; Raise CLK and send LSB of d0
          	nop			; Delays as max CLK is 2MHz
	  	move.b  d0,RTCCLK_LO	; Lower the CLK
          	lsr.w   d0		; Shift the contents down one bit
          	dbf     d1,L1		; Loop back
          	move.b  d0,RTCCE_LO	; Turn off CE
          	rts 

; Send a command to the RTC and get back the data. d1 gets destroyed.
; uint8_t rtcread(uint8_t cmd)
rtcread::	move.b	7(a7),d0	; Get the command
		move.l  d2,-(a7)	; Save d2
	  	move.w  #7,d1		; Send 8 bits (loop 7 ... 0)
          	move.b  d0,RTCCE_HI	; Turn on CE
L2:       	move.b  d0,RTCCLK_HI	; Raise CLK and send LSB of d0
          	nop			; Delays as max CLK is 2MHz
	  	move.b  d0,RTCCLK_LO	; Lower the CLK
          	lsr.w   d0		; Shift the contents down one bit
          	dbf     d1,L2		; Loop back

		clr.w	d0		; Clear out d0
		move.b	RTCCLK_LO,d0	; As CLK low, get one data bit
		and.b   #$01,d0		; Keep only the lsb
	  	move.w  #6,d1		; Read 7 more bits (loop 6 ... 0)
L3:		
		ror.b	d0		; Rotate the contents one bit
		tst.b	RTCCLK_HI	; Raise CLK
		nop
		move.b	RTCCLK_LO,d2	; Lower the CLK, get a bit of data
		and.b   #$01,d2		; Keep only the lsb
		or.b	d2,d0		; Add that bit to d0
          	dbf     d1,L3		; Loop back

		ror.b	d0		; Rotate the last bit into place
          	move.b  d0,RTCCE_LO	; Turn off CE
		move.l  (a7)+,d2	; Restore d2
          	rts 
