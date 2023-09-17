#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constantes.h"

enum tiposDato {
    tID,
    tINT,
    tFLOAT,
    tSTRING
};

typedef struct
{
    char nombre[ID_LARGO_MAX+2];
    char tipo_dato[TIPO_DATO_LARGO_MAX];
    char valor[VALOR_LARGO_MAX];
    int longitud;
} Simbolo;

typedef struct _Nodo
{
    Simbolo simb;
    struct _Nodo *sig;
} Nodo;

typedef Nodo *Lista;

Lista crearLista();
void insertarEnLista(Lista *lista, char *nombre, enum tiposDato tDato);
void imprimirLista(Lista *lista);
Lista lista;