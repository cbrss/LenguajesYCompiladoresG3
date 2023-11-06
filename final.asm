include number.asm
include macros.asm
.MODEL LARGE
.386
.STACK 200h
.DATA
c dd ?
b dd ?
a dd ?
_2 dd 2.00
_1 dd 1.00
_0 dd 0.00
_-1 dd -1.00
@auxExp dd ?
@_contI dd ?
.CODE
START:
FLD 0
FRNDINT
FSTP @_contI
FLD _2
FSTP @auxExp
fld @auxExp
fcomp a
fstsw ax
sahf
JE falso0
FLD @_contI
FLD 1
FADD
FSTP @_contI
falso0:
fld @_contI
fcomp 0
fstsw ax
sahf
JNE falso1
FLD -1
FRNDINT
FSTP c
BI verdadero0
falso1:
FLD @_contI
FRNDINT
FSTP c
verdadero0:

MOV EAX, 4C00H
INT 21h
END START