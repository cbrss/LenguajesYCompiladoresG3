.MODEL LARGE
.386
.STACK 200h
.DATA

z dd 
d dd 
c dd 
b dd 
a dd 
_2 dd 2

fld a
fcomp b
fstsw ax
sahf
JBE falso0
fld a
fcomp b
fstsw ax
sahf
JNB falso0
FLD _2
FRNDINT
FSTP a
falso0
