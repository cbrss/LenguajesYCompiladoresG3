#include "../include/tab_simb.h"
void sacarEspacios(char *str);
void sacarPuntos(char* str);

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
        strcpy(nuevo_simbolo.tipo_dato, "Id");
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
        char strAux[STRING_LARGO_MAX];

        strcpy(nuevo_simbolo.nombre, "_");
        snprintf(strAux, STRING_LARGO_MAX, "%.2f", nombre);
        
        strcat(nuevo_simbolo.nombre, strAux);
        strcpy(nuevo_simbolo.tipo_dato, TFLOAT);
        strcpy(nuevo_simbolo.valor, nombre);
        nuevo_simbolo.longitud = strlen(nombre);
        
        sacarPuntos(nuevo_simbolo.nombre);
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
        strcpy(nuevo_simbolo.nombre, "_s_");
        strcat(nuevo_simbolo.nombre, nNombre);
        strcpy(nuevo_simbolo.tipo_dato, TSTRING);
        strcpy(nuevo_simbolo.valor, nNombre);
        nuevo_simbolo.longitud = longitud;

        sacarEspacios(nuevo_simbolo.nombre);
    }

    while ((*lista != NULL) && strcmp((*lista)->simb.nombre, nuevo_simbolo.nombre) > 0 )
    {
        lista = &(*lista)->sig;
    }
   
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
void sacarEspacios(char *str) {
    for(int i = 0; str[i]; i++){
        if(str[i] == ' ')
            str[i] = '_';
    }
}
void sacarPuntos(char *str) {
    for(int i = 0; str[i]; i++){
        if(str[i] == '.')
            str[i] = '_';
    }
}

char* obtenerNombre(Lista *lista, char* valor, char* tDato) {
    while ((*lista != NULL) && (strcmp((*lista)->simb.valor, valor) != 0 || strcmp((*lista)->simb.tipo_dato, tDato) != 0))
    {
        lista = &(*lista)->sig;
    }
    if (*lista != NULL && strcmp((*lista)->simb.valor, valor) == 0 && strcmp((*lista)->simb.tipo_dato, tDato) == 0)
    {
        return (*lista)->simb.nombre;
    }
    return NULL;
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
    if (*lista != NULL && strcmp((*lista)->simb.nombre, id) == 0 && strcmp((*lista)->simb.tipo_dato, "Id") == 0)
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

void obtenerSinComillas(char* strDest, char* strOri) 
{
    int len = strlen(strOri);
    char strCopy[len];
    strncpy(strCopy, strOri + 1, len - 2);
    strCopy[len - 2] = '\0';
    strcpy(strDest, strCopy);

}

int listaVacia(Lista* lista)
{
    return !*lista;
}

int eliminarDeLista(Lista* lista, char* id)
{
    if(!*lista)
        return 0;
    NodoL* nae = *lista;
    *lista = nae->sig;
     
    strcpy(id, nae->simb.nombre);
   
    free(nae);

    return 1;
}

void imprimirEncabezado(Lista* lista, int cantAux){
    FILE *arch = fopen("final.asm", "w");
    if (arch == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    //number.asm
    
    fprintf(arch, "include ./asm/number.asm\n");
    fprintf(arch, "include ./asm/macros.asm\n");
    fprintf(arch, ".MODEL LARGE\n.386\n.STACK 200h\n.DATA\n");
    while (*lista != NULL)
    {
        if (strlen((*lista)->simb.valor) == 0){     // si es ID
            fprintf(arch, "%s dd ?\n", (*lista)->simb.nombre);

        } else if(strcmp((*lista)->simb.tipo_dato, "Int") == 0 ){
            fprintf(arch, "%s dd %s.00\n", (*lista)->simb.nombre, (*lista)->simb.valor);
        } else if (strcmp((*lista)->simb.tipo_dato, "Float") == 0){
            fprintf(arch, "%s dd %s\n", (*lista)->simb.nombre, (*lista)->simb.valor);   //si es float
        } else{
            fprintf(arch, "%s db \"%s\" , '$', %d dup (?)\n", (*lista)->simb.nombre, (*lista)->simb.valor, (*lista)->simb.longitud);
        }
        //TODO: faltaria si es un string

        lista = &(*lista)->sig;
    }
    int i = 1;
    if(cantAux != -1){
        while(cantAux != 0){
            char auxiliar[ID_LARGO_MAX];
            char numero[ID_LARGO_MAX];

            strcpy(auxiliar, "@aux");
            itoa(i, numero, 10);
            strcat(auxiliar, numero);
            fprintf(arch, "%s dd ?\n",auxiliar);
            i++;
            cantAux--;

            insertarEnLista(lista, auxiliar, tID);
        }
    }
    
    fprintf(arch, ".CODE\n");
    fprintf(arch, "START:\n");
    fprintf(arch, "mov AX, @DATA\n");
    fprintf(arch, "mov DS, AX\n");
    fprintf(arch, "mov es, ax\n\n");
    fclose(arch);

}