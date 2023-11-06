include number.asm
include macros.asm
.MODEL LARGE
.386
.STACK 200h
.DATA
c dd ?
b dd ?
a dd ?
_3 dd 3.00
_2 dd 2.00
_1 dd 1.00
.CODE
START:

MOV EAX, 4C00H
INT 21h
END START