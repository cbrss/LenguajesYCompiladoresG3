#include "tab_simb.h"

Lista crearLista(Lista *pl)
{
    *pl = NULL;
    return *pl;
}

void insertarEnLista(Lista *lista, char *nombre, enum tiposDato tDato)
{
    Simbolo nuevo_simbolo;

    nuevo_simbolo.valor[0] = '\0';
    nuevo_simbolo.tipo_dato[0] = '\0';
    nuevo_simbolo.longitud = 0;

    if (tDato == tID)
    {
        strcpy(nuevo_simbolo.nombre, nombre);
    }
    else if (tDato == tINT)
    {
        strcpy(nuevo_simbolo.nombre, "_");
        strcat(nuevo_simbolo.nombre, nombre);
        strcpy(nuevo_simbolo.tipo_dato, TINT);
        strcpy(nuevo_simbolo.valor, nombre);
        nuevo_simbolo.longitud = strlen(nombre);
    }
    else if (tDato == tFLOAT)
    {
        strcpy(nuevo_simbolo.nombre, "_");
        strcat(nuevo_simbolo.nombre, nombre);
        strcpy(nuevo_simbolo.tipo_dato, TFLOAT);
        strcpy(nuevo_simbolo.valor, nombre);
        nuevo_simbolo.longitud = strlen(nombre);
    }
    else if (tDato == tSTRING)
    {
        char aux[] = "\"\"";
        if(strcmp(nombre, aux) == 0){    //no guarda string vacios
            return;
        }
        int longitud = strlen(nombre) - 2; // -1 para sacar\0 -1 para "
        char nNombre[longitud - 2];
        strncpy(nNombre, nombre + 1, longitud); // saco los "" del string
        nNombre[longitud] = '\0';               // strncpy no mete \0
        strcpy(nuevo_simbolo.nombre, "_");
        strcat(nuevo_simbolo.nombre, nNombre);
        strcpy(nuevo_simbolo.tipo_dato, TSTRING);
        strcpy(nuevo_simbolo.valor, nNombre);
        nuevo_simbolo.longitud = longitud;
    }

    while ((*lista != NULL) && strcmp((*lista)->simb.nombre, nuevo_simbolo.nombre) > 0 )
    {
        lista = &(*lista)->sig;
    }

    // a int
    // a
    // a str

   
    if (*lista != NULL && tDato == tID && strcmp((*lista)->simb.nombre, nuevo_simbolo.nombre) == 0)
    {
        return;
    }
    else if (*lista != NULL && strcmp((*lista)->simb.nombre, nuevo_simbolo.nombre) == 0 &&  strcmp((*lista)->simb.tipo_dato, nuevo_simbolo.tipo_dato) == 0)
    {
        return;
    }

    NodoL *nuevo = (NodoL *)malloc(sizeof(NodoL));
    memcpy(&(nuevo->simb), &nuevo_simbolo, sizeof(Simbolo));
    nuevo->sig = *lista;
    *lista = nuevo;
}

void imprimirLista(Lista *lista)
{
    FILE *arch = fopen("symbol-table.txt", "w");
    if (arch == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    fprintf(arch, "%-50s|%-7s|%-50s|%-10s\n", "nombre", "tipoDato", "valor", "longitud");
    while (*lista != NULL)
    {
        fprintf(arch, "%-50s|%-7s|%-50s|%-10d\n", (*lista)->simb.nombre, (*lista)->simb.tipo_dato, (*lista)->simb.valor, (*lista)->simb.longitud);
        lista = &(*lista)->sig;
    }

    fclose(arch);
}

int idDeclarado(Lista *lista, char *id)
{

    while ((*lista != NULL) && strcmp((*lista)->simb.nombre, id) > 0)
    {
        lista = &(*lista)->sig;
    }
    if (*lista != NULL && strcmp((*lista)->simb.nombre, id) == 0 && !strlen((*lista)->simb.tipo_dato))
    {
        return FALSE;
    }

    return TRUE;
}

void asignarTipoDato(Lista *lista, char *id, char *tipoDato)
{

    while ((*lista != NULL) && strcmp((*lista)->simb.nombre, id) > 0)
    {
        lista = &(*lista)->sig;
    }
    if (*lista != NULL && strcmp((*lista)->simb.nombre, id) == 0)
    {
        strcpy((*lista)->simb.tipo_dato, tipoDato);
    }
}

void vaciarLista(Lista *pl)
{
    NodoL *aux;

    while (*pl)
    {
        aux = *pl;
        *pl = (aux)->sig;
        free(aux);
    }
}

void asignarTipo(Lista *lista, char *auxTipo)
{
    while ((*lista != NULL))
    {
        strcpy((*lista)->simb.tipo_dato, auxTipo);
        lista = &(*lista)->sig;
    }
}
void fusionarLista(Lista *lista1, Lista *lista2)
{
    while ((*lista2 != NULL && *lista1 != NULL))
    {
        if (strcmp((*lista1)->simb.nombre, (*lista2)->simb.nombre) == 0)
        {
            strcpy((*lista1)->simb.tipo_dato, (*lista2)->simb.tipo_dato);
            lista2 = &(*lista2)->sig;
        }
        lista1 = &(*lista1)->sig;
    }
}

int esMismoTipo(Lista *lista, char *id, char *auxTipo)
{
    while ((*lista != NULL) && strcmp((*lista)->simb.nombre, id) > 0)
    {
        lista = &(*lista)->sig;
    }
    if (*lista != NULL && strcmp((*lista)->simb.nombre, id) == 0)
    {
        if (strcmp((*lista)->simb.tipo_dato, auxTipo) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

char *obtenerTipo(Lista *lista, char *id)
{
    while ((*lista != NULL) && strcmp((*lista)->simb.nombre, id) > 0)
    {
        lista = &(*lista)->sig;
    }
    if (*lista != NULL && strcmp((*lista)->simb.nombre, id) == 0)
    {
        return (*lista)->simb.tipo_dato;
    }
    return NULL;
}