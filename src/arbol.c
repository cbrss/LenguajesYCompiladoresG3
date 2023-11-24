#include "../include/arbol.h"
int esComparadorArbol(char* op);
void limpiarComillas(char* dest, char* ori);

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
    nuevo->nro = contadorArbolInd;

    contadorArbolInd++;
    return nuevo;
}

NodoA *crearHoja(char *simb)
{
    return crearNodo(simb, NULL, NULL);
}

void imprimirArbolGrafico(Arbol *pa)
{
    FILE *arch = fopen("intermediate-code.dot", "w");
    if (!arch)
    {
        printf("No se pudo abrir el archivo para escritura\n");
        return;
    }
    fprintf(arch, "digraph G{\n");

    recorrerArbolInOrdenGrafico(pa, arch);
    fprintf(arch, "}\n");
    fclose(arch);
}

void recorrerArbolInOrdenGrafico(Arbol *pa, FILE *arch)
{
    static int contador = 0;
    
    if (!*pa )
        return;
    char strCopy[100];
    
    if((*pa)->simbolo[0] == '"'){
        limpiarComillas(strCopy, (*pa)->simbolo);

        fprintf(arch, "\"nodo%d\"[ label=\"\\\"%s\\\"\"];\n", (*pa)->nro, strCopy);    //si es un string literal tiene comilllas
    } else{
        fprintf(arch, "\"nodo%d\"[ label=\"%s\"];\n", (*pa)->nro, (*pa)->simbolo);
    }
        
    if ((*pa)->izq != NULL ) {
    
        if((*pa)->izq->simbolo[0] == '"'){
            
            limpiarComillas(strCopy, (*pa)->der->simbolo);
            fprintf(arch, "\"nodo%d\"[ label=\"\\\"%s\\\"\"];\n", (*pa)->izq->nro, strCopy);    //si es un string literal tiene comilllas
        }
        else {
            fprintf(arch, "\"nodo%d\"[ label=\"%s\"];\n", (*pa)->izq->nro, (*pa)->izq->simbolo);
        }
        fprintf(arch, "nodo%d -> nodo%d\n", (*pa)->nro, (*pa)->izq->nro);   //creo una arista entre los nodos padre e hijo

        recorrerArbolInOrdenGrafico(&(*pa)->izq, arch);
    }
    if ((*pa)->der != NULL ) {

        if((*pa)->der->simbolo[0] == '"'){
            limpiarComillas(strCopy, (*pa)->der->simbolo);
            fprintf(arch, "\"nodo%d\"[ label=\"\\\"%s\\\"\"];\n", (*pa)->der->nro, strCopy); //si es un string literal tiene comilllas
        }
        else {
            fprintf(arch, "\"nodo%d\"[ label=\"%s\"];\n", (*pa)->der->nro, (*pa)->der->simbolo); 
        }
        fprintf(arch, "nodo%d -> nodo%d\n", (*pa)->nro, (*pa)->der->nro);
    
        recorrerArbolInOrdenGrafico(&(*pa)->der, arch);
    }
}

void limpiarComillas(char* dest, char* ori){
    int len = strlen(ori);
    char strCopy[len];
    strncpy(strCopy, ori + 1, len - 2);
    strCopy[len - 2] = '\0';
    strcpy(dest, strCopy);

}

void imprimirArbolTexto(Arbol *pa)
{
    FILE *arch = fopen("intermediate-code.txt", "w");
    if (!arch)
    {
        printf("No se pudo abrir el archivo para escritura\n");
        return;
    }
    recorrerArbolInOrdenTexto(pa, 0, arch);
    fclose(arch);
}

void recorrerArbolInOrdenTexto(Arbol *pa, int nivel, FILE *arch)
{
    if (!*pa)
        return;
    recorrerArbolInOrdenTexto(&(*pa)->izq, nivel + 1, arch);
   
    for (int i = 0; i < nivel; i++)
        fprintf(arch, "\t");
    fprintf(arch, "%s\n", (char*)&(*pa)->simbolo);

    recorrerArbolInOrdenTexto(&(*pa)->der, nivel + 1, arch);
}

void recorrerArbolInOrdenOUT(Arbol *pa)
{
    if (!*pa)
        return;
    recorrerArbolInOrdenOUT(&(*pa)->izq);
   
   
    printf("%s\n", (char*)&(*pa)->simbolo);

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
        strcmp(op, "||") == 0 || 
        strcmp(op, "cant") == 0)
        return 1;

    return 0;
}