#ifndef _ASSEMBLERH_
#define _ASSEMBLERH_

#include "../include/tab_simb.h"
#include "../include/arbol.h"


void imprimirEncabezado(Lista* listaSimbolos, int cantidadAuxiliares);
void generarCodigo(FILE* arch, Lista* listaSimbolos, Arbol* cod_intermedio);
void imprimirFin(FILE* arch);
void generarAssembler(Lista* listaSimbolos, Arbol* cod_intermedio, int cantAuxiliares);
void invertirCondicion(NodoA* padre);

int esComparador(char* op);

int esOperacionAritmetica(char* op);
int esOperadorLogico(char* op);

#endif