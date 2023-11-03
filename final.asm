.MODEL LARGE
.386
.STACK 200h
.DATA

z dd ??
pepe dd ??
d dd ??
c dd ??
b dd ??
a dd ??
_2_+_42 db "2 + 42" , '$', 6 dup (?)
_2 dd 2.0
@aux1 dd 

FLD "2 + 42"
FSTP pepe
FLD _2
FLD _2
FADD
FSTP @aux1
FLD @aux1
FRNDINT
FSTP a
