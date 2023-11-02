.MODEL LARGE
.386
.STACK 200h
.DATA

z dd 
d dd 
c dd 
b dd 
a dd 
_3 dd 3
_2 dd 2

fld a
fcomp b
fstsw ax
sahf
JNB falso0
fld d
fcomp c
fstsw ax
sahf
JNB falso1
FLD a
FRNDINT
FSTP _2
BI verdadero0
falso1
FLD a
FRNDINT
FSTP _3
verdadero0
falso0
