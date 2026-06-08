bits 32
global sys_call_isr

;;; System call interrupt handler. To be implemented in Module R3.
extern sys_call			; The C function that sys_call_isr will call
sys_call_isr:
	;; Push all general registers (explicitly)
	push EAX
	push ECX
	push EDX
	push EBX
	push EBP
	push ESI
	push EDI

	;; Push remaining segment registers in order
	push SS
	push GS
	push FS
	push ES
	push DS

	;; Push esp
	push esp

	;; Call c file to handle
	call sys_call

	;; Store value in EAX into ESP
	mov ESP, EAX

	;; Pop registers in order for stuct
	pop DS
	pop ES
	pop FS
	pop GS
	pop SS

	;; Pop general purpose registers (explicitly)
	pop EDI
	pop ESI
	pop EBP
	pop EBX
	pop EDX
	pop ECX
	pop EAX

	iret