; <filename>.asm
; written by <author>
; <YYYY.MM.DD>

<module_name>_data segment
    ; test <module_name>
    <module_name>_s db ' **test <module_name>** ', '$'
    
    ; 
<module_name>_data ends

; in: none, ret: none
test_<module_name> proc near
    push ds
    push es
    
    mov ax, <module_name>_data
    mov ds, ax
    mov es, ax
    assume ds:<module_name>_data
    assume es:<module_name>_data
    
    lea dx, <module_name>_s
    call puts
    call pause
    ; Your code begins here
    
    pop es
    pop ds
    ret
test_<module_name> endp

; Your sub-routine begins here