#include "arbol.h"
int esComparadorArbol(char* op);

void crearArbol(Arbol *pa)
{
    *pa = NULL;
}

NodoA *crearNodo(char *simb, NodoA *hIzq, NodoA *hDer)
{

    NodoA *nuevo = (NodoA *)malloc(sizeof(NodoA)); // crearNodoA(el, tam);
    if (!nuevo)
        exit(EXIT_FAILURE);

    nuevo->der = hDer;
    nuevo->izq = hIzq;
    strcpy(nuevo->simbolo, simb);

    return nuevo;
}

NodoA *crearHoja(char *simb)
{
    return crearNodo(simb, NULL, NULL);
}

void imprimirArbol(Arbol *pa)
{
    FILE *arch = fopen("intermediate-code.txt", "w");
    if (!arch)
    {
        printf("No se pudo abrir el archivo para escritura\n");
        return;
    }
    recorrerArbolInOrden(pa, 0, arch);
    fclose(arch);
}

void recorrerArbolInOrden(Arbol *pa, int nivel, FILE *arch)
{
    if (!*pa)
        return;
    recorrerArbolInOrden(&(*pa)->izq, nivel + 1, arch);
   
    for (int i = 0; i < nivel; i++)
        fprintf(arch, "\t");
    fprintf(arch, "%s\n", &(*pa)->simbolo);

    recorrerArbolInOrden(&(*pa)->der, nivel + 1, arch);
}
void recorrerArbolInOrdenOUT(Arbol *pa)
{
    if (!*pa)
        return;
    recorrerArbolInOrdenOUT(&(*pa)->izq);
   
   
    printf("%s\n", &(*pa)->simbolo);

    recorrerArbolInOrdenOUT(&(*pa)->der);
}

void vaciarArbol(Arbol *pa)
{
    if (!*pa)
        return;

    vaciarArbol(&(*pa)->izq);
    vaciarArbol(&(*pa)->der);
    free(*pa);
    *pa = NULL;
}


NodoA *padreMasIzq(Arbol *pa) {
    if (!*pa)
        return NULL;

    NodoA *res = padreMasIzq(&(*pa)->izq);
    if (res)
        return res;
    
    if ((*pa)->izq  && (*pa)->der && !esComparadorArbol((*pa)->simbolo)){
        return *pa;
    }
    res = padreMasIzq(&(*pa)->der);
    if (res)
        return res;

    return NULL;
}

int esHoja(NodoA* nodo)
{
    return !nodo->izq && !nodo->der;
}

void borrarHijos(NodoA* pa){
    if(!pa){
        return;
    }
    if (pa->izq)
	{
		free(pa->izq);
		pa->izq = NULL;
	}
	if (pa->der)
	{
		free(pa->der);
		pa->der = NULL;
	}
}

int esComparadorArbol(char* op){
    if(strcmp(op, "<") == 0 ||
        strcmp(op, "<=") == 0 ||
        strcmp(op, ">") == 0 ||
        strcmp(op, ">=") == 0 ||
        strcmp(op, "==") == 0 ||
        strcmp(op, "!=") == 0 ||
        strcmp(op, "&") == 0 ||
        strcmp(op, "||") == 0 )
        return 1;

    return 0;
}