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
@aux1 dd ??
@aux2 dd ??
.CODE
START:
FLD _1
FLD _2
FADD
FSTP @aux1
FLD @aux1
FLD _3
FADD
FSTP @aux2
FLD @aux2
FRNDINT
FSTP c

MOV EAX, 4C00H
INT 21h
END START