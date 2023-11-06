include number.asm
include macros.asm
.MODEL LARGE
.386
.STACK 200h
.DATA
a dd ?
_s_2 db "2" , '$', 1 dup (?)
.CODE
START:
displayString _s_2
newLine 1

MOV EAX, 4C00H
INT 21h
END START