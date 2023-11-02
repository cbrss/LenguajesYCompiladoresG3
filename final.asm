.MODEL LARGE
.386
.STACK 200h
.DATA

z dd 
d dd 
c dd 
b dd 
a dd 
_b dd b
_a dd a

fld a
fcomp b
fstsw ax
sahf
JNB falso0
dato: "a"
fld b
fcomp d
fstsw ax
sahf
JNB falso1
dato: "b"
falso1
falso0
