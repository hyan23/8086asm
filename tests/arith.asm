; arith.asm
; written by hyan23
; 2017.11.28

arith_data segment
    ; test arith
    arith_s db ' **test arith** ', '$'
    
    ; 
arith_data ends

; in: none, ret: none
test_arith proc near
    push ds
    push es
    
    mov ax, arith_data
    mov ds, ax
    mov es, ax
    assume ds:arith_data
    assume es:arith_data
    
    lea dx, arith_s
    call puts
    call pause
    ; Your code begins here
    
    mov dx, 1234h
    mov ax, 5678h
    mov cx, 5678h
    mov bx, 1234h
    call mul32
    call opthexqw
    call crlf
    call pause
    
    pop es
    pop ds
    ret
test_arith endp

; Your sub-routine begins here