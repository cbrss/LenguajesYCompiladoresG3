#ifndef _ARBOLH_
#define _ARBOLH_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _NodoA
{
    char simbolo[50];
    struct _NodoA *der;
    struct _NodoA *izq;
    int nro;
    int visitado;
} NodoA;

typedef NodoA *Arbol;

void crearArbol(Arbol *pa);

NodoA *crearNodo(char *simb, NodoA *hIzq, NodoA *hDer);
NodoA *crearHoja(char *simb);
void imprimirArbolGrafico(Arbol *pa);
void recorrerArbolInOrdenGrafico(Arbol *pa, FILE *fp);
void imprimirArbolTexto(Arbol *pa);
void recorrerArbolInOrdenTexto(Arbol *pa, int nivel, FILE *arch);
void recorrerArbolInOrdenOUT(Arbol *pa);
void vaciarArbol(Arbol *pa);
NodoA *padreMasIzq(Arbol *pa);
int esHoja(NodoA* pa);
void borrarHijos(NodoA* pa);

int static contadorArbolInd = 0;

#endif