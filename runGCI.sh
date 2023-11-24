#!/bin/bash
set -e
## Script para Unix
flex Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c ./src/tab_simb.c ./src/arbol.c ./src/pila.c ./src/cola.c ./src/assembler.c -o lyc-compiler-3.0.0
./lyc-compiler-3.0.0 "casos_de_prueba/test.txt"
dot -Tpng intermediate-code.dot -o arbol.png
rm lex.yy.c
rm y.tab.c
rm y.output
rm y.tab.h
rm lyc-compiler-3.0.0
