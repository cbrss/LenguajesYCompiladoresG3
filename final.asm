.MODEL LARGE
.386
.STACK 200h
INCLUDE number.asm
.DATA
c dd ??
b dd ??
a dd ??
_1 dd 1.0
_1 db "1" , '$', 1 dup (?)

FLD "1"
FSTP a
FLD _1
FRNDINT
FSTP c
