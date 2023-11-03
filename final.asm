.MODEL LARGE
.386
.STACK 200h
.DATA

z dd 
d dd 
c dd 
b dd 
a dd 
_a dd a
_4 dd 4
_3 dd 3
_2 dd 2

FLD _2
FLD _3
FADD
FSTP @aux1
FLD @aux1
FLD _4
FADD
FSTP @aux2
FLD @aux2
FRNDINT
FSTP a
fld a
fcomp _3
fstsw ax
sahf
JNB falso0
dato: "a"
falso0
