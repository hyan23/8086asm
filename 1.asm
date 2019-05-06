; 1.asm
; written by hyan23
; 2017.11.26

data segment
arraySize   equ 21
array dw -10, -9, -8, -7, -6, -5, -4, -3, -2, -1
    dw 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
data ends

sta segment stack
    db 4096 dup(0)
sta ends

code segment
assume cs: code, ds:data, ss:sta
include common.inc
include macros.inc
start:
main proc far
    mov ax, data
    mov ds, ax
    mov es, ax
    ; Your code begins here
    
    call time
    mov ax, dx
    call optdec
    mov al, ':'
    call optch
    call time
    mov al, ah
    cbw
    call optdec
    mov al, ':'
    call optch
    call time
    cbw
    call optdec
    call crlf
    
    call getch
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    call clear
    call srand
    call dump
    
    lea bx, array
    lea dx, array
    mov cx, arraySize
    
    WriteString 'before: '
    call crlf
    call printArrayS
    call crlf
    
    WriteString 'shuffle: '
    call crlf
    call shuffle
    call printArrayS
    call crlf
    
    WriteString 'sort the array: '
    call crlf
    call qsort
    ;call sort
    call printArrayS
    call crlf
    
    WriteString 'reverse the array: '
    call crlf
    call reverse
    call printArrayS
    call crlf
    call exit
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    call clear
    
    call test_iopt                      ; test iopt
    call clear
    
    call test_file                      ; test file
    call clear
    
    call test_arith                     ; test arith
    call clear
    
    call test_string                    ; test string
    call clear
    
    call test_util                      ; test util
    call clear
    
    call test_sys                       ; test sys
exit1:
    mov ax, 4c00h
    int 21h
main endp
; Your procedure begins here

; implementations
include iopt.inc
include file.inc
include arith.inc
include string.inc
include util.inc
include sys.inc

; tests
include tests\iopt.asm
include tests\file.asm
include tests\arith.asm
include tests\string.asm
include tests\util.asm
include tests\sys.asm

code ends
end start