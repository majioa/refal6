CSEG    SEGMENT
	PUBLIC 	_GO_TO  ; tail recursion call for Turbo C 2.0.
			; The use in C:
			;     return(GO_TO(fun));
			; function fun should return int.
_GO_TO:	pop	cx
        pop 	cx	; remove return address
	pop	ax
	pop	bx	; <bx,ax> - transfer address (fun)
        mov 	sp,bp 	; restore sp
        pop 	bp    	; restore bp
	push	bx
	push	ax	; transfer address onto the stack
	retf
CSEG    ENDS
        END
