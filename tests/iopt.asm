; iopt.asm
; written by hyan23
; 2017.11.28

iopt_data segment
    ; test iopt
    iopt_s db ' **test iopt** ', '$'
    
    ; test cursor
    iopt_0_msg0 db 'test cursor', '$'
    iopt_0_msg1 db 'set cursor position: ', '$'
    iopt_0_msg2 db 'print at given position', '$'
    
    ; test iptpwd
    iopt_1_msg0 db 'test iptpwd', '$'
    iopt_1_msg1 db 'Enter password: ', '$'
    iopt_1_msg2 db 'You entered: ', '$'
    
    iopt_1_buf db 100 dup('?')
iopt_data ends

; in: none, ret: none
test_iopt proc near
    push ds
    push es
    
    mov ax, iopt_data
    mov ds, ax
    mov es, ax
    assume ds:iopt_data
    assume es:iopt_data
    
    lea dx, iopt_s
    call puts
    call pause
    ; Your code begins here
    
    call test_cursor                    ; test cursor
    call pause
    
    call test_iptpwd                    ; test iptpwd
    call pause
    
    pop es
    pop ds
    ret
test_iopt endp

; Your sub-routine begins here

; in: none, ret: none
test_cursor proc near
    push dx
    
    lea dx, iopt_0_msg0                 ; test cursor
    call puts
    
    lea dx, iopt_0_msg1                 ; set cursor position
    call optstr
    mov ah, 10
    mov al, 20
    push ax
    call opthex
    pop ax
    call cursor
    
    lea dx, iopt_0_msg2                 ; print string
    call puts
    
    pop dx
    ret
test_cursor endp

; in: none, ret: none
test_iptpwd proc near
    push dx
    
    lea dx, iopt_1_msg0                 ; test iptpwd
    call puts
    
    lea dx, iopt_1_msg1                 ; input password
    call optstr
    lea bx, iopt_1_buf
    call iptpwd
    
    lea dx, iopt_1_msg2                 ; print inputed password
    call optstr
    lea dx, iopt_1_buf
    call putsq
    
    pop dx
    ret
test_iptpwd endp