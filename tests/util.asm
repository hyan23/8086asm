; util.asm
; written by hyan23
; 2017.11.28

util_data segment
    ; test util
    util_s db ' **test util** ', '$'
    
    ; test rand
    util_1_msg0 db 'test rand', '$'
util_data ends

; in: none, ret: none
test_util proc near
    push ds
    push es
    
    mov ax, util_data
    mov ds, ax
    mov es, ax
    assume ds:util_data
    assume es:util_data
    
    lea dx, util_s
    call puts
    call pause
    ; Your code begins here
    
    call test_rand                      ; test rand
    call pause
    
    pop es
    pop ds
    ret
test_util endp

; Your sub-routine begins here

; in: none, ret: none
test_rand proc near
    push cx
    push dx
    
    lea dx, util_1_msg0                 ; test rand
    call puts
    
    call srand                          ; init
    mov cx, 10
test_rand0:
    call rand                           ; print a random array
    call optdec
    call space
    loop test_rand0
    call crlf
    
    pop dx
    pop cx
    ret
test_rand endp