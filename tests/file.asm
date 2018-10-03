; file.asm
; written by hyan23
; 2017.11.28

file_data segment
    ; test file
    file_s db ' **test file** ', '$'
    
    ; common
    file db 'c:\\a\1.txt', 0
    file1 db 'c:\\a\11.txt', 0
    buf db 100 dup('$')
    buf1 db 100 dup('$')
    handle dw ?
    
    ; test properties
    file_0_msg0 db 'test properties', '$'
    
    ; test open
    file_1_msg0 db 'test open', '$'
    file_1_msg1 db 'handle: ', '$'
    
    ; test read
    file_2_msg0 db 'test read', '$'
    file_2_msg1 db 'read 10 bytes data: ', '$'
    file_2_msg2 db 'move file pointer 1 byte back', '$'
file_data ends

; in: none, ret: none
test_file proc near
    push ds
    push es
    
    mov ax, file_data
    mov ds, ax
    mov es, ax
    assume ds:file_data
    assume es:file_data
    
    lea dx, file_s
    call puts
    call pause
    ; Your code begins here
    
    call test_properties                ; test properties
    call pause
    
    call test_open                      ; test open
    call pause
    
    call test_read                      ; test read
    call pause
    
    pop es
    pop ds
    ret
test_file endp

; Your sub-routine begins here

; in: none, ret: none
test_properties proc near
    push dx
    
    lea dx, file_0_msg0                 ; test properties
    call puts
    
    lea dx, file                        ; open file
    mov ax, @ORD
    call open
    cmp ax, -1
    je test_properties0
    push ax
    call properties                     ; get properties
    call opthex
    call crlf
    pop ax
    call close
    
test_properties0:
    pop dx
    ret
test_properties endp

; in: none, ret: none
test_open proc near
    push dx
    
    lea dx, file_1_msg0                 ; test open
    call puts
    
    lea dx, file                        ; open file
    mov ax, @ORD
    call open
    push ax
    call opthex                         ; print file handle
    call crlf
    
    lea dx, file                        ; open file
    mov ax, @ORD
    call open
    push ax
    call opthex                         ; print the file handle
    call crlf
    
    lea dx, file                        ; open file
    mov ax, @ORD
    call open
    push ax
    call opthex                         ; print the file handle
    call crlf
    
    pop ax                              ; close file
    call close
    pop ax
    call close
    pop ax
    call close
    
    lea dx, file                        ; open file
    mov ax, @ORD
    call open
    call opthex                         ; print the file handle
    call crlf
    call close                          ; close file
    
    pop dx
    ret
test_open endp

; in: none, ret: none
test_read proc near
    push bx
    push dx
    
    lea dx, file_2_msg0                 ; test read
    call puts
    
    mov ax, @ORD                        ; open file
    lea dx, file
    call open
    cmp ax, -1
    je test_read0
    mov handle, ax
    call opthex                         ; print the file handle
    call crlf
    
    lea dx, file_2_msg1                 ; read file
    call optstr
    mov ax, handle
    mov cx, 10
    lea bx, buf
    call read
    lea dx, buf                         ; print read content
    call putsq
    
    lea dx, file_2_msg2                 ; file pointer -= 1
    call puts
    
    mov ax, handle
    mov cx, @SEEK_CUR
    mov dx, -1
    call seek
    
    lea dx, file_2_msg1                 ; read again
    call optstr
    mov ax, handle
    mov cx, 10
    lea bx, buf
    call read
    lea dx, buf                         ; print read content
    call putsq
    
    mov ax, handle                      ; close file
    call close
    
test_read0:
    pop dx
    pop bx
    ret
test_read endp

; in: none, ret: none
test_cp proc near
    push bx
    push dx
    
    lea dx, file                        ; file => file1
    lea bx, file1
    call cpfile
    
    pop dx
    pop bx
    ret
test_cp endp

; in: none, ret: none
test_rm proc near
    push bx
    push dx
    
    lea dx, file1                       ; file1 -> file
    lea bx, file
    call mvfile
    
    pop dx
    pop bx
    ret
test_rm endp