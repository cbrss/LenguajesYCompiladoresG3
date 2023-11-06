.MODEL LARGE
.386
.STACK 200h
.DATA

b dd ??
a dd ??
_3 dd 3.0
_2 dd 2.0
_1 dd 1.0
@aux1 dd ??
@aux2 dd ??

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
FSTP a
