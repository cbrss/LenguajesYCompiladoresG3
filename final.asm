.MODEL LARGE
.386
.STACK 200h
.DATA

pepe dd ??
h dd ??
g dd ??
f dd ??
e dd ??
d dd ??
c dd ??
b dd ??
a dd ??
_true_interno db "true interno" , '$', 12 dup (?)
_true_externo db "true externo" , '$', 12 dup (?)
_fin_true_externo db "fin true externo" , '$', 16 dup (?)
_false_interno db "false interno" , '$', 13 dup (?)
_false_externo db "false externo" , '$', 13 dup (?)

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
dato: "true externo"
fld e
fcomp f
fstsw ax
sahf
JBE falso1
fld g
fcomp h
fstsw ax
sahf
JNB etiquetaOr1
falso1
dato: "true interno"
BI verdadero0
etiquetaOr1
dato: "false interno"
verdadero0
dato: "fin true externo"
BI verdadero1
etiquetaOr0
dato: "false externo"
verdadero1
