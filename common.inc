; common.inc
; written by hyan23
; 2017.11.26

AMACRO macro x, y
    mov ax, x
    mov dx, y
endm

; 定义规范: in: , ret: 
; 如果参数是一个地址, 只读写上索引寄存器即可, 可写标明段寄存器
; 建议: dx for readonly access, bx for writeable access
; 建议: dx: src, bx: dest
;PROC0 proc near
;    ; to be implemented
;    ret
;PROC0 endp

; 实现规范: 在一个模块(测试程序除外)调用另一个模块的例程, 必须标注该例程属于哪个模块

; 32位寄存器组: dx:ax
; 64位寄存器组: dx:ax:cx:bx

@WORD       equ             2

@TRUE       equ             1
@FALSE      equ             0

; file.inc.open
@ORD        equ             0
@OWR        equ             1
@ORDWR      equ             2

; file.inc.seek
@SEEK_SET   equ             0
@SEEK_CUR   equ             1
@SEEK_END   equ             2