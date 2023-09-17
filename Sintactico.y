%{
    #include "tab_simb.h"
    #include "y.tab.h"
 
    int yystopparser=0;
    FILE* yyin;

    int yyerror();
    int yylex();
%}

 
%token INT    
%token FLOAT      
%token STRING    
// Palabras reservadas
%token INIT      
%token DEC_INT    
%token DEC_FLOAT  
%token DEC_STRING
%token NOT      
%token IF      
%token ELSE    
%token CICLO
// Funciones builtin
%token WRITE    
%token READ
%token CONCAT
%token TIMER
%token ESTA_CONT
// ID
%token ID
// Caracteres especiales   
%token PA        
%token PC        
%token LLA      
%token LLC      
%token COMA      
%token DOS_P    
// Operadores aritmeticos
%token OP_AS
%token OP_SUM
%token OP_MUL
%token OP_RES
%token OP_DIV
// Operadores logicos
%token AND
%token OR
// Comparadores
%token MAYOR
%token MENOR
%token IGUAL
%token DISTINTO
%token MAYOR_IGUAL
%token MENOR_IGUAL
 
%%
programa_prima: programa    {printf("COMPILADO\n");}
    ;
programa: INIT LLA declaraciones LLC bloque_ejec    {printf("\tinit {declaraciones} bloque_ejec es Programa\n");} 
    | INIT LLA LLC bloque_ejec                      {printf("\tinit {} bloque_ejec es Programa\n");}
    ;

declaraciones: dec          {printf("\tdec es Declaraciones\n");}
    | declaraciones dec     {printf("\tdeclaraciones dec es Declaraciones\n");}
    ;

dec: listado_ids DOS_P tipo {printf("\t\tlistado_ids : tipo es Dec\n");}
    ;

listado_ids:
    ID                      {printf("\t\tid es Listado_ids\n");}
    |listado_ids COMA ID    {printf("\t\tlistado_ids , id es Listado_ids\n");}
    ;

tipo: DEC_INT       {printf("\t\tdec_int es Tipo\n");}
    | DEC_FLOAT     {printf("\t\tdec_float es Tipo\n");}
    | DEC_STRING    {printf("\t\tdec_string es Tipo\n");}
    ;

bloque_ejec: sentencia                  {printf("\tsentencia es Bloque_ejec\n");}
    | bloque_ejec sentencia            {printf("\tbloque_ejec sentencia es Bloque_ejec\n");}
    ;
sentencia:        
    asignacion                          {printf("\t\tasignacion es Sentencia\n");}
    |ciclo                              {printf("\t\tciclo es Sentencia\n");}
    |eval                               {printf("\t\teval es Sentencia\n");}
    |TIMER PA INT COMA bloque_ejec PC   {printf("\t\ttimer(int,bloque_ejec) es Sentencia\n");}
    |WRITE PA ID PC                     {printf("\t\twrite(id) es Sentencia\n");}
    |WRITE PA STRING PC                 {printf("\t\twrite(string) es Sentencia\n");}
    |READ PA ID PC                      {printf("\t\tread(id) es Sentencia\n");}
    ;
 
asignacion:
    ID OP_AS expresion {printf("\t\tID = Expresion es ASIGNACION\n");}
    |ID OP_AS string    {printf("\t\tID = String es ASIGNACION\n");}
    ;

string:
    STRING                                      {printf("\t\t\tstring es String\n");}
    |CONCAT PA STRING COMA STRING COMA INT PC   {printf("\t\t\ttconcatenarConRecorte(String, String, Int) es String\n");}
    ;

ciclo: CICLO PA condicion PC LLA bloque_ejec LLC    {printf("\t\tciclo(Condicion) {bloque_ejec} es Ciclo\n");}
    ;

eval: 
    IF PA condicion PC LLA bloque_ejec LLC                              {printf("\t\tif (condicion) {bloque_ejec} es Eval\n");}
    |IF PA condicion PC LLA bloque_ejec LLC ELSE LLA bloque_ejec LLC    {printf("\t\tif (condicion) {bloque_ejec} else {bloque_ejec} es Eval\n");}
    ;

condicion:
    comparacion                             {printf("\t\t\tcomparacion es Condicion\n");}
    |comparacion op_logico comparacion      {printf("\t\t\tcomparacion op_logico comparacion es Condicion\n");}
    ;

comparacion:
    expresion comparador expresion          {printf("\t\t\t\texpresion comparador expresion es Comparacion\n");}
    |ESTA_CONT PA STRING COMA STRING PC     {printf("\t\t\t\testaContenido(String, String) es Comparacion\n");}
    |NOT comparacion                        {printf("\t\t\t\tnot comparacion es Comparacion\n");}
    |NOT expresion                          {printf("\t\t\t\tnot expresion es Comparacion\n");}
    ;

op_logico:
    AND             {printf("\t\t\t\t& es Op_logico\n");}
    |OR             {printf("\t\t\t\t|| es Op_logico\n");}
    ;

comparador:
    MAYOR           {printf("\t\t\t\t  > es Comparador\n");}
    |MENOR          {printf("\t\t\t\t  < es Comparador\n");}
    |IGUAL          {printf("\t\t\t\t  == es Comparador\n");}
    |DISTINTO       {printf("\t\t\t\t  != es Comparador\n");}
    |MAYOR_IGUAL    {printf("\t\t\t\t  >= es Comparador\n");}
    |MENOR_IGUAL    {printf("\t\t\t\t  <= es Comparador\n");}
    ;

expresion:
    termino {printf("\t\t\t\tTermino es Expresion\n");}
    |expresion OP_SUM termino {printf("\t\t\t\tExpresion+Termino es Expresion\n");}
    |expresion OP_RES termino {printf("\t\t\t\tExpresion-Termino es Expresion\n");}
    ;
 
termino:
    factor {printf("\t\t\t\t  Factor es Termino\n");}
    |OP_RES factor  {printf("\t\t\t\t  -Factor es Termino\n");}
    |termino OP_MUL factor {printf("\t\t\t\t  Termino*Factor es Termino\n");}
    |termino OP_DIV factor {printf("\t\t\t\t  Termino/Factor es Termino\n");}
    ;
 
factor:
    ID {printf("\t\t\t\t    ID es Factor \n");}
    | INT {printf("\t\t\t\t    INT es Factor\n");}
    | FLOAT {printf("\t\t\t\t    FLOAT es Factor\n");}
    | PA expresion PC {printf("\t\t\t\t    Expresion entre parentesis es Factor\n");}
    ;
%%
 
int main(int argc, char *argv[]) {
    if((yyin = fopen(argv[1], "rt"))==NULL) {
        printf("\nNo se puede abrir el archivo de prueba: %s\n", argv[1]);
    }
    else {
        yyparse();
    }
    fclose(yyin);
    imprimirLista(&lista);
    return 0;
}
 
int yyerror() {
    printf("Error sintáctico\n");
    exit(1);
}