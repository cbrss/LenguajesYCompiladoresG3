#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constantes.h"

enum tiposDato
{
    tID,
    tINT,
    tFLOAT,
    tSTRING
};

typedef struct
{
    char nombre[ID_LARGO_MAX + 2];
    char tipo_dato[TIPO_DATO_LARGO_MAX + 1];
    char valor[VALOR_LARGO_MAX + 1];
    int longitud;
} Simbolo;

typedef struct _NodoL
{
    Simbolo simb;
    struct _NodoL *sig;
} NodoL;

typedef NodoL *Lista;

Lista crearLista(Lista *pl);
void insertarEnLista(Lista *lista, char *nombre, enum tiposDato tDato);
void imprimirLista(Lista *lista);

int idDeclarado(Lista *lista, char *var1);
void asignarTipoDato(Lista *lista, char *id, char *tipoDato);
void vaciarLista(Lista *pl);
void asignarTipo(Lista *listaIds, char *auxTipo);
void fusionarLista(Lista *lista1, Lista *lista2);
int esMismoTipo(Lista *, char *, char *);
char *obtenerTipo(Lista *, char *);
void obtenerSinComillas(char* strDest, char* strOri);
int listaVacia(Lista *);
int eliminarDeLista(Lista* lista, char* id);
void imprimirEncabezado(Lista* lista, int cantAux);
char* obtenerNombre(Lista *lista, char* valor, char* tDato);