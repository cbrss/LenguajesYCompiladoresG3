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
_true db "true" , '$', 4 dup (?)
_false db "false" , '$', 5 dup (?)
_Asdasdasdas db "Asdasdasdas" , '$', 11 dup (?)

fld a
fcomp b
fstsw ax
sahf
JBE falso0
fld c
fcomp d
fstsw ax
sahf
JNB etiquetaOr0
falso0
fld a
fcomp b
fstsw ax
sahf
JBE falso0
fld c
fcomp d
fstsw ax
sahf
JNB etiquetaOr1
falso0
dato: "true"
BI verdadero2
etiquetaOr1
dato: "false"
verdadero1
BI verdadero1
etiquetaOr0
dato: "false"
verdadero1
dato: "Asdasdasdas"
