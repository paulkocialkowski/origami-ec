; Copyright (C) 2017 Paul Kocialkowski <contact@paulk.fr>
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.

.area HOME (CODE)

__sdcc_banked_call::
	mov r7, a		; store acc value to r7
	push dpl		; store dpl value to stack
	push dph		; store dph value to stack
	mov dptr, #0xfea1	; load XBISEG3 to dptr
	movx a, @dptr		; load register value to acc
	pop dph			; restore dph value from stack
	pop dpl			; restore dpl value from stack
	push acc		; store acc (register value) to stack
	mov a, r0		; move r0 (call lsb) to acc
	push acc		; store acc to stack
	mov a, r1		; move r1 (call msb) value to acc
	push acc		; store acc to stack

;	SDCC currently only allows segments to reside 64 kiB above their mapped
;	code offset, with no way of controlling that offset.
;	Thus, a fixed 64 kiB offset is applied regardless of the requested bank.
	mov a, #0x07		; offset by 112 KiB (64 kiB + 48 kiB)

;	Proper code to allow bank switching with a bank-based offset follows:
;	mov a, r2		; move r2 (bank) value to acc
;	add a, #0x06		; offset by 16 KiB blocks - 1

	orl a, #0x80		; enable SEG3 remapping
	push dpl		; store dpl value to stack
	push dph		; store dph value to stack
	mov dptr, #0xfea1	; load XBISEG3 to dptr
	movx @dptr, a		; store value in dptr
	pop dph			; restore dph value from stack
	pop dpl			; restore dpl value from stack
	mov a, r7		; restore acc value from r7
	ret			; call banked function

__sdcc_banked_ret::
	mov r7, a		; store acc value to r7
	pop acc			; restore register value from stack
	push dpl		; store dpl value to stack
	push dph		; store dph value to stack
	mov dptr, #0xfea1	; load XBISEG3 to dptr
	movx @dptr, a		; load acc value to register
	pop dph			; restore dph value from stack
	pop dpl			; restore dpl value from stack
	mov a, r7		; restore acc value from r7
	ret			; return to initial call
