FLD b
FLD c
FMUL
FSTP @aux1
FLD a
FLD @aux1
FADD
FSTP @aux2
FLD d
FLD _20
FDIV
FSTP @aux3
FLD @aux2
FLD @aux3
FSUB
FSTP @aux4
FLD @aux4
FRNDINT
FSTP z
