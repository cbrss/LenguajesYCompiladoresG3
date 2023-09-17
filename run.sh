#!/bin/bash

## Script para Unix
flex lexico.l
bison -dyv sintactico.y
gcc lex.yy.c y.tab.c tab_simb.c -o lyc-compiler-1_0_0.exe
./compilador "casos_de_prueba/test.txt"
rm lex.yy.c
rm y.tab.c
rm y.output
rm y.tab.h
rm compilador
