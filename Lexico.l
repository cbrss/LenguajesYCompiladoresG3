%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>

    #include "tab_simb.h"
    #include "y.tab.h"
    
    FILE* yyin;
    char* yyltext;

    int fa_validarLongitudID(char*);
    int fa_verificarRangoInt(char*);
    int fa_verificarRangoFloat(char*);
    int fa_verificarRangoStr(char*);
%}

/* OPCIONES */
/* Input en un solo archivo */
%option noyywrap
/* Contador de lineas */
%option yylineno

/* TOKENS */
DIGITO      [0-9]
LETRA		[a-zA-Z]
INT		    {DIGITO}+
FLOAT       {DIGITO}+\.{DIGITO}*|{DIGITO}*\.{DIGITO}+
STRING      \"[^"]*\"
COMEN_A             \*-
COMEN_CONT          [^*]*
COMEN_C             -\*
COMENTARIO          {COMEN_A}{COMEN_CONT}{COMEN_C}
COMENTARIO_ANIDADO  {COMEN_A}({COMEN_CONT}*{COMENTARIO}+{COMEN_CONT}*){COMEN_C}
/* Palabras reservadas */
INIT        "init"
DEC_INT     "Int"
DEC_FLOAT   "Float"
DEC_STRING  "String"
NOT         "not"
IF          "if"
ELSE        "else"
CICLO       "ciclo"
/* Funciones built-in */
WRITE       "write"
READ        "read"
CONCAT      "concatenarConRecorte"
TIMER       "timer"
ESTA_CONT   "estaContenido"
/* ID, TODO: Agregar guiones? */
ID          {LETRA}({LETRA}|{DIGITO}|_)*
/* Caracteres especiales */
PA          "("
PC		    ")"
LLA         "{"
LLC         "}"
COMA        ","
DOS_P       ":"
/* Operadores aritmeticos */
OP_AS   	"="
OP_SUM		"+"
OP_MUL      "*"
OP_RES		"-"
OP_DIV      "/"
/* Operadores logicos */
AND         "&"
OR          "||"
/* Comparadores */
MAYOR       ">"
MENOR       "<"
IGUAL       "=="
DISTINTO    "!="
MAYOR_IGUAL ">="
MENOR_IGUAL "<="

/* Reglas */
%% 
{INIT}          {return INIT;}
{DEC_INT}       {return DEC_INT;}
{DEC_FLOAT}     {return DEC_FLOAT;}
{DEC_STRING}    {return DEC_STRING;}
{INT}           {fa_verificarRangoInt(yytext); insertarEnLista(&lista, yytext, tINT); return INT;}
{FLOAT}         {fa_verificarRangoFloat(yytext); insertarEnLista(&lista, yytext, tFLOAT); return FLOAT;}
{STRING}        {fa_verificarRangoStr(yytext); insertarEnLista(&lista, yytext, tSTRING); return STRING;}
{COMENTARIO}            {}
{COMENTARIO_ANIDADO}    {}
 
{NOT}           {return NOT;}
{IF}            {return IF;}
{CICLO}         {return CICLO;}
{ELSE}          {return ELSE;}

{WRITE}         {return WRITE;}
{READ}          {return READ;}
{CONCAT}        {return CONCAT;}
{TIMER}         {return TIMER;}
{ESTA_CONT}     {return ESTA_CONT;}

{ID}            {fa_validarLongitudID(yytext); insertarEnLista(&lista, yytext, tID); return ID;}
 
{OP_SUM}        {return OP_SUM;}
{OP_AS}         {return OP_AS;}
{OP_MUL}        {return OP_MUL;}
{OP_RES}        {return OP_RES;}
{OP_DIV}        {return OP_DIV;}
 
{PA}            {return PA;}
{PC}            {return PC;}
{LLA}           {return LLA;}
{LLC}           {return LLC;}
{COMA}          {return COMA;}
{DOS_P}         {return DOS_P;}
 
{AND}           {return AND;}
{OR}            {return OR;}

{MAYOR}         {return MAYOR;}
{MENOR}         {return MENOR;}
{IGUAL}         {return IGUAL;}
{DISTINTO}      {return DISTINTO;}
{MAYOR_IGUAL}   {return MAYOR_IGUAL;}
{MENOR_IGUAL}   {return MENOR_IGUAL;}

"\n"      		
"\t"
"\n\t"
" "             		
"\r\n"
.               {printf("[%d] Error, \"%s\" carácter inválido\n", yylineno, yytext); exit(1);}
%%



int fa_validarLongitudID(char* ptr) {
    long valor = strlen(ptr);

    if ( valor < 1 || valor > ID_LARGO_MAX) {
        printf ("[%d] Error, largo de ID fuera de rango\n", yylineno);
        return 1;
    }
}

int fa_verificarRangoInt(char* ptr) {
    long long valor = atoi(ptr);

    if ( valor < 0 || valor > MAX_INT) {
        printf ("[%d] Error, variable Int fuera de rango\n", yylineno);
        return 1;
    }
}

int fa_verificarRangoFloat(char* ptr) {
    double valor = atof(ptr);

    if ( valor < 0 || valor > 2147483648) {
        printf ("[%d] Error, variable Float fuera de rango\n", yylineno);
        return 1;
    }
}

int fa_verificarRangoStr(char* ptr) {
    long valor = strlen(ptr);

    if ( valor < 2 || valor > STRING_LARGO_MAX + 2) {
        printf ("[%d] Error, variable String fuera de rango\n", yylineno);
        return 1;
    }
}
