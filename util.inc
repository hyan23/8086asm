; util.inc
; written by hyan23
; 2016.10.24

; in: none, ret: none
srand proc near
    jmp srd1
srdseed dw ?
srd1:
    push ax
    push cx
    push dx
    
    mov ah, 2ch                         ; get time
    int 21h
    xor ax, ax                          ; hour * 60 * 60
    mov al, ch
    xor cx, cx
    mov cl, 60
    mul cl
    mul cx
    mov cs:[srdseed], ax
    
    mov ah, 2ch                         ; minute * 60
    int 21h
    mov al, cl
    mov cl, 60
    mul cl
    add cs:[srdseed], ax
    
    xor dh, dh                          ; second
    add cs:[srdseed], dx
    
    pop dx
    pop cx
    pop ax
    ret
srand endp

; in: none, ret: ax
rand proc near
    push cx
    push dx
    
    mov ax, cs:[srdseed]
    add ax, 423 * 4 + 1
    mov cx, 813 * 2 + 1
    mul cx
    mov cs:[srdseed], ax
    
    pop dx
    pop cx
    ret
rand endp

; in: ds:bx, cx, count
; ret: ds:bx
sort proc near
bsarray     equ     -1 * @WORD
bscount     equ     -2 * @WORD
bsi         equ     -3 * @WORD
bsj         equ     -4 * @WORD
    push ax
    push bx
    push dx
    push bp
    mov bp, sp
    sub sp, 10h
    
    mov [bp + bsarray], bx              ; array
    mov [bp + bscount], cx              ; count
    
    mov word ptr [bp + bsi], 0          ; i = 0
bs0:
    mov ax, [bp + bsi]                  ; while i < count
    cmp ax, [bp + bscount]
    jge bs1
    
    mov word ptr [bp + bsj], 0          ; j = 0
    mov bx, [bp + bsarray]
bs2:
    mov ax, [bp + bsj]                  ; while j < count - i - 1
    mov dx, [bp + bscount]
    sub dx, [bp + bsi]
    sub dx, 1
    cmp ax, dx
    jge bs3 
    
    mov ax, [bx]                        ; swap array[j] and array[j + 1]
    cmp ax, [bx + @WORD]
    jle bs4
    xchg ax, [bx + @WORD]
    mov [bx], ax
bs4:
    add bx, @WORD                       ; j ++
    add word ptr [bp + bsj], 1
    jmp bs2
bs3:
    add word ptr [bp + bsi], 1          ; i ++
    jmp bs0
bs1:
    
    mov sp, bp
    pop bp
    pop dx
    pop bx
    pop ax
    ret
sort endp

; in: ds:bx array, dx left, ax right
; ret: none
swap proc near                          ; swap two elements in an array
    push ax
    push bx
    push cx
    push dx
    
    shl ax, 1
    add ax, bx
    shl dx, 1
    add dx, bx
    mov bx, ax
    mov cx, [bx]
    mov bx, dx
    xchg [bx], cx
    mov bx, ax
    mov [bx], cx
    
    pop dx
    pop cx
    pop bx
    pop ax
    ret
swap endp

; storeIndex partition(array, left, right, pivotIndex); 
partition proc near
    push bp
    mov bp, sp
    sub sp, 10h
ptarray         equ     5 * @WORD
ptleft          equ     4 * @WORD
ptright         equ     3 * @WORD
ptpivotIndex    equ     2 * @WORD
ptpivotValue    equ     -1 * @WORD
ptscoreIndex    equ     -2 * @WORD
pti             equ     -3 * @WORD
    push bx
    push dx
    
    mov ax, [bp + ptpivotIndex]         ; ptpivotValue = ptarray[ptpivotIndex]
    shl ax, 1
    add ax, [bp + ptarray]
    mov bx, ax
    mov ax, [bx]
    mov [bp + ptpivotValue], ax
    
    mov bx, [bp + ptarray]              ; swap(ptarray, ptpivotIndex, ptright)
    mov dx, [bp + ptpivotIndex]
    mov ax, [bp + ptright]
    call swap
    
    mov ax, [bp + ptleft]               ; ptscoreIndex = ptleft
    mov [bp + ptscoreIndex], ax
    
    mov ax, [bp + ptleft]               ; for pti from ptleft to ptright - 1
    mov [bp + pti], ax
pt0:
    mov ax, [bp + ptright]
    cmp [bp + pti], ax
    jge pt1
    
    mov ax, [bp + pti]                  ; if ptarray[pti] < ptpivotValue:
    shl ax, 1
    add ax, [bp + ptarray]
    mov bx, ax
    mov ax, [bx]
    cmp ax, [bp + ptpivotValue]
    jge pt2
    
    mov bx, [bp + ptarray]              ; swap(ptarray, ptscoreIndex, pti)
    mov dx, [bp + ptscoreIndex]
    mov ax, [bp + pti]
    call swap
    add word ptr [bp + ptscoreIndex], 1 ; ptscoreIndex += 1
pt2:
    add word ptr [bp + pti], 1
    jmp pt0
    
pt1:
    mov bx, [bp + ptarray]              ; swap(ptarray, ptright, ptscoreIndex)
    mov dx, [bp + ptright]
    mov ax, [bp + ptscoreIndex]
    call swap
    
    mov ax, [bp + ptscoreIndex]         ; return ptscoreIndex
    pop dx
    pop bx
    mov sp, bp
    pop bp
    ret
partition endp

; void quick_sort(array, left, right);
quick_sort proc near
    push bp
    mov bp, sp
    sub sp, 10h
qsarray         equ     4 * @WORD
qsleft          equ     3 * @WORD
qsright         equ     2 * @WORD
qspivotNewIndex equ     -1 * @WORD
    push ax
    
    mov ax, [bp + qsleft]               ; if qsleft < qsright:
    cmp ax, [bp + qsright]
    jge qs0
    
    mov ax, [bp + qsarray]              ; qspivotNewIndex = partition(qsarray, 
    push ax                             ;   qsleft, qsright, qsleft)
    mov ax, [bp + qsleft]
    push ax
    mov ax, [bp + qsright]
    push ax
    mov ax, [bp + qsleft]
    push ax
    call partition
    mov [bp + qspivotNewIndex], ax
    add sp, 4 * @WORD
    
    mov ax, [bp + qsarray]              ; quick_sort(qsarray, qsleft, 
    push ax                             ;   qspivotNewIndex - 1)
    mov ax, [bp + qsleft]
    push ax
    mov ax, [bp + qspivotNewIndex]
    sub ax, 1
    push ax
    call quick_sort
    add sp, 3 * @WORD
    
    mov ax, [bp + qsarray]              ; quick_sort(qsarray, qspivotNewIndex + 1, 
    push ax                             ;   qsright)
    mov ax, [bp + qspivotNewIndex]
    add ax, 1
    push ax
    mov ax, [bp + qsright]
    push ax
    call quick_sort
    add sp, 3 * @WORD
qs0:
    pop ax
    mov sp, bp
    pop bp
    ret
quick_sort endp

; in: ds:bx array, cx: number of elements
; ret: ds:bx
qsort proc near
    push ax
    
    push bx
    mov ax, 0
    push ax
    sub cx, 1
    push cx
    add cx, 1
    call quick_sort
    add sp, 3 * @WORD
    
    pop ax
    ret
qsort endp

; in: ds:bx array, cx: number of elements
; ret: ds:bx
reverse proc near
    push cx
    push si
    push di
    
    mov si, bx                          ; si = array[0]
    sub cx, 1                           ; di = array[cx-1]
    shl cx, 1
    add cx, bx
    mov di, cx
rv0:
    cmp si, di
    jge rv1
    mov cx, [si]                        ; swap array[si] and array[di]
    xchg cx, [di]
    mov [si], cx
    add si, @WORD
    sub di, @WORD
    jmp rv0
rv1:
    pop di
    pop si
    pop cx
    ret
reverse endp

; in: ds:bx array, cx: number of elements
; ret: ds:bx
shuffle proc near
    push ax
    push cx
    push dx
    
    mov dx, 0                           ; dx = 0
sf0:
    push dx
    xor dx, dx                          ; ax = rand() % cx
    call rand
    div cx
    mov ax, dx
    pop dx
    add ax, dx
    call swap                           ; swap array[dx] and array[dx + ax]
    add dx, 1                           ; dx ++;
    loop sf0                            ; cx --;
    
    pop dx
    pop cx
    pop ax
    ret
shuffle endp