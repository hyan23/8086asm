; <filename>.asm
; written by <author> 
; <YYYY.MM.DD>

@WORD equ 2

data segment
data ends

sta segment stack
    db 4096 dup(0)
sta ends

code segment
assume cs:code, ds:data, ss:sta
start:
main proc far
    mov ax, data
    mov ds, ax
    mov es, ax
    ; Your code begins here
    
exit1:
    mov ax, 4c00h
    int 21h
main endp
; Your procedure begins here

code ends
end start