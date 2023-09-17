#include "tab_simb.h"

Lista crearLista()
{
    return NULL;
}

void insertarEnLista(Lista *lista, char *nombre, enum tiposDato tDato)
{
    Simbolo nuevo_simbolo;
    
    
    nuevo_simbolo.valor[0] = '\0';
    nuevo_simbolo.tipo_dato[0] = '\0';
    nuevo_simbolo.longitud = 0;
    
    if ( tDato == tID ) {
        strcpy(nuevo_simbolo.nombre, nombre);
    }
    else if (tDato == tINT ) {
        strcpy(nuevo_simbolo.nombre, "_");
        strcat(nuevo_simbolo.nombre, nombre);
        strcpy(nuevo_simbolo.tipo_dato, "Int");
        strcpy(nuevo_simbolo.valor, nombre);
    }
    else if ( tDato == tFLOAT ) {
        strcpy(nuevo_simbolo.nombre, "_");
        strcat(nuevo_simbolo.nombre, nombre);
        strcpy(nuevo_simbolo.tipo_dato, "Float");
        strcpy(nuevo_simbolo.valor, nombre);
    }
    else if ( tDato == tSTRING ) {
        int longitud = strlen(nombre) - 2; // -1 para sacar\0 -1 para "
        char nNombre[longitud - 2];
        strncpy(nNombre, nombre + 1, longitud ); //saco los "" del string
        nNombre[longitud] = '\0';               //strncpy no mete \0
        strcpy(nuevo_simbolo.nombre, "_");
        strcat(nuevo_simbolo.nombre, nNombre);
        strcpy(nuevo_simbolo.tipo_dato, "String");
        strcpy(nuevo_simbolo.valor, nNombre);
        nuevo_simbolo.longitud = longitud;
    }

    while ((*lista != NULL) && strcmp((*lista)->simb.nombre, nombre))
    {
        lista = &(*lista)->sig;
    }
    if (*lista == NULL)
    {
        Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
        memcpy(&(nuevo->simb), &nuevo_simbolo, sizeof(Simbolo));
        nuevo->sig = NULL;
        *lista = nuevo;
    }
}

void imprimirLista(Lista *lista)
{
    FILE* arch = fopen("symbol-table.txt", "w");
    if ( arch == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    while (*lista != NULL)
    {
        fprintf(arch, "%-50s|%-7s|%-50s|%-10d\n", (*lista)->simb.nombre, (*lista)->simb.tipo_dato, (*lista)->simb.valor, (*lista)->simb.longitud);
        lista = &(*lista)->sig;
        
    }
}

