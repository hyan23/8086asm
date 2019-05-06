; string.inc
; written by hyan23
; 2017.11.26

; in: ds:bx
; ret: ds:bx (processed)
convert proc near
    push bx
    
convert0:
    cmp byte ptr [bx], 0                ; C sytle end of string character
    jne convert1
    mov byte ptr [bx], '$'              ; convert to Dos style
    jmp convert2
convert1:
    cmp byte ptr [bx], '$'
    je convert2
    ;add bx, 1
    inc bx
    jmp convert0
    
convert2:
    pop bx
    ret
convert endp

; in: ds:bx
; ret: ds:bx (processed)
convertC proc near
    push bx
    
convertC0:
    cmp byte ptr [bx], '$'              ; Dos sytle end of string character
    jne convertC1
    mov byte ptr [bx], 0                ; convert to C style
    jmp convertC2
convertC1:
    cmp byte ptr [bx], 0
    je convertC2
    ;add bx, 1
    inc bx
    jmp convertC0
    
convertC2:
    pop bx
    ret
convertC endp

; in: ds:bx
; ret: ds:bx (processed)
lower proc near
    push ax
    push bx
    
lower0:
    mov al, [bx]
    cmp al, '$'
    je lower1
    cmp al, 'Z'
    jg lower2
    cmp al, 'A'
    jl lower2
    or al, 20h                          ; convert to lower case
    mov [bx], al
lower2:
    ; add bx, 1
    inc bx
    jmp lower0
    
lower1:
    pop bx
    pop ax
    ret
lower endp

; in: ds:bx
; ret: ds:bx (processed)
upper proc near
    push ax
    push bx
    
upper0:
    mov al, [bx]
    cmp al, '$'
    je upper1
    cmp al, 'z'
    jg upper2
    cmp al, 'a'
    jl upper2
    and al, 0dfh                        ; convert to upper case
    mov [bx], al
upper2:
    ; add bx, 1
    inc bx
    jmp upper0
    
upper1:
    pop bx
    pop ax
    ret
upper endp

; in: dx, ret: ax
strlen proc near
    push bx
    
    mov bx, dx
    xor ax, ax
strlen0:
    cmp byte ptr [bx], '$'              ; capable for both styles
    je strlen1
    cmp byte ptr [bx], 0
    je strlen1
    ; add bx, 1
    inc bx
    jmp strlen0
strlen1:
    mov ax, bx
    sub ax, dx
    
    pop bx
    ret
strlen endp

; in: dx, bx
; ret: ax: boolean
strequ proc near
    push si
    push di
    
    mov di, dx
    mov si, bx
strequ0:
    mov ah, [di]
    mov al, [si]
    cmp ah, al
    jne strequ1                         ; ah != al
    or ah, al                           ; ah = al
    cmp ah, '$'
    jz strequ2                          ; ah = al = '$'
    inc si
    inc di
    jmp strequ0
    
strequ1:
    mov ax, @FALSE
    jmp strequ3
strequ2:
    mov ax, @TRUE
strequ3:
    
    pop di
    pop si
    ret
strequ endp

; in: dx: s1, bx: s2
; ret: ax
strcmp proc near
    push si
    push di
    
    mov si, dx
    mov di, bx
    cld
strcmp0:
    mov al, [si]
    mov ah, [di]
    cmpsb
    jne strcmp1
    cmp al, '$'
    je strcmp2
    jmp strcmp0
strcmp1:
    cmp al, '$'
    je strcmp3
    cmp ah, '$'
    je strcmp4
    cmp al, ah
    jl strcmp3
    jmp strcmp4
strcmp2:
    mov ax, 0                           ; s1 = s2
    jmp strcmp5
strcmp3:                                ; s1 < s2
    mov ax, -1
    jmp strcmp5
strcmp4:                                ; s1 > s2
    mov ax, 1
    
strcmp5:
    pop di
    pop si
    ret
strcmp endp

; int: dx: src, ds:bx: dest
; ret: ds:bx (src)
strcpy proc near
    push ax
    push cx
    
    call strlen
    add ax, 1                           ; end of string
    mov cx, ax
    call memcpy                         ; sys.inc
    
    pop cx
    pop ax
    ret
strcpy endp

; in: dx: src, ds:bx: dest
; ret: ds:bx (dest+src)
concat proc near
    push ax
    push dx
    
    mov dx, bx                          ; dx
    call strlen                         ; bx = bx+strlen(bx)
    add bx, ax                          ; strcpy
    pop dx
    call strcpy
    
    pop ax
    ret
concat endp

; in: dx: s, bx: pattern
; ret: ax
index proc near
    push si
    push di
    
    mov di, dx                          ; s
    mov si, bx                          ; pattern
index0:
    cmp byte ptr [si], '$'              ; find
    je index2
    cmp byte ptr [di], '$'              ; not find
    je index3
    
    mov al, [si]                        ; compare characters
    cmp al, [di]
    jne index1
    ;add si, 1                           ; inc pattern pointer
    inc si
    jmp index5
index1:
    mov si, bx                          ; rewind pattern pointer
index5:
    ;add di, 1                           ; inc s pointer
    inc di
    jmp index0
    
index2:
    push dx                             ; di - strlen(pattern) - dx
    mov dx, bx
    call strlen
    pop dx
    sub di, ax
    sub di, dx
    mov ax, di
    jmp index4
index3:
    mov ax, -1                          ; -1
index4:
    
    pop di
    pop si
    ret
index endp

; in: ds:bx, cx: length
; ret: ds:bx (random string)
rands proc near
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    jmp rands0
    
rands1: db 'abcdefghijklmnopqrstuvwxyz'
    db 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    alphabet_length equ $-rands1
rands0:
    mov di, bx
    mov si, offset rands1
rands2:
    push cx
    
    call rand                           ; util.inc
    xor dx, dx
    mov cx, alphabet_length
    div cx
    mov bx, dx                          ; random index
    mov al, cs:[si+bx]
    mov [di], al
    
    pop cx
    inc di
    loop rands2
    mov byte ptr [di], '$'              ; terminate string
    
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
rands endp