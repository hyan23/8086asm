; sys.inc
; written by hyan23
; 2017.11.26

; TODO: alloc free

; in: ax.(0-7): return value
; ret: none
exit proc near
    mov ah, 4ch                         ; exit with return value
    int 21h
exit endp

; in: dx: size, ret: none
reside proc near
    int 27h                             ; terminate and reside
reside endp

; in: none, ret: ax: K
memory proc near
    int 12h                             ; get memory size(in K)
memory endp

; in: none
; ret: dx: year, ah: month, al: day 
date proc near
    push cx
    
    mov ah, 2ah                         ; get system date
    int 21h
    mov ax, dx
    mov dx, cx
    
    pop cx
    ret
date endp

; in: none
; ret: dx: hour, ah: minute, al: second
time proc near
    push cx
    
    mov ah, 2ch                         ; get system time
    int 21h
    mov al, dh
    xor dx, dx
    mov dl, ch
    mov ah, cl
    
    pop cx
    ret
time endp

; in; cx
; ret: dx:ax
get_interrupt proc near
    push bx
    push es
    
    mov ax, cx                          ; get specified interrupt vector
    mov ah, 35h
    int 21h
    mov dx, es
    mov ax, bx
    
    pop es
    pop bx
    ret
get_interrupt endp

; in: cx: intnum, dx:ax
; ret: none
set_interrupt proc near
    push ax
    push dx
    push ds
    
    mov ds, dx                          ; replace interrupt handler
    mov dx, ax
    mov ax, cx
    mov ah, 25h
    int 21h
    
    pop ds
    pop dx
    pop ax
    ret
set_interrupt endp

; in: none, ret: ax
pspseg proc near
    push bx
    
    mov ah, 62h                         ; get program segment prefix
    int 21h
    mov ax, bx
    
    pop bx
    ret
pspseg endp

; in: none, ret: ax
cmdlen proc near
    push ds
    
    call pspseg
    mov ds, ax
    xor ax, ax
    mov al, ds:[80h]                    ; command line arguments length
                                        ; => ' arg1 arg2 arg3...', cr
                                        ; the length counts the first space and 
    pop ds                              ; does not count the ending cr
    ret
cmdlen endp

; in: ds:bx: buf
; ret: ds:bx
cmdline proc near
    push ax
    push bx
    push cx
    push si
    push di
    push ds
    
    mov di, bx                          ; dest
    
    call pspseg                         ; src
    mov ds, ax
    mov si, 82h
    
    mov dx, bx                          ; length
    call cmdlen
    test ax, ax                         ; 0?
    jz cmdline0
    sub ax, 1
    mov cx, ax
    
    cld                                 ; copy string
    rep movsb
    xor ax, ax
cmdline0:
    mov bx, ax
    mov byte ptr es:[di+bx], '$'        ; terminate string using '$'
    
    pop ds
    pop di
    pop si
    pop cx
    pop bx
    pop ax
    ret
cmdline endp

; in: dx, ds:bx: (buf)
; ret: ax: increased dx, ds:bx a possible argument
; [ax]='$' or strlen(buf)=0 indicates that all args have been extracted
arg proc near
    push cx
    push si
    push di
    
    mov si, dx
    mov di, bx
    mov cx, 0                           ; handle redundant spaces
arg0:
    mov al, [si]
    cmp al, '$'
    je arg1
    cmp al, ' '                         ; al=' ' AND cx=1, end
    jne arg2
    cmp cx, 1
    je arg1
    jmp arg3
    
arg2:
    mov cx, 1                           ; copy to buffer
    mov [di], al
    ;add di, 1
    inc di
arg3:
    ;add si, 1
    inc si
    jmp arg0
    
arg1:                                   ; terminate string
    mov byte ptr [di], '$'
    mov ax, si
    
    pop di
    pop si
    pop cx
    ret
arg endp

; in: none, ret: ax
drive proc near
    mov ah, 19h                         ; get drive number
    int 21h
    xor ah, ah
    ret
drive endp

; in: ds:bx (buf)
; ret: ds:bx
pwd proc near
    push ax
    push bx
    push dx
    push si
    
    call drive                          ; drive number, 0: 'c', 1: 'd', etc.
    add ax, 41h
    mov [bx], al
    add bx, 1
    mov byte ptr [bx], ':'              ; ":\\"
    add bx, 1
    mov byte ptr [bx], '\'
    add bx, 1
    mov byte ptr [bx], '\'
    add bx, 1
    
    mov si, bx                          ; get pwd
    mov dl, 0
    mov ah, 47h
    int 21h
    
    pop si
    pop dx
    pop bx
    pop ax
    ret
pwd endp

;
alloc proc near
    ; to be implemented
    ret
alloc endp

;
free proc near
    ; to be implemented
    ret
free endp

; in: dx: src, ds:bx: dest, cx: bytes to copy
; ret: ds:bx
memcpy proc near
    push si
    push di
    
    mov si, dx
    mov di, bx
    
    test cx, 1h                         ; odd?
    jz memcpy0
    cld
    movsb                               ; move first byte
    
memcpy0:
    shr cx, 1
    cld
    rep movsw                           ; move words
    
    pop di
    pop si
    ret
memcpy endp

; Bit 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
;     X  X  X  X  OF DF IF TF SF ZF X  AF X  PF X  CF
; X  -- Unused
; OF -- Overflow flag
; DF -- Direction flag      ---+
; IF -- Interrupt enable flag  | Control flags
; TF -- Trap flag           ---+
; SF -- Sign flag
; ZF -- Zero flag
; AF -- Auxiliary carry flag
; PF -- Parity flag
; CF -- Carry flag

; in: none, ret: none
dump proc near
    jmp dp0
DUMP_REG macro label, ofset, addend
    push ax
    push dx
    
    lea dx, label
    call optstr                         ; iopt.inc
    mov ax, [bp + ofset]
    add ax, addend
    ;call opthexw
    call opthex                         ; iopt.inc
    
    pop dx
    pop ax
endm

DUMP_FLAG macro label, ofset, flag
LOCAL L0
    push ax
    push dx
    
    mov ax, [bp + ofset]
    lea dx, label
    test ax, 1 SHL flag
    jnz L0
    add dx, 4
L0:
    call optstr                         ; iopt.inc
    
    pop dx
    pop ax
endm

lip:    db  'IP: ', '$'                 ; labels
lbp:    db  ' BP: ', '$'
lsp:    db  ' SP: ', '$'
lflags: db  'FLAGS: ', '$'
lax:    db  'AX: ', '$'
lbx:    db  ' BX: ', '$'
lcx:    db  ' CX: ', '$'
ldx:    db  ' DX: ', '$'
lsi:    db  ' SI: ', '$'
ldi:    db  ' DI: ', '$'
lcs:    db  'CS: ', '$'
lss0:   db  ' SS: ', '$'
lds0:   db  ' DS: ', '$'
les0:   db  ' ES: ', '$'

lof:    db  'OF $of ', '$'
ldf:    db  'DF $df ', '$'
lif:    db  'IF $if ', '$'
ltf:    db  'TF $tf ', '$'
lsf:    db  'SF $sf ', '$'
lzf:    db  'ZF $zf ', '$'
laf:    db  'AF $af ', '$'
lpf:    db  'PF $pf ', '$'
lcf:    db  'CF$ cf', '$'
dp0:
    push bp
    mov bp, sp
    push sp
    pushf
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push cs
    push ss
    push ds
    push es
    push ds
oip     equ     1 * @WORD               ; offsets
obp     equ     0
osp     equ     -1 * @WORD
oflags  equ     -2 * @WORD
oax     equ     -3 * @WORD
obx     equ     -4 * @WORD
ocx     equ     -5 * @WORD
odx     equ     -6 * @WORD
osi     equ     -7 * @WORD
odi     equ     -8 * @WORD
ocs     equ     -9 * @WORD
oss     equ     -10 * @WORD
ods     equ     -11 * @WORD
oes     equ     -12 * @WORD
    push ds
    mov ax, cs
    mov ds, ax
    
    DUMP_REG lcs, ocs, 0h               ; dump segment registers
    DUMP_REG lss0, oss, 0h
    DUMP_REG lds0, ods, 0h
    DUMP_REG les0, oes, 0h
    call crlf
    
    DUMP_REG lax, oax, 0h               ; dump general registers
    DUMP_REG lbx, obx, 0h
    DUMP_REG lcx, ocx, 0h
    DUMP_REG ldx, odx, 0h
    DUMP_REG lsi, osi, 0h
    DUMP_REG ldi, odi, 0h
    call crlf
    
    DUMP_REG lip, oip, 0h               ; dump pointer registers
    DUMP_REG lsp, osp, 3 * @WORD
    call crlf
    
    DUMP_REG lflags, oflags, 0h         ; dump flags
    call crlf
    
    DUMP_FLAG lof, oflags, 11
    DUMP_FLAG ldf, oflags, 10
    DUMP_FLAG lif, oflags, 9
    DUMP_FLAG ltf, oflags, 8
    DUMP_FLAG lsf, oflags, 7
    DUMP_FLAG lzf, oflags, 6
    DUMP_FLAG laf, oflags, 4
    DUMP_FLAG lpf, oflags, 2
    DUMP_FLAG lcf, oflags, 0
    call crlf
    
    pop ds
    mov sp, bp
    pop bp
    ret
dump endp