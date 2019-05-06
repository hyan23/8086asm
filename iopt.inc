; iopt.inc
; written by hyan23
; 2016.10.24

; in: none, ret: none
clear proc near
	push ax
    mov ax, 03h                         ; ah: 0h: set video mode
    int 10h                             ; al: 03h: 80*25*16, text
	pop ax
    ret
clear endp

; in: ah: row, al: column
; ret: none
cursor proc near
    push bx
    push dx
    push ax
    mov dx, ax
    mov bh, 0
    mov ah, 02h                         ; set cursor position
    int 10h
    pop ax
    pop dx
    pop bx
    ret
cursor endp

; in: al: char, bl: attribute
; ret: none
putc proc near
    push bx
    push cx
    push ax
	mov bl,ah
    mov ah, 09h                         ; put char by cursor
    mov bh, 0    
    mov cx, 1
    int 10h
    pop ax
    pop cx
    pop bx
    ret
putc endp

; in: none, ret: none
space proc near
    push dx
    push ax
    mov dl, 20h                         ; print char
    mov ah, 02h
    int 21h
    pop ax
    pop dx
    ret
space endp

; in: none, ret: none
crlf proc near
    push dx
    push ax
    mov ah, 02h                         ; cr
    mov dl, 0dh
    int 21h
    mov dl, 0ah                         ; lf
    int 21h
    pop ax
    pop dx
    ret
crlf endp

; in: none, ret: ax
getch proc near
		
    mov ah, 01h                         ; input char
    int 21h
    xor ah, ah   
	
    ret
getch endp

; in: ds:bx, ret: ds:bx
gets proc near
    push bx
    push ax
	
gets0:
    mov ah, 01h                         ; input char
    int 21h
    cmp al, 0dh                         ; cr?
    je gets1
    mov [bx], al
    inc bx
    jmp gets0
gets1:
    mov byte ptr [bx], '$'              ; terminate inputed string
    
	pop ax
    pop bx
    ret
gets endp

; in: ds:bx, ret: ds:bx
iptpwd proc near
    push bx
    push ax
	
iptp0:
    mov ah, 08h                         ; input char, without echo
    int 21h
    cmp al, 0dh                         ; cr?
    je iptp1
    mov [bx], al
    inc bx
    jmp iptp0
iptp1:
    call crlf
    mov byte ptr [bx], '$'              ; terminate inputed password
    
	pop ax
    pop bx
    ret
iptpwd endp

; in: none, ret: none
await proc near
    push dx
    push ds
	push ax
	
    jmp await0
    
await1: db ' - Press Enter to continue - ', '$'
await0:
    mov ax, cs                          ; print tip
    mov ds, ax
    mov dx, offset await1
    call puts
await3:
    mov ah, 08h                         ; input char no echo
    int 21h
    cmp al, 0dh
    je await2
    jmp await3
    
await2:
    call crlf
	
	pop ax
    pop ds
    pop dx
    ret
await endp

; in: al, ret: none
optch proc near
    push dx 
	push ax
	
    mov dl, al                          ; output char
    mov ah, 02h
    int 21h  

	pop ax
    pop dx
    ret
optch endp

; in: none, ret: ax
; behavior: <ch><Space>|<Enter>
iptch proc near
    push dx
    push di
   
	
    mov di, 0                           ; flag
iptc0:
    mov ah, 01h                         ; input char
    int 21h
    and ax, 0ffh
    cmp ax, 0dh                         ; cr?
    jz iptc1
    cmp ax, 20h                         ; ' '?
    jz iptc1
    mov dx, ax
    mov di, 1
    jmp iptc0
    
iptc1:
    test di, di
    jz iptc0
    
    mov ax, dx	
	
    pop di
    pop dx
    ret
iptch endp

; in: dx, ret: none
optstr proc near
	push ax
    mov ah, 09h                         ; print string
    int 21h
    pop ax
    ret
optstr endp

; in: dx, ret: none
optstrq proc near
    mov al, '"'                         ; enclose with quotes
    call optch
    call optstr
    mov al, '"'
    call optch
    
    ret
optstrq endp

; in: dx, ret:none
puts proc near
    call optstr                         ; print string
    call crlf                           ; print cr lf
    ret
puts endp

; in: dx, ret:none
putsq proc near
    call optstrq                        ; print string enclosed by quotes
    call crlf                           ; print cr lf
    ret
putsq endp

; in: ax, ret: none
optdec proc near
    push bx
    push cx
    push dx
    
    xor cx, cx                          ; count
    mov bx, 10                          ; /= 10
optd1:
    xor dx, dx
    div bx
    push dx
    inc cx
    cmp ax, 0
    jnz optd1
    
optd2:                                  ; print digit
    pop ax
    add ax, 30h
    mov dl, al
    mov ah, 02h
    int 21h
    loop optd2
    
    pop dx
    pop cx
    pop bx
    ret
optdec endp

; in: dx:ax
; ret: none
optdecdw proc near                       ; dword
    push bx
    push cx
    push dx
    
    xor cx, cx                          ; count
optddw0:                                 ; /= 10
    cmp dx, ax
    jne optddw1
    test ax, ax
    jz optddw2
optddw1:
    push cx
    mov cx, 10
    call div32
    pop cx
    add cx, 1
    push bx
    jmp optddw0
    
optddw2:                                 ; print digit
    pop ax
    add ax, 30h
    mov dl, al
    mov ah, 02h
    int 21h
    loop optddw2
    
    pop dx
    pop cx
    pop bx
    ret
optdecdw endp

; in: none, ret: ax
iptdec proc near
    push bx
    push cx
    push dx
    
    xor bx, bx                          ; sum
iptd5:
    mov ah, 01h                         ; input char
    int 21h
    
    cmp al, 0dh                         ; cr?
    jz iptdexit
    cmp al, 20h                         ; ' '?
    jz iptdexit
iptd7:
    cmp al, '0'                         ; < '0' OR > '9'
    jb iptderr
    cmp al, '9'
    ja iptderr
    and ax, 0fh                         ; atoi
    xchg ax, bx                         ; bx: sum
    mov cx, 10                          ; sum*10+input
    mul cx
    add bx, ax
    jmp iptd5
    
iptderr:
iptdexit:
    mov ax, bx
    pop dx
    pop cx
    pop bx
    ret
iptdec endp

; in: ax, ret: none
optdecs proc near
    push bx
    push cx
    push dx
    push si
    
    mov si, 0                           ; sign
    and ax, ax
    jns optds0                          ; positive integer
    mov si, 1
    neg ax
optds0:
    xor cx, cx                          ; count
    mov bx, 10                          ; /= 10
optds1:
    xor dx, dx
    idiv bx
    push dx
    inc cx
    cmp ax, 0
    jnz optds1
    
    test si, si                         ; if si:print '-'
    jz optds2
    mov dl, 2dh
    mov ah, 2
    int 21h
    
optds2:                                 ; print digit
    pop ax
    add ax, 30h
    mov dl, al
    mov ah, 02h
    int 21h
    loop optds2
    
    pop si
    pop dx
    pop cx
    pop bx
    ret
optdecs endp

; in: none, ret: ax
iptdecs proc near
    push bx
    push cx
    push dx
    push si
    push di
    
    xor bx, bx                          ; sum
    mov si, 0                           ; sign
    mov di, 0                           ; flag
iptd2:
    mov ah, 01h                         ; input char
    int 21h
    
    cmp al, 0dh                         ; cr?
    jz iptdexit1
    cmp al, 20h                         ; ' '?
    jz iptdexit1
    
    cmp al, 2dh                         ; '-', negative
    jnz iptd0
    mov si, 1                           ; set bit
    jmp iptd1
    
iptd0:
    mov di, 1                           ; flag=1
    cmp al, '0'                         ; < '0' OR > '9'
    jb iptderr1
    cmp al, '9'
    ja iptderr1
    and ax, 0fh                         ; atoi
    xchg ax, bx                         ; bx: sum
    mov cx, 10                          ; sum*10+input
    mul cx
    add bx, ax
iptd1:
    jmp iptd2
iptdexit1:
    test di, di                         ; cr|' '?
    jz iptd2
iptderr1:
    test si, si                         ; negative?
    jz iptd3                            ; to two's complement
    neg bx
iptd3:
    
    mov ax, bx
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    ret
iptdecs endp

; in: al(3-0), ret: al
hex2ascii proc near
    and al, 0fh
    cmp al, 0ah
    jb h2a0
    sub al, 0ah
    add al, 61h                         ; 'a', 'A':41h
    jmp h2a1
h2a0:
    add al, 30h                         ; '0'
h2a1:
    ret
hex2ascii endp

; in: ax, ret: none
opt0x proc near
    push dx
    
    mov ah, 02h                         ; print '0x'
    mov dl, '0'
    int 21h
    mov dl, 'x'
    int 21h
    
    pop dx
    ret
opt0x endp

; in: ax, ret: none
opthex proc near
    push cx
    push dx
    
    mov cx, 4
opth0:
    push cx
    
    push ax                             ; print bits.(15-12)
    mov cl, 12
    shr ax, cl
    call hex2ascii
    mov dl, al
    mov ah, 02h
    int 21h
    pop ax
    mov cl, 4                           ; ax <<= 4
    shl ax, cl
    
    pop cx
    loop opth0
    
    pop dx
    pop cx
    ret
opthex endp

; in: al; ret: none
opthexb proc near                       ; byte
    push ax
    call opt0x
    pop ax
    xor ah, ah
    call opthex
    
    ret
opthexb endp

; in: ax, ret: none
opthexw proc near                       ; word
    push ax
    call opt0x
    pop ax
    call opthex
    
    ret
opthexw endp

; in: dx:ax, ret: none
opthexdw proc near                      ; dword
    push ax
    call opt0x
    pop ax
    xchg dx, ax
    call opthex
    xchg dx, ax
    call opthex
    
    ret
opthexdw endp

; in: dx:ax:cx:bx, ret: none
opthexqw proc near                      ; qword
    push ax
    call opt0x
    pop ax
    xchg dx, ax
    call opthex
    xchg dx, ax
    call opthex
    xchg dx, cx
    xchg ax, bx
    xchg dx, ax
    call opthex
    xchg dx, ax
    call opthex
    xchg dx, cx
    xchg ax, bx
    
    ret
opthexqw endp

; in: al, ret: al
ascii2hex proc near
    test al, 10h                        ; bit4!=1?
    jz a2h0
    sub al, 30h
    jmp a2h1
a2h0:
    sub al, 61h
    add al, 0ah
a2h1:
    ret
ascii2hex endp

; in: none, ret: ax
ipthex proc near
    push bx
    push cx
    push dx
    push di
    
    xor bx, bx                          ; sum
    mov di, 0                           ; flag
ipth2:
    mov ah, 01h                         ; input char
    int 21h
    cmp al, 0dh                         ; cr?
    jz ipthexit
    cmp al, 20h                         ; ' '?
    jz ipthexit
ipth0:
    mov di, 1                           ; flag=1
    
    cmp al, 'a'                         ; '0' - '9' | 'a' - 'f'
    jb ipth4
    cmp al, 'f'
    ja iptherr
    jmp ipth5
ipth4:
    cmp al, '0'
    jb iptherr
    cmp al, '9'
    ja iptherr
    
ipth5:
    call ascii2hex                      ; atoi
    and ax, 0fh
    xchg ax, bx                         ; bx: sum
    mov cx, 16                          ; sum*16+input
    mul cx
    add bx, ax
    jmp ipth2
    
ipthexit:
    test di, di
    jz ipth2
    
iptherr:
    mov ax, bx
    pop di
    pop dx
    pop cx
    pop bx
    ret
ipthex endp

; in: ax, ret: none
optbin proc near
    push cx
    push dx
    
    mov cx, 16
optb0:
    push cx
    
    push ax
    mov cl, 15                          ; print bits.15
    shr ax, cl
    add al, 30h
    mov dl, al
    mov ah, 02h
    int 21h
    pop ax
    shl ax, 1                           ; ax <<= 1
    
    pop cx
    loop optb0
    
    pop dx
    pop cx
    ret
optbin endp

; in: none, ret: ax
iptbin proc near
    push bx
    push cx
    push dx
    push di
    
    xor bx, bx                          ; sum
    mov di, 0                           ; flag
iptb2:
    mov ah, 01h                         ; input char
    int 21h
    cmp al, 0dh                         ; cr?
    jz iptbexit
    cmp al, 20h                         ; ' '?
    jz iptbexit
iptb0:
    mov di, 1                           ; flag=1
    
    cmp al, '0'                         ; '0' OR '1'
    jnz iptb4
    jmp iptb5
iptb4:
    cmp al, '1'
    jnz iptberr
    
iptb5:
    sub al, 30h                         ; atoi
    and ax, 0fh
    xchg ax, bx                         ; bx: sum
    mov cx, 2                           ; sum*2+input
    mul cx
    add bx, ax
    jmp iptb2
    
iptbexit:
    test di, di
    jz iptb2
    
iptberr:
    mov ax, bx
    pop di
    pop dx
    pop cx
    pop bx
    ret
iptbin endp

; in: ax, ret: none
optbcd proc near
    push cx
    push dx
    push si
    
    mov cx, 4                           ; 4 digits
    mov si, 0                           ; flag
optbcd0:
    push cx
    mov dl, ah                          ; left-most 4bits
    and dl, 0f0h
    mov cl, 4
    shr dl, cl
    cmp dl, 0
    jne optbcd2
    cmp si, 0
    je optbcd3
optbcd2:
    mov si, 1
    or dl, 30h                          ; itoa
    push ax
    mov ah, 02h                         ; print char
    int 21h
    pop ax
optbcd3:
    shl ax, cl                          ; shift left 4bits
    pop cx
    loop optbcd0
    
    pop si
    pop dx
    pop cx
    ret
optbcd endp

; in: none, ret: ax
iptbcd proc near
    push cx
    push dx
    
    xor cx, cx                          ; count
    xor dx, dx                          ; result
iptbcd0:
    mov ah, 01h                         ; input char
    int 21h
    
    cmp al, 0dh                         ; '\n'?
    je iptbcdexit
    cmp al, 20h                         ; ' '?
    je iptbcdexit
    
    cmp al, '0'                         ; no digit
    jl iptbcderr
    cmp al, '9'
    jg iptbcderr                        ; any of above terminates input
    
    cmp cx, 4                           ; exceeds 4 digits
    jge iptbcdov                        ; ignore inputed char
    
    and al, 0fh                         ; atoi
    push cx
    mov cl, 4                           ; shift the result left 4bits
    shl dx, cl
    pop cx
    or dl, al                           ; place inputed digit on right-most 4bits
iptbcdov:
    inc cx
    jmp iptbcd0
    
iptbcderr:
iptbcdexit:
    mov ax, dx
    pop dx
    pop cx
    ret
iptbcd endp

; in: dx, cx: count
printArray proc near
    push bx
    push cx
    
    mov bx, dx
pa21:
    mov ax, [bx]                        ; "%u "
    call optdec
    call space
    add bx, @WORD
    loop pa21
    
    pop cx
    pop bx
    ret
printArray endp

; in: dx, cx: count
printArrayS proc near
    push bx
    push cx
    push dx
	
    mov bx, dx
pa11:
    mov ax, [bx]                        ; "%d "
    call optdecs
    call space
    add bx, @WORD
    loop pa11
    
	pop dx
	pop cx
    pop bx
    ret
printArrayS endp