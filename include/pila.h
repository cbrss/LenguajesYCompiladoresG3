#ifndef _PILAH_
#define _PILAH_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constantes.h"

typedef struct _NodoP{
    void* pdato;
    size_t tamElem;
    struct _NodoP* sig;
} NodoP;


typedef NodoP* Pila;

 
Pila crearPila(Pila* pp);

int apilar(Pila* pp, const void* elem, size_t tam_el);
 
int desapilar(Pila* pp, void* p_elem, size_t tam_el);
 
int verTope(Pila* pp, void* p_elem, size_t tam_el);
 
int pilaLLena(Pila* pp, size_t tam_el);
 
int pilaVacia(Pila* pp);
 
void vaciarPila(Pila* pp);

#endif