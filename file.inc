; file.inc
; written by hyan23
; 2017.11.26

; TODO: tell mvdir cpdir

; in: dx: path
; ret: ax: boolean
mkdir proc near
    mov ah, 39h                         ; make directory
    int 21h
    jc mkdir0
    mov ax, @TRUE
    jmp mkdir1
mkdir0:
    mov ax, @FALSE
mkdir1:
    ret
mkdir endp

; in: dx: path
; ret: ax: boolean
rmdir proc near
    mov ah, 3ah                         ; remove directory
    int 21h
    jc rmdir0
    mov ax, @TRUE
    jmp rmdir1
rmdir0:
    mov ax, @FALSE
rmdir1:
    ret
rmdir endp

; in: dx: file
; ret: ax: boolean
mkfile proc near
    push cx
                                        ;               (overwrite)
    ;call exists                         ; exists, refuse to create
    ;cmp ax, @TRUE
    ;je mkf0
    
    mov ah, 3ch                         ; create file
    xor cx, cx
    int 21h
    jc mkf0
    call close                          ; close handle
    mov ax, @TRUE
    jmp mkf1
mkf0:
    mov ax, @FALSE
    
mkf1:
    pop cx
    ret
mkfile endp

; in: dx: file
; ret: ax: boolean
rmfile proc near
    mov ah, 41h                         ; delete file
    int 21h
    jc rmf0
    mov ax, @TRUE
    jmp rmf1
rmf0:
    mov ax, @FALSE
rmf1:
    ret
rmfile endp

; in: dx: file
; ret: ax: boolean
exists proc near
    call open                           ; file exists
    cmp ax, -1
    je exists0
    call close
    mov ax, @TRUE
    jmp exists1
exists0:
    mov ax, @FALSE
exists1:
    ret
exists endp

; in: dx: file, ax: mode
; ret: ax: handle
open proc near
    mov ah, 3dh                         ; open file
    int 21h
    jc open0
    jmp open1
open0:
    mov ax, -1
open1:
    ret
open endp

; in: ax: handle
; ret: ax: boolean
close proc near
    push bx                             ; close file
    mov bx, ax
    mov ah, 3eh
    int 21h
    jc close0
    mov ax, @TRUE
    jmp close1
close0:
    mov ax, @FALSE
close1:
    pop bx
    ret
close endp

; in: ax: handle
; ret: ax: properties
; TODO: include file size
properties proc near
    push bx                             ; get file properties
    push cx
    
    mov bx, ax
    mov al, 0h
    mov ah, 43h
    int 21h
    
    mov ax, cx
    pop cx
    pop bx
    ret
properties endp

; in: ax: handle, cx: bytes to read, ds:bx: buf
; ret: ax: boolean, cx: bytes read, ds:bx
read proc near
    push bx
    push dx
    
    mov dx, bx                          ; read file
    mov bx, ax
    mov ah, 3fh
    int 21h
    jc read0
    mov cx, ax
    mov ax, @TRUE
    jmp read1
read0:
    mov cx, 0
    mov ax, @FALSE
    
read1:
    pop dx
    pop bx
    ret
read endp

; in: ax: handle, cx: bytes to write, dx: data
; ret: ax: boolean, cx: bytes written
write proc near
    push bx
    
    mov bx, ax                          ; write file
    mov ah, 40h
    int 21h
    jc write0
    mov cx, ax
    mov ax, @TRUE
    jmp write1
write0:
    mov cx, 0
    mov ax, @FALSE
    
write1:
    pop bx
    ret
write endp

; in: ax: handle, cx: mode, dx: offset(signed)
; ret: ax
seek proc near
    push bx
    push cx
    push dx
    
    mov bx, ax
    mov ah, 42h
    mov al, cl
    xor cx, cx
    test dx, 8000h                      ; cx:dx: offset
    jz seek2                            ; sign extending
    not cx
seek2:
    int 21h                             ; move file pointer
                                        ; => dx:ax: cur
    jc seek0
    mov ax, @TRUE
    jmp seek1
seek0:
    mov ax, @FALSE
    
seek1:
    pop dx
    pop cx
    pop bx
    ret
seek endp

; in: ax: handle
; ret: ax: boolean
rewind proc near
    push cx
    push dx
    
    mov cx, @SEEK_SET                   ; SEEK_SET, 0
    mov dx, 0
    call seek
    
    pop dx
    pop cx
    ret
rewind endp

tell proc near
    ; to be implemented
    ret
tell endp

; in: dx: src, bx: dest
; ret: ax: boolean
; it will overwrite dest silently
mvfile proc near
    call cpfile                         ; src => dest
    cmp ax, @TRUE
    jne mvf0
    call rmfile                         ; delete src
mvf0:
    ret
mvfile endp

mvdir proc near
    ; to be implemented
    ret
mvdir endp

; in: dx: src, bx: dest
; ret: ax: boolean
; it will overwrite dest silently
cpfile proc near
cpfh0 equ -(1*@WORD)
cpfh1 equ -(2*@WORD)
    push cx
    push dx
    push ds
    push bp
    mov bp, sp
    sub sp, 110h                        ; alloc read buf
    
    mov ax, @ORD                        ; open src file
    call open
    cmp ax, -1
    je cpf0
    mov [bp+cpfh0], ax
    
cpf7:
    mov ax, @OWR                        ; open dest file
    mov dx, bx
    call open
    cmp ax, -1                          ; not exists, create
    je cpf5
    mov [bp+cpfh1], ax
    jmp cpf6
    
cpf5:
    call mkfile                         ; create dest file
    cmp ax, @TRUE
    jne cpf2
    jmp cpf7
    
cpf6:
    mov ax, ss
    mov ds, ax
cpf4:
    mov dx, sp                          ; read 100h bytes
    mov ax, [bp+cpfh0]
    mov cx, 100h
    call read
    push cx
    mov ax, [bp+cpfh1]                  ; write to dest file
    call write
    pop cx
    cmp cx, 100h                        ; meets end of file
    jl cpf3
    jmp cpf4
    
cpf3:
    mov ax, [bp+cpfh0]                  ; close src file
    call close
    mov ax, [bp+cpfh1]                  ; close dest file, save written data
    call close
    mov ax, @TRUE                       ; copied
    jmp cpf1
cpf0:                                   ; failed to open src
    mov ax, @FALSE
    jmp cpf1
cpf2:                                   ; failed to open dest
    mov ax, [bp+cpfh0]                  ; close src
    call close
    
cpf1:
    mov sp, bp
    pop bp
    pop ds
    pop dx
    pop cx
    ret
cpfile endp

cpdir proc near
    ; to be implemented
    ret
cpdir endp