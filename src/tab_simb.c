#include "../include/tab_simb.h"
void sacarEspacios(char *str);
void sacarPuntos(char* str);
void borrarComas(char* str);
void sacarMenos(char* str);
void sacarDosPuntos(char* str);
void sacarEspacios(char* str);
void sacarDosPuntos(char* str);
void sacarOr(char* str);
void sacarAnd(char* str);
void sacarCondMayor(char* str);
void sacarCondMenor(char* str);
void sacarIgual(char* str);
void sacarNegar(char* str);

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
        strcpy(nuevo_simbolo.valor, nombre);
        sacarMenos(nuevo_simbolo.nombre);

        strcpy(nuevo_simbolo.tipo_dato, TINT);
        
        nuevo_simbolo.longitud = strlen(nombre);
    }
    else if (tDato == tFLOAT)
    {

        strcpy(nuevo_simbolo.nombre, "_");
        strcat(nuevo_simbolo.nombre, nombre);
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
        strcpy(nuevo_simbolo.valor, nNombre);
        borrarComas(nNombre);
        strcat(nuevo_simbolo.nombre, nNombre);
        strcpy(nuevo_simbolo.tipo_dato, TSTRING);
        
        nuevo_simbolo.longitud = longitud;

        sacarEspacios(nuevo_simbolo.nombre);
        sacarDosPuntos(nuevo_simbolo.nombre);
        sacarOr(nuevo_simbolo.nombre);
        sacarOr(nuevo_simbolo.nombre);
        sacarAnd(nuevo_simbolo.nombre);
        sacarCondMayor(nuevo_simbolo.nombre);
        sacarCondMenor(nuevo_simbolo.nombre);
        sacarNegar(nuevo_simbolo.nombre);
        sacarIgual(nuevo_simbolo.nombre);
        sacarIgual(nuevo_simbolo.nombre);

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
void borrarComas(char* str){
    char aux[STRING_LARGO_MAX + 7];
    int coma = 0;
    int j = 0;
    for(int i = 0; str[i]; i++){
        
        if(str[i] == ','){
            coma = 1;
            aux[i] = '_';
            aux[i+1] = 'c';
            aux[i+2] = 'o';
            aux[i+3] = 'm';
            aux[i+4] = 'a';
            aux[i+5] = '_';
            i++;
            j+=6;
        }
        aux[j] = str[i];
        j++;
    }
    aux[j]='\0';
    if(coma == 1){
        strcpy(str, aux);
    }
    
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
void sacarMenos(char* str){
    char aux[STRING_LARGO_MAX + 1];
    int menos = 0;
    int j = 0;
    for(int i = 0; str[i]; i++){
        
        if(str[i] == '-'){
            menos = 1;
            aux[i] = '_';
            aux[i+1] = 'm';
            aux[i+2] = 'e';
            aux[i+3] = 'n';
            aux[i+4] = 'o';
            aux[i+5] = 's';
            aux[i+6] = '_';

            i++;
            j+=7;
        }
        aux[j] = str[i];
        j++;
    }
    aux[j] = '\0';
    if(menos == 1){
        strcpy(str, aux);
    }

}

void sacarDosPuntos(char* str){
    char aux[STRING_LARGO_MAX + 1];
    int menos = 0;
    int j = 0;
    for(int i = 0; str[i]; i++){
        
        if(str[i] == ':'){
            menos = 1;
            aux[i] = '_';
            aux[i+1] = '2';
            aux[i+2] = 'p';
            aux[i+3] = 'u';
            aux[i+4] = 'n';
            aux[i+5] = 't';
            aux[i+6] = '_';

            i++;
            j+=7;
        }
        aux[j] = str[i];
        j++;
    }
    aux[j] = '\0';
    if(menos == 1){
        strcpy(str, aux);
    }

}
void sacarCondMenor(char* str){
    char aux[STRING_LARGO_MAX + 1];
    int menos = 0;
    int j = 0;
    for(int i = 0; str[i]; i++){
        
        if(str[i] == '<'){
            menos = 1;
            aux[i] = '_';
            aux[i+1] = 'm';
            aux[i+2] = 'e';
            aux[i+3] = 'n';
            aux[i+4] = 'o';
            aux[i+5] = 'r';
            aux[i+6] = '_';

            i++;
            j+=7;
        }
        aux[j] = str[i];
        j++;
    }
    aux[j] = '\0';
    if(menos == 1){
        strcpy(str, aux);
    }

}

void sacarCondMayor(char* str){
    char aux[STRING_LARGO_MAX + 1];
    int menos = 0;
    int j = 0;
    for(int i = 0; str[i]; i++){
        
        if(str[i] == '>'){
            menos = 1;
            aux[i] = '_';
            aux[i+1] = 'm';
            aux[i+2] = 'a';
            aux[i+3] = 'y';
            aux[i+4] = 'o';
            aux[i+5] = 'r';
            aux[i+6] = '_';

            i++;
            j+=7;
        }
        aux[j] = str[i];
        j++;
    }
    aux[j] = '\0';
    if(menos == 1){
        strcpy(str, aux);
    }

}

void sacarAnd(char* str){
    char aux[STRING_LARGO_MAX + 1];
    int menos = 0;
    int j = 0;
    for(int i = 0; str[i]; i++){
        
        if(str[i] == '&'){
            menos = 1;
            aux[i] = '_';
            aux[i+1] = 'a';
            aux[i+2] = 'n';
            aux[i+3] = 'd';
            aux[i+4] = 'l';
            aux[i+5] = 'o';
            aux[i+6] = '_';

            i++;
            j+=7;
        }
        aux[j] = str[i];
        j++;
    }
    aux[j] = '\0';
    if(menos == 1){
        strcpy(str, aux);
    }

}
void sacarOr(char* str){
    char aux[STRING_LARGO_MAX + 1];
    int menos = 0;
    int j = 0;
    for(int i = 0; str[i]; i++){
        
        if(str[i] == '|'){
            menos = 1;
            aux[i] = '_';
            aux[i+1] = 'o';
            aux[i+2] = 'r';
            aux[i+3] = '_';
            i++;
            j+=4;
        }
        aux[j] = str[i];
        j++;
    }
    aux[j] = '\0';
    if(menos == 1){
        strcpy(str, aux);
    }

}
void sacarIgual(char* str){
    char aux[STRING_LARGO_MAX + 1];
    int menos = 0;
    int j = 0;
    for(int i = 0; str[i]; i++){
        
        if(str[i] == '='){
            menos = 1;
            aux[i] = '_';
            aux[i+1] = 'i';
            aux[i+2] = 'g';
            aux[i+3] = '_';
            i++;
            j+=4;
        }
        aux[j] = str[i];
        j++;
    }
    aux[j] = '\0';
    if(menos == 1){
        strcpy(str, aux);
    }
}
void sacarNegar(char* str){
    char aux[STRING_LARGO_MAX + 1];
    int menos = 0;
    int j = 0;
    for(int i = 0; str[i]; i++){
        
        if(str[i] == '!'){
            menos = 1;
            aux[i] = '_';
            aux[i+1] = 'n';
            aux[i+2] = 'e';
            aux[i+3] = '_';
            i++;
            j+=4;
        }
        aux[j] = str[i];
        j++;
    }
    aux[j] = '\0';
    if(menos == 1){
        strcpy(str, aux);
        
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
        } else{ //si es string
            fprintf(arch, "%s db \"%s\" , '$', %d dup (?)\n", (*lista)->simb.nombre, (*lista)->simb.valor, (*lista)->simb.longitud);
        }
   

        lista = &(*lista)->sig;
    }


    int i = 1;
    if(cantAux != -1){
        while(cantAux != 0){
            char auxiliar[ID_LARGO_MAX];
            char numero[ID_LARGO_MAX];

            strcpy(auxiliar, "@aux");

            
 
            snprintf(numero, STRING_LARGO_MAX + 1, "%d", i);
            
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
