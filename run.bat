flex lexico.l
bison -dyv sintactico.y

gcc.exe lex.yy.c y.tab.c tab_simb.c -o lyc-compiler-1_0_0.exe

compilador.exe casos_de_prueba/test.txt

@echo off
del lyc-compiler-1_0_0.exe
del lex.yy.c
del y.tab.c
del y.tab.h
del y.output

pause