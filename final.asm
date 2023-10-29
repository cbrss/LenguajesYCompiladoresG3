FLD b
FLD _2
FADD
FSTP @aux1
FLD @aux1
FLD _2
FADD
FSTP @aux2
fld a
fcomp @aux2
fstsw ax
sahf
FLD _2
FRNDINT
FSTP a
