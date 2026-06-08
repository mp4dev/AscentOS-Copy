bits 32
global serial_isr

;;; Serial interrupt service routine
extern serial_interrupt         ; define external C funciton

serial_isr: 
    ;; Call C funciton
    call serial_interrupt

    ;; Return
    iret