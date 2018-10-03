; sys.asm
; written by hyan23
; 2017.11.28

sys_data segment
    ; test sys
    sys_s db ' **test sys** ', '$'
    
    ; test cmdline
    sys_0_msg0 db 'test cmdline', '$'
    sys_0_msg1 db 'command line: ', '$'
    
    sys_0_buf db 100 dup('$')
    
    ; test arg
    sys_1_msg0 db 'test arg', '$'
    sys_1_msg1 db 'cmdline', '$'
    sys_1_msg2 db 'arg #', '$'
    
    sys_1_buf db 100 dup('$')
    sys_1_arg db 100 dup('$')
    
    ; test pwd
    sys_2_msg0 db 'test pwd', '$'
    sys_2_msg1 db 'program working directory: ', '$'
    
    sys_2_buf db 100 dup('$')
sys_data ends

; in: none, ret: none
test_sys proc near
    push ds
    push es
    
    mov ax, sys_data
    mov ds, ax
    mov es, ax
    assume ds:sys_data
    assume es:sys_data
    
    lea dx, sys_s
    call puts
    call pause
    ; Your code begins here
    
    call test_cmdline                   ; test cmdline
    call pause
    
    call test_arg                       ; test arg
    call pause
    
    call test_pwd                       ; test pwd
    call pause
    
    pop es
    pop ds
    ret
test_sys endp

; Your sub-routine begins here

; in: none, ret: none
test_cmdline proc near
    push dx
    
    lea dx, sys_0_msg0                  ; test cmdline
    call puts
    
    lea dx, sys_0_msg1                  ; get cmdline
    call optstr
    lea bx, sys_0_buf
    call cmdline
    lea dx, sys_0_buf
    call putsq                          ; print string
    
    pop dx
    ret
test_cmdline endp

; in: none, ret: none
test_arg proc near
    push bx
    push cx
    push dx
    
    lea dx, sys_1_msg0                  ; test arg
    call puts
    
    lea dx, sys_1_msg1                  ; get cmdline
    call optstr
    lea bx, sys_1_buf
    call cmdline
    lea dx, sys_1_buf
    call putsq
    
    lea bx, sys_1_arg                   ; extract args successively
    xor cx, cx                          ; count
parse1:
    call arg
    mov dx, ax
    push dx
    lea dx, sys_1_arg
    call strlen
    pop dx
    cmp ax, 0
    je parse0
    push dx                             ; print arg
    lea dx, sys_1_msg2                  ; "arg#%d: %s"
    call optstr
    mov ax, cx
    call optdec
    mov al, ':'
    call optch
    mov al, ' '
    call optch
    lea dx, sys_1_arg
    call putsq
    pop dx
    add cx, 1
    jmp parse1
    
parse0:
    pop dx
    pop cx
    pop bx
    ret
test_arg endp

; in: none, ret: none
test_pwd proc near
    push dx
    
    lea dx, sys_2_msg0                  ; test pwd
    call puts
    
    lea dx, sys_2_msg1                  ; get pwd
    call optstr
    lea bx, sys_2_buf
    call pwd
    call convert                        ; convert to Dos style string
    lea dx, sys_2_buf
    call putsq                          ; print string
    
    pop dx
    ret
test_pwd endp