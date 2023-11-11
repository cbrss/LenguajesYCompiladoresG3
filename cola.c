#include <stdlib.h>
#include <memory.h>

#include "include/cola.h"

#define min(a, b) ((a)<(b)? (a) : (b))

void crearCola(Cola* pc)
{
    pc->frente = pc->fondo = NULL;
}

int encolar(Cola* pc, const void* dato, size_t tamElem)
{
    NodoC* nue = (NodoC*)malloc(sizeof(NodoC));
    void* datoN = malloc(tamElem);

    if(!nue || !datoN)
    {
        free(nue);
        free(datoN);
        return 0;/// falso
    }

    memcpy(datoN, dato, tamElem);

    nue->dato = datoN;
    nue->tamElem = tamElem;
    nue->sig = NULL;

    if(pc->fondo)
        pc->fondo->sig = nue;
    else
        pc->frente = nue;

    pc->fondo = nue;

    return 1;
}

int desencolar(Cola* pc, void* dato, size_t tamElem)
{
    if(!pc->frente)
        return 0;
    NodoC* nae = pc->frente;
    pc->frente = nae->sig;

    if(!pc->frente)
        pc->fondo = NULL;

    memcpy(dato, nae->dato, min(tamElem, nae->tamElem));

    free(nae->dato);
    free(nae);

    return 1;
}

int verFrenteDeCola(const Cola* pc, void* dato, size_t tamElem)
{
    if(!pc->frente)
        return 0;


    NodoC* frente = pc->frente;
    memcpy(dato, frente->dato, min(tamElem, frente->tamElem));

    return 1;
}

int colaVacia(const Cola* pc)
{
    return !pc->frente;
}

int colaLlena(const Cola* pc, size_t tamElem)
{
    void* NodoC = malloc(sizeof(NodoC));
    void* dato = malloc(tamElem);

    free(NodoC);
    free(dato);

    return !NodoC || !dato;
}

void vaciarCola(Cola* pc)
{
    NodoC* nae;

    while(pc->frente)
    {
        nae= pc->frente;
        pc->frente = nae->sig;

        free(nae->dato);
        free(nae);
    }

    pc->fondo = NULL;
}
