; string.asm
; written by hyan23
; 2017.11.28

string_data segment
    ; test string
    string_s db ' **test string** ', '$'
    
    ; test_lower_upper
    string_0_msg0 db 'test lower upper', '$'
    string_0_msg1 db 'original s=', '$'
    string_0_msg2 db 'lower s=', '$'
    string_0_msg3 db 'upper s=', '$'
    
    string_0_s db 'hElLo wOrlD abc#123', '$'
    
    ; test strequ
    string_1_msg0 db 'test strequ', '$'
    string_1_msg1 db 's1=', '$'
    string_1_msg2 db 's2=', '$'
    string_1_msg3 db 's3=', '$'
    string_1_msg4 db 's1=s2?: ', '$'
    string_1_msg5 db 's2=s3?: ', '$'
    string_1_msg6 db 's1=s3?: ', '$'
    
    string_1_s1 db ' hello', '$'
    string_1_s2 db 'hello ', '$'
    string_1_s3 db ' hello', '$'
    
    ; test strcpy
    string_2_msg0 db 'test strcpy', '$'
    string_2_msg1 db 's: ', '$'
    string_2_msg2 db 'buf: ', '$'
    string_2_msg3 db 'copy s to buf s=>buf', '$'
    
    string_2_s db 'AAbbCCddEEffGG', '$'
    string_2_buf db 100 dup('$')
    
    ; test concat
    string_3_msg0 db 'test concat', '$'
    string_3_msg1 db 's: ', '$'
    string_3_msg2 db 'buf: ', '$'
    string_3_msg3 db 'buf<=s', '$'
    
    string_3_s db 'na', '$'
    string_3_buf db 'ba', 100 dup('$')
    
    ; test index
    string_4_msg0 db 'test index', '$'
    string_4_msg1 db 's: ', '$'
    string_4_msg2 db 'p1: ', '$'
    string_4_msg3 db 'p2: ', '$'
    string_4_msg4 db 'p3: ', '$'
    string_4_msg5 db 's.index p1: ', '$'
    string_4_msg6 db 's.index p2: ', '$'
    string_4_msg7 db 's.index p3: ', '$'
    
    string_4_s db 'hello world hel wor helloo wordl world', '$'
    string_4_p1 db 'hel', '$'
    string_4_p2 db 'ld', '$'
    string_4_p3 db 'dfe', '$'
    
    ; test rands
    string_5_msg0 db 'test rands', '$'
    
    string_5_buf db 100 dup('$')
string_data ends

; in: none, ret: none
test_string proc near
    push ds
    push es
    
    mov ax, string_data
    mov ds, ax
    mov es, ax
    assume ds:string_data
    assume es:string_data
    
    lea dx, string_s
    call puts
    call pause
    ; Your code begins here
    
    call test_lower_upper               ; test lower upper
    call pause
    
    call test_strequ                    ; test strequ
    call pause
    
    call test_strcpy                    ; test strcpy
    call pause
    
    call test_concat                    ; test concat
    call pause
    
    call test_index                     ; test index
    call pause
    
    call test_rands                     ; test rands
    call pause
    
    pop es
    pop ds
    ret
test_string endp

; Your sub-routine begins here

; in: none, ret: none
test_lower_upper proc near
    push bx
    push dx
    
    lea dx, string_0_msg0               ; test lower upper
    call puts
    
    lea dx, string_0_msg1               ; print original string
    call optstr
    lea dx, string_0_s
    call putsq
    
    lea bx, string_0_s                  ; lower
    call lower
    
    lea dx, string_0_msg2               ; print lower string
    call optstr
    lea dx, string_0_s
    call putsq
    
    lea bx, string_0_s                  ; upper
    call upper
    
    lea dx, string_0_msg3               ; print upper string
    call optstr
    lea dx, string_0_s
    call putsq
    
    pop dx
    pop bx
    ret
test_lower_upper endp

; in: none, ret: none
test_strequ proc near
    push bx
    push dx
    
    lea dx, string_1_msg0               ; test strequ
    call puts
    
    lea dx, string_1_msg1               ; print s1
    call optstr
    lea dx, string_1_s1
    call putsq
    
    lea dx, string_1_msg2               ; print s2
    call optstr
    lea dx, string_1_s2
    call putsq
    
    lea dx, string_1_msg3               ; print s3
    call optstr
    lea dx, string_1_s3
    call putsq
    
    lea dx, string_1_msg4               ; s1 = s2?
    call optstr
    lea dx, string_1_s1
    lea bx, string_1_s2
    call strequ
    call optdec
    call crlf
    
    lea dx, string_1_msg5               ; s2 = s3?
    call optstr
    lea dx, string_1_s2
    lea bx, string_1_s3
    call strequ
    call optdec
    call crlf
    
    lea dx, string_1_msg6               ; s1 = s3?
    call optstr
    lea dx, string_1_s1
    lea bx, string_1_s3
    call strequ
    call optdec
    call crlf
    
    pop dx
    pop bx
    ret
test_strequ endp

; in: none, ret: none
test_strcpy proc near
    push bx
    push dx
    
    lea dx, string_2_msg0               ; test strcpy
    call puts
    
    lea dx, string_2_msg1               ; print s
    call optstr
    lea dx, string_2_s
    call putsq
    
    lea dx, string_2_msg2               ; print buf
    call optstr
    lea dx, string_2_buf
    call putsq
    
    lea dx, string_2_msg3               ; copy s to buf
    call puts
    
    lea dx, string_2_s
    lea bx, string_2_buf
    call strcpy
    
    lea dx, string_2_msg2               ; print buf
    call optstr
    lea dx, string_2_buf
    call putsq
    
    pop dx
    pop bx
    ret
test_strcpy endp

; in: none, ret: none
test_concat proc near
    push bx
    push dx
    
    lea dx, string_3_msg0               ; test concat
    call puts
    
    lea dx, string_3_msg1               ; print s
    call optstr
    lea dx, string_3_s
    call putsq
    
    lea dx, string_3_msg2               ; print buf
    call optstr
    lea dx, string_3_buf
    call putsq
    
    lea dx, string_3_msg3               ; buf <= s
    call puts
    
    lea dx, string_3_s
    lea bx, string_3_buf
    call concat
    
    lea dx, string_3_msg2               ; print buf
    call optstr
    lea dx, string_3_buf
    call putsq
    
    lea dx, string_3_msg3               ; buf <= s
    call puts
    
    lea dx, string_3_s
    lea bx, string_3_buf
    call concat
    
    lea dx, string_3_msg2               ; print buf
    call optstr
    lea dx, string_3_buf
    call putsq
    
    pop dx
    pop bx
    ret
test_concat endp

; in: none, ret: none
test_index proc near
    push bx
    push dx
    
    lea dx, string_4_msg0               ; test index
    call puts
    
    lea dx, string_4_msg1               ; print s
    call optstr
    lea dx, string_4_s
    call putsq
    
    lea dx, string_4_msg2               ; print p1
    call optstr
    lea dx, string_4_p1
    call putsq
    
    lea dx, string_4_msg3               ; print p2
    call optstr
    lea dx, string_4_p2
    call putsq
    
    lea dx, string_4_msg4               ; print p3
    call optstr
    lea dx, string_4_p3
    call putsq
    
    lea dx, string_4_msg5               ; s.index p1
    call optstr
    
    lea dx, string_4_s
    lea bx, string_4_p1
    call index
    call optdecs
    call crlf
    
    lea dx, string_4_msg6               ; s.index p2
    call optstr
    
    lea dx, string_4_s
    lea bx, string_4_p2
    call index
    call optdecs
    call crlf
    
    lea dx, string_4_msg7               ; s.index p3
    call optstr
    
    lea dx, string_4_s
    lea bx, string_4_p3
    call index
    call optdecs
    call crlf
    
    pop dx
    pop bx
    ret
test_index endp

test_rands proc near
    push dx
    
    lea dx, string_5_msg0               ; test rands
    call puts
    
    lea bx, string_5_buf                ; print string
    mov cx, 20
    call rands
    lea dx, string_5_buf
    call putsq
    
    pop dx
    ret
test_rands endp