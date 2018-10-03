; macros.inc
; written by hyan23
; 2018.09.22

WriteString macro string
LOCAL L0, L1
    jmp L0
L1:     db      string, '$'
L0:
    push ax
    push dx
    push ds
    
    mov ax, cs
    mov ds, ax
    lea dx, L1
    mov ah, 09h
    int 21h
    
    pop ds
    pop dx
    pop ax
endm