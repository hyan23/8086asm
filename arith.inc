; arith.inc
; written by hyan23
; 2016.10.24

; in: ax, ret: ax
bcd2dec proc near
    push cx
    push dx
    
    xor dx, dx                          ; sum
    mov cx, 4
bcd2dec0:
    push cx
    push ax                             ; sum*=10
    mov ax, dx
    mov cx, 10
    mul cx
    mov dx, ax
    pop ax
    mov cl, ah                          ; cx=left-most 4bits
    push ax
    mov al, cl
    mov cl, 4
    shr al, cl
    xor cx, cx
    mov cl, al
    pop ax
    add dx, cx                          ; sum+=cx
    mov cl, 4                           ; shift 4bits left ax
    shl ax, cl
    pop cx
    loop bcd2dec0
    
    mov ax, dx
    pop dx
    pop cx
    ret
bcd2dec endp

;           dx    ax
; *         cx    bx
; --------------------
;           bahi balo   (bx*ax)
;      bdhi bdlo        (bx*dx)
;      cahi calo        (cx*ax)
; cdhi cdlo             (cx*dx)
; dx   ax   cx   bx     (dx:ax * cx*bx)

; in: dx:ax * cx:bx
; ret: dx:ax:cx:bx
mul32 proc near
    push si
    push di
    
    mov di, dx
    mov si, ax
    
    mul bx                              ; bx*ax
    push ax
    push dx
    
    mov ax, di                          ; bx*dx
    mul bx
    pop bx
    add ax, bx                          ; bdlo+bahi
    adc dx, 0                           ; bdhi+cf
    push dx
    push ax
    
    mov ax, si                          ; cx*ax
    mul cx
    pop bx
    add ax, bx                          ; calo+bdlo
    pop bx                              ; cahi+bdhi+cf
    adc dx, bx
    push ax
    push dx
    
    mov ax, di                          ; cx*dx
    mul cx
    pop di
    add ax, di                          ; cdlo+cahi
    adc dx, 0                           ; cdhi+cf
    
    pop cx                              ; form final product dx:ax:cx:bx
    pop bx
    
    pop di
    pop si
    ret
mul32 endp

; 00:dx / cx = q1 ... r1
; r1:ax / cx = q2 ... r2
; q1:q2 ... r2

; in: dx:ax / cx
; ret: dx:ax ... bx
div32 proc near                         ; 32-bit division
    mov bx, dx
    ; bx:ax / cx
    ; bx:ax ... dx
    xchg ax, bx
    xor dx, dx
    div cx
    xchg ax, bx
    div cx
    push dx
    mov dx, bx
    pop bx
    
    ret
div32 endp