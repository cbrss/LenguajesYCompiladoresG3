flex lexico.l
bison -dyv sintactico.y

gcc.exe lex.yy.c y.tab.c ./src/tab_simb.c ./src/arbol.c ./src/pila.c ./src/cola.c ./src/assembler.c -o lyc-compiler-2.0.0.jar
lyc-compiler-2.0.0.jar casos_de_prueba/testSimple.txt
dot -Tpng intermediate-code.dot -o arbol.png

@echo off
del lyc-compiler-2.0.0.jar
del lex.yy.c
del y.tab.c
del y.tab.h
del y.output