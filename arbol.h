#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _NodoA
{
    char simbolo[50];
    struct _NodoA *der;
    struct _NodoA *izq;
} NodoA;

typedef NodoA *Arbol;

void crearArbol(Arbol *pa);

NodoA *crearNodo(char *simb, NodoA *hIzq, NodoA *hDer);
NodoA *crearHoja(char *simb);
void imprimirArbol(Arbol *pa);
void recorrerArbolInOrden(Arbol *pa, int nivel, FILE *fp);
void vaciarArbol(Arbol *pa);
