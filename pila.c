#include "include/pila.h"

 
Pila crearPila(Pila* pp)
{
    *pp = NULL;
    return *pp;
}
 
int apilar(Pila* pp, const void* elem, size_t tam_el)
{
    NodoP* nuevo = malloc(sizeof(NodoP));
    void* p_el = malloc(tam_el);
    if(!nuevo || !p_el)
    {
        free(nuevo);
        free(p_el);
        return FALSE;
    }
 
    // "Engancho" el nodo a la pila
    nuevo->pdato = p_el;
    nuevo->sig = *pp;
    *pp = nuevo;
    // Copio datos al nodo
    memcpy(nuevo->pdato, elem, tam_el);
    nuevo->tamElem = tam_el;
 
    return TRUE;
}
 
int desapilar(Pila* pp, void* p_elem, size_t tam_el)
{
    if(!*pp){
        return FALSE;
    }
    NodoP* aux = *pp;    // NodoP a desapilar
    // "Desengancho" el nodo
    *pp = aux->sig;
    // Copio datos del nodo
    
    memcpy(p_elem, aux->pdato, MIN(tam_el, aux->tamElem));
    // Libero memoria
    free(aux->pdato);
    free(aux);
 
    return TRUE;
}
 
int verTope(Pila* pp, void* p_elem, size_t tam_el)
{
    if(!*pp)
        return FALSE;
    memcpy(p_elem, (*pp)->pdato, MIN(tam_el, (*pp)->tamElem));
    return TRUE;
}
 
int pilaLLena(Pila* pp, size_t tam_el)
{
    NodoP* ntemp = malloc(sizeof(NodoP));
    void* dtemp = malloc(tam_el);
    free(ntemp);
    free(dtemp);
    return !ntemp || !dtemp;
}
 
int pilaVacia(Pila* pp)
{
    return *pp == NULL;
}
 
void vaciarPila(Pila* pp)
{
    while(*pp)
    {
        NodoP* aux = *pp;    // NodoP a desapilar
        // "Desengancho" el nodo
        *pp = aux->sig;
        // Libero memoria
        free(aux->pdato);
        free(aux);
    }
}