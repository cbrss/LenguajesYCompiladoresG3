%{ 
    //borrar nodos declaracion
    #include "include/constantes.h"
    #include "include/tab_simb.h"
    #include "y.tab.h"
    #include "include/arbol.h"
    #include "include/pila.h"
    #include "include/cola.h"
    #include "include/assembler.h"
    
    int yystopparser=0;
    extern FILE* yyin;

    int yyerror();
    int yylex();

    char* concatenar(char*, char*, int);
    int estaContenido(char*, char*);


    void maximo();

    Arbol compilado;
    Lista listaSimbolos;
    Lista listaIds;
    Pila anidaciones;
    Pila condAnidados;


    Cola colaIds;
    
    int boolCompiladoOK = 1;

    

    NodoA* CompiladoPtr, *ProgramaPtr, *DeclaPtr, *BloPtr, *DecPtr, *ListPtr, *SentPtr, *AsigPtr, *tipoAux,
            *CicPtr, *EvalPtr, *Eptr, *StrPtr, *ConPtr, *CmpPtr, *EptrAux, *BloAux, *Tptr, *Fptr, *CmpAux, *StrPtrAux;
    NodoA* EjePtr, * ConAux;
    NodoA* DecAsigPTr,* DecAsigMPtr,* ParamAsigPtr,* CtePtr, * ParamContPtr;
    char AuxDec[ID_LARGO_MAX + 1];
    NodoA* AuxPtr;
    NodoA * auxCont;
    char  auxTipo[7], strAux[VALOR_LARGO_MAX + 1], strAux2[VALOR_LARGO_MAX + 1], cmpAux[3], opAux[3];
    char strAuxAsig[VALOR_LARGO_MAX + 1];
    char auxValor[VALOR_LARGO_MAX];
    int intAux;
    float floatAux;
    int contador;
    int cantidadAuxiliares = 0;
    int contadorAuxiliares = 0; 

   
%}

 
%token <int_val>INT    
%token <float_val>FLOAT      
%token <string_val>STRING    
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
%token ASIGCOMP
%token CONT

// ID
%token <string_val>ID
// Caracteres especiales   
%token PA        
%token PC     
%token CA
%token CC   
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

%union { 
    int int_val;
    float float_val;
    char string_val[50];
} 
 
%%
programa_prima: 
    programa    { compilado = ProgramaPtr;
                 if(boolCompiladoOK == 1){
                    printf("R1: COMPILACION EXITOSA\n");
                    imprimirArbolTexto(&compilado);
                    imprimirArbolGrafico(&compilado);
                    
                    /*imprimirEncabezado(&listaSimbolos, cantidadAuxiliares - 1);
                    FILE *arch = fopen("final.asm", "a");
                    generar_assembler(&compilado, arch);
                    generar_fin(arch);
                    fclose(arch);*/
              
                    generarAssembler(&listaSimbolos, &compilado, cantidadAuxiliares - 1);

                }
                else{
                    printf("R1: ERROR DE COMPILACION\n");
                }
                ;  }
    ;
programa: 
    INIT LLA declaraciones LLC bloque_ejec  { printf("\tR2: init { declaraciones} bloque_ejec es Programa\n"); ProgramaPtr = crearNodo("Programa", BloPtr, NULL); } 
    |INIT LLA LLC bloque_ejec               { printf("\tR3: init { } bloque_ejec es Programa\n"); ProgramaPtr = BloPtr; }
    | INIT LLA declaraciones LLC            { printf("\tRx8: init { declaraciones} es Programa\n"); ProgramaPtr = crearNodo("Programa", NULL, NULL); } 
    ;

declaraciones: 
    dec                 { printf("\tR4: dec es Declaraciones\n"); }
    |declaraciones dec  { printf("\tR5: declaraciones dec es Declaraciones\n"); }
    ;

dec: 
    listado_ids DOS_P tipo  { 
        printf("\t\tR6: listado_ids : tipo es Dec\n");
        asignarTipo(&listaIds, auxTipo);
        fusionarLista(&listaSimbolos, &listaIds);
     
        eliminarDeLista(&listaIds, AuxDec);
        DecPtr = crearNodo(":", crearHoja(AuxDec), crearHoja(auxTipo));
       
        while(!listaVacia(&listaIds)){
            eliminarDeLista(&listaIds, AuxDec);
        }
        //vaciarLista(&listaIds);
        
        
    }
    ;

listado_ids:
    ID  { 
        printf("\t\tR7: id es Listado_ids\n");

        if(idDeclarado(&listaSimbolos, $1)){ 
            printf("\nError, id: *%s* ya fue declarado\n", $1);
            return 1;
        };
        insertarEnLista(&listaIds, $1, tID);
        ListPtr = crearHoja($1);
    }
    |listado_ids COMA ID    { 
        printf("\t\tR8: listado_ids , id es Listado_ids\n"); 
        if(idDeclarado(&listaSimbolos, $3)){ 
            printf("\nError, id: *%s* ya fue declarado\n", $3);
            return 1;
        };
        
        insertarEnLista(&listaIds, $3, tID); 
        ListPtr = crearNodo(",", ListPtr, crearHoja($3));
    }
    ;

tipo: 
    DEC_INT     { printf("\t\tR9: dec_int es Tipo\n"); strcpy(auxTipo, TINT); }
    |DEC_FLOAT  { printf("\t\tR10: dec_float es Tipo\n"); strcpy(auxTipo, TFLOAT); }
    |DEC_STRING { printf("\t\tR11: dec_string es Tipo\n"); strcpy(auxTipo, TSTRING); }
    ;

bloque_ejec: 
    sentencia { printf("\tR12: sentencia es Bloque_ejec\n"); BloPtr = SentPtr; }
    |bloque_ejec { apilar(&anidaciones, &BloPtr, sizeof(BloPtr)); } sentencia { 
        printf("\tR13: bloque_ejec sentencia es Bloque_ejec\n"); 
        desapilar(&anidaciones, &BloAux, sizeof(BloAux));
        BloPtr = crearNodo("BloEjec", BloAux, SentPtr);
    }
    ;

sentencia:        
    asignacion  { printf("\t\tR14: asignacion es Sentencia\n"); SentPtr = AsigPtr; }
    |ciclo      { printf("\t\tR15: ciclo es Sentencia\n"); SentPtr = CicPtr; }
    |eval       { printf("\t\tR16: eval es Sentencia\n"); SentPtr = EvalPtr; }
    |TIMER PA INT { intAux = yylval.int_val; } COMA bloque_ejec PC { 
        printf("\t\tR17: timer(int,bloque_ejec) es Sentencia\n");
        snprintf(strAux, sizeof(intAux), "%d", intAux);
        SentPtr = crearNodo(
                "ciclo", 
                crearNodo("<", crearHoja("@_timerI"), crearHoja(strAux)),
                    crearNodo(
                        "BloEjec", BloPtr, 
                        crearNodo("BloEjec", crearNodo("=", crearHoja("@_timerI"), crearHoja("0")),crearNodo("=", crearHoja("@_timerI"), crearNodo("+", crearHoja("@_timerI"), crearHoja("1")))
                )
            )
        );
        
        insertarEnLista(&listaSimbolos, "@_timerI",  tID);
        asignarTipoDato(&listaSimbolos, "@_timerI", "Int");
        contadorAuxiliares++;
        contadorAuxiliares++;
        maximo();
        ;
    }
    |WRITE PA ID PC { 
        printf("\t\tR18: write(id) es Sentencia\n"); 
        if(!idDeclarado(&listaSimbolos, $3)){ 
            printf("\nError, id: *%s* no fue declarado\n", $3);
            return 1;
        };

        strcpy(auxTipo, obtenerTipo(&listaSimbolos, $3));
        SentPtr = crearNodo("Write", crearHoja($3), crearHoja(auxTipo));
    }
    |WRITE PA STRING PC { printf("\t\tR19: write(string) es Sentencia\n");
        SentPtr = crearNodo("Write", crearHoja($3), crearHoja("Literal")); }

    |READ PA ID PC      { 
        printf("\t\tR20: read(id) es Sentencia\n"); 
        if(!idDeclarado(&listaSimbolos, $3)){ 
            printf("\nError, id: *%s* no fue declarado\n", $3);
            return 1;
        };
        strcpy(auxTipo, obtenerTipo(&listaSimbolos, $3));
        SentPtr = crearNodo("READ", crearHoja($3), crearHoja(auxTipo));
    }
    |ASIGCOMP PA CA dec_asig_mul CC PC {
        printf("\t\tRespecial1: asigcomp ( [ dec_asig ]) es Sentencia\n");
        SentPtr = DecAsigMPtr;

    }
    
    |ID OP_AS CONT PA expresion {auxCont=Eptr;} DOS_P DOS_P CA param_cont_mul CC PC {
        printf("\t\tRespecial2: !cont(expresion :: [param_cont_mul]) es Sentencia\n");

        //270 talvez sacarlo y meter en "bloqueEjec" el if
        SentPtr = crearNodo("BloEjec", 
                                ParamContPtr, 
                                    crearNodo("if",
                                        crearNodo("==", crearHoja("@_contI"), crearHoja("_0")),
                                        crearNodo("Cuerpo",
                                            crearNodo("=", crearHoja($1), crearHoja("__menos_1")),
                                            crearNodo("=", crearHoja($1), crearHoja("@_contI"))
                                        )
                                    )
                                
                            );
       //verificar que ID sea float o int
        contadorAuxiliares++;
        contadorAuxiliares++;
        maximo();
        
        insertarEnLista(&listaSimbolos, "1", tINT);
        insertarEnLista(&listaSimbolos, "0", tINT);
        insertarEnLista(&listaSimbolos, "-1", tINT);
        insertarEnLista(&listaSimbolos, "@auxExp", tID);
        asignarTipoDato(&listaSimbolos, "@auxExp", "Float");
        insertarEnLista(&listaSimbolos, "@_contI",  tID);
        asignarTipoDato(&listaSimbolos, "@_contI", "Int");
        
    }
    
    ;



dec_asig_mul: 
    ID {encolar(&colaIds, $1, STRING_LARGO_MAX + 1); printf("Encolando: %s", $1);} CC DOS_P CA param_asig {
        printf("\t\t\tRx3: ] : [ es cierre de Dec_asig_mul\n");

        desencolar(&colaIds, strAux, STRING_LARGO_MAX + 1);
        printf("desencolando: %s\n", strAux);
        if(!idDeclarado(&listaSimbolos, strAux)){ 
            printf("\nError, id: *%s* no fue declarado\n", $1);
            return 1;
        };
        printf("\nse asigna: *%s* a *%s*\n ", strAux, strAuxAsig);
        if(!esMismoTipo(&listaSimbolos, strAux, strAuxAsig)){ 
            printf("\nError, datos de diferente tipo.\n");
            return 1;
        }
        DecAsigMPtr = crearNodo("=", crearHoja(strAux), ParamAsigPtr);
      
        }
    |ID COMA {encolar(&colaIds, $1, STRING_LARGO_MAX + 1);} dec_asig_mul COMA param_asig {
        printf("\t\t\tRx2: , id dec_asig_mul param_asig , es Dec_asig_mul\n");
        /*tal vez rompe el sizeof*/
        desencolar(&colaIds, strAux, STRING_LARGO_MAX + 1);
        if(!idDeclarado(&listaSimbolos, strAux)){ 
            printf("\nError, id: *%s* no fue declarado\n", $1);
            return 1;
        };
    
        if(!esMismoTipo(&listaSimbolos, strAux, strAuxAsig)){ 
            printf("\nError, datos de diferente tipo.\n");
            return 1;
        }
        DecAsigMPtr = crearNodo("decAsigM", DecAsigMPtr, crearNodo("=", crearHoja(strAux), ParamAsigPtr));
    } 
    ;
param_asig: 
    ID  {printf("\t\t\t  Rx3: id es Param_asig\n");
        if(!idDeclarado(&listaSimbolos, $1)){ 
                printf("\nError, id: *%s* no fue declarado\n", $1);
                return 1;
        };
        //necesito auxtipo de $1
     
        
        strcpy(strAuxAsig, obtenerTipo(&listaSimbolos, $1));

        ParamAsigPtr = crearHoja($1);
        
    }
    | cte {printf("\t\t\t Rx4: cte es Param_asig\n");
        ParamAsigPtr = CtePtr;
    }
    ;
/**/

param_cont_mul:
    param_asig  {printf("\t\t\tRx6: param_asig es param_cont_mul\n");
     
        ParamContPtr = crearNodo("BloEjec", 
                                        crearNodo("BloEjec", crearNodo("=", crearHoja("@_contI"), crearHoja("_0")),crearNodo("=",crearHoja("@auxExp"), auxCont)), 
                                        crearNodo("if", 
                                            crearNodo("!=", crearHoja("@auxExp"), ParamAsigPtr), 
                                            crearNodo("+", crearHoja("@_contI"), crearHoja("_1"))));
        
    }
    | param_cont_mul COMA param_asig {printf("\t\t\tRx6: param_asig , param_asig es param_cont_mul\n");
        ParamContPtr = crearNodo("BloEjec", ParamContPtr, crearNodo("if", crearNodo("!=", crearHoja("@auxExp"), ParamAsigPtr), crearNodo("+", crearHoja("@_contI"), crearHoja("_1"))));
    }
    ; 

cte: 
    INT {printf("\t\t\t  Rx4: int es Cte\n"); 
        snprintf(strAux, sizeof($1), "_%d", $1);
        CtePtr = crearHoja(strAux);
        strcpy(strAuxAsig, "Int");
    }
    | FLOAT  {printf("\t\t\t  Rx5: float es Cte\n");
        snprintf(strAux, MIN(sizeof($1), VALOR_LARGO_MAX), "_%.2f", $1);
        CtePtr = crearHoja(strAux);
        strcpy(strAuxAsig, "Float");
    }
    | STRING    {printf("\t\t\t  Rx6: string es Cte\n");
        CtePtr = crearHoja($1);
        strcpy(strAuxAsig, "String");
        
    } 
    ;

 
 //TODO: falta verificar que si tengo un id int, no se le asigne un float o string
 // en ID OP_AS string, no hay problema, en la regla expresion talvez haya que guardar un string con el tipo de la expresion(float, int) en un auxiliar y enviarselo 
 // a una funcion de la lista ***
asignacion:
    ID OP_AS expresion { 
        printf("\t\tR21: ID = Expresion es ASIGNACION\n");
        if(!idDeclarado(&listaSimbolos, $1)){ 
            printf("\nError, id: *%s* no fue declarado\n", $1);
            return 1;
        };
        if(!esMismoTipo(&listaSimbolos, $1, auxTipo)){ 
            printf("\nError, datos de diferente tipo.\n");
            return 1;
        }
        
        AsigPtr = crearNodo("=", crearHoja($1), Eptr);
        maximo();
        printf("\n\ncontador en exp: %d\n\n",contadorAuxiliares);
        contadorAuxiliares=0;
    }
    |ID OP_AS string  { 
        printf("\t\tR22: ID = String es ASIGNACION\n"); 
        if(!idDeclarado(&listaSimbolos, $1)){ 
            printf("\nError, id: *%s* no fue declarado\n", $1);
            return 1;
        }
        if(!esMismoTipo(&listaSimbolos, $1, TSTRING)){ 
            printf("\nError, datos de diferente tipo.\n");
            return 1;
        }
        AsigPtr = crearNodo("=", crearHoja($1), StrPtr);
        maximo();
        contadorAuxiliares=0;
    }
    ;

string:
    STRING  { 
        printf("\t\t\tR23: string es String\n");
        StrPtr = crearHoja($1);
        strcpy(auxTipo, TSTRING);
    }
    |CONCAT PA STRING { strcpy(strAux, $3); } COMA STRING { strcpy(strAux2, $6); } COMA INT PC { 
        printf("\t\t\tR24: concatenarConRecorte(String, String, Int) es String\n"); 
        StrPtr = crearHoja(concatenar(strAux, strAux2, yylval.int_val));
    }
    ;


ciclo: 
    CICLO PA condicion  PC LLA bloque_ejec  LLC    { 
        desapilar(&condAnidados, &ConAux, sizeof(ConAux));
        printf("\t\tR25: ciclo(Condicion) { bloque_ejec} es Ciclo\n"); CicPtr = crearNodo("ciclo", ConAux, BloPtr);
    }
    ;

eval: 
    IF PA condicion PC LLA bloque_ejec LLC { 
        printf("\t\tR26: if (condicion) { bloque_ejec} es Eval\n"); 
        desapilar(&condAnidados, &ConAux, sizeof(ConAux));
        EvalPtr = crearNodo("if", ConAux, BloPtr);
    }
    |IF PA condicion PC LLA bloque_ejec LLC{ apilar(&anidaciones, &BloPtr, sizeof(BloPtr)); } ELSE LLA bloque_ejec LLC { 
        printf("\t\tR27: if (condicion) { bloque_ejec} else { bloque_ejec} es Eval\n"); 
        desapilar(&condAnidados, &ConAux, sizeof(ConAux));
        desapilar(&anidaciones, &BloAux, sizeof(BloAux));   //el apilar de blo_ejec no funciona aca por que el else ejecuta otra instancia de bloque_Ejec
        EvalPtr = crearNodo("if", ConAux, crearNodo("Cuerpo", BloAux, BloPtr));
    }
    ;


condicion:
    comparacion { 
        printf("\t\t\tR28: comparacion es Condicion\n"); ConPtr = CmpPtr;
        apilar(&condAnidados, &ConPtr, sizeof(ConPtr));
    }
   // |comparacion { strcpy(cmpAux, CmpPtr->simbolo); } op_logico comparacion      { printf("\t\t\tcomparacion op_logico comparacion es Condicion\n"); ConPtr = crearNodo(opAux, crearHoja(cmpAux), CmpPtr); }
    |comparacion { CmpAux = CmpPtr; } op_logico comparacion { 
        printf("\t\t\tR29: comparacion op_logico comparacion es Condicion\n"); 
        ConPtr = crearNodo(opAux, CmpAux, CmpPtr);
        apilar(&condAnidados, &ConPtr, sizeof(ConPtr));
    }
    ;

comparacion:
    expresion { EptrAux = Eptr; } comparador expresion          { printf("\t\t\t\tR30: expresion comparador expresion es Comparacion \n"); 
        CmpPtr = crearNodo(cmpAux, EptrAux, Eptr); 
        contadorAuxiliares = 0;
    }
    |ESTA_CONT PA STRING { strcpy(strAux, $3); } COMA STRING PC { printf("\t\t\t\tR31: estaContenido(String, String) es Comparacion\n"); 
        if(estaContenido(strAux, yylval.string_val) == 1){
            CmpPtr = crearNodo("==", crearHoja("1"), crearHoja("1"));
        } else{
            CmpPtr = crearNodo("==", crearHoja("1"), crearHoja("0"));
        }
        
    }
    
    |NOT comparacion                                            { printf("\t\t\t\tR32: not comparacion es Comparacion\n"); CmpPtr = crearNodo("&", crearHoja("false"), CmpPtr); }
    |NOT expresion                                              { printf("\t\t\t\tR33: not expresion es Comparacion\n"); CmpPtr = crearNodo("&", crearHoja("false"), Eptr); }
    ;

op_logico:
    AND             { printf("\t\t\t\tR34: & es Op_logico\n"); strcpy(opAux,"&"); }
    |OR             { printf("\t\t\t\tR35: ||es Op_logico\n"); strcpy(opAux,"||"); }
    ;

comparador:
    MAYOR           { printf("\t\t\t\t  R36: > es Comparador\n"); strcpy(cmpAux,">"); }
    |MENOR          { printf("\t\t\t\t  R37: < es Comparador\n"); strcpy(cmpAux,"<"); }
    |IGUAL          { printf("\t\t\t\t  R38: == es Comparador\n"); strcpy(cmpAux,"=="); }
    |DISTINTO       { printf("\t\t\t\t  R39: != es Comparador\n"); strcpy(cmpAux,"!="); }
    |MAYOR_IGUAL    { printf("\t\t\t\t  R40: >= es Comparador\n"); strcpy(cmpAux,">="); }
    |MENOR_IGUAL    { printf("\t\t\t\t  R41: <= es Comparador\n"); strcpy(cmpAux,"<="); }
    ;

expresion:
    termino                     { printf("\t\t\t\tR42: Termino es Expresion\n"); Eptr = Tptr; contadorAuxiliares++;}
    |expresion OP_SUM termino   { printf("\t\t\t\tR43: Expresion+Termino es Expresion\n"); Eptr = crearNodo("+", Eptr, Tptr); contadorAuxiliares++;}
    |expresion OP_RES termino   { printf("\t\t\t\tR44: Expresion-Termino es Expresion\n"); Eptr = crearNodo("-", Eptr, Tptr); contadorAuxiliares++;}
    
    ;
 
termino:
    factor                  { printf("\t\t\t\t  R45: Factor es Termino\n"); Tptr = Fptr; }                                 
    |OP_RES factor          { printf("\t\t\t\t  R46: -Factor es Termino\n"); Tptr = crearNodo("*", crearHoja("_-1"), Fptr); insertarEnLista(&listaSimbolos, "-1", tINT );  contadorAuxiliares++;}
    |termino OP_MUL factor  { printf("\t\t\t\t  R47: Termino*Factor es Termino\n"); Tptr = crearNodo("*", Tptr, Fptr); contadorAuxiliares++;}
    |termino OP_DIV factor  { printf("\t\t\t\t  R48: Termino/Factor es Termino\n"); Tptr = crearNodo("/", Tptr, Fptr); contadorAuxiliares++;}
    ;

factor:
    ID  { 
        printf("\t\t\t\t    R50: ID es Factor \n"); 
        if(!idDeclarado(&listaSimbolos, $1)){ 
            printf("\nError, id: *%s* no fue declarado\n", $1);
            return 1;
        }
        if(esMismoTipo(&listaSimbolos, $1, TSTRING)){ 
            printf("\nError: No es posible realizar operaciones aritmeticas sobre variables String.\n");
            return 1;
        }
        strcpy(auxTipo, obtenerTipo(&listaSimbolos, $1)); // Se copia en auxTipo el tipo de la ID (Ojo cuando escala a termino y se pisa)
        Fptr= crearHoja($1); 
    }
    |INT   { 
        printf("\t\t\t\t    R51: INT es Factor %d\n", $1); 
        snprintf(strAux, VALOR_LARGO_MAX, "_%d", $1);
        strcpy(auxTipo, TINT);
        Fptr= crearHoja(strAux); 
    }
    |FLOAT { 
        printf("\t\t\t\t    R52: FLOAT es Factor\n"); 
        snprintf(strAux, VALOR_LARGO_MAX, "_%.2f", $1);
      
        strcpy(auxTipo, TFLOAT);
        Fptr= crearHoja(strAux);
    }
    |PA expresion PC    { printf("\t\t\t\t    R53: Expresion entre parentesis es Factor\n"); Fptr = Eptr; }
    ;
%%
 
int main(int argc, char *argv[]) { 
  
    crearLista(&listaSimbolos);
    crearLista(&listaIds);
    crearPila(&anidaciones);
    crearPila(&condAnidados);
    crearCola(&colaIds);


    if((yyin = fopen(argv[1], "rt"))==NULL) { 
        printf("\nNo se puede abrir el archivo de prueba: %s\n", argv[1]);
    }
    else { 
        yyparse();
    }
    fclose(yyin);

    imprimirLista(&listaSimbolos);
    

    vaciarLista(&listaSimbolos);
    vaciarLista(&listaIds);
    vaciarPila(&anidaciones);
    vaciarPila(&condAnidados);
    vaciarArbol(&compilado);
    vaciarCola(&colaIds);


    return 0;
}
 
int yyerror() { 
    printf("Error sintáctico\n");
    exit(1);
}

char* concatenar(char* str1, char* str2, int n){ 

    if(strlen(str1) <= n+2 ||strlen(str2) <= n+2){  //+2 por ""
        return "ERROR";
    } 


    char aux [strlen(str1) + strlen(str2) + 3]; //si n=0
    aux[0] = '"';

    strcpy(aux+ 1, str1+n+1); 
    strcpy(aux + strlen(aux) - 1, str2+n+1);  
    strcpy(str1, aux);

    if(strlen(str1) >= STRING_LARGO_MAX + 3 ){   //+3 "" \0
        return "ERROR";
    }
   
    return str1;
}

int estaContenido(char* str1, char* str2){ 
    char aux1[STRING_LARGO_MAX];
    char aux2[STRING_LARGO_MAX];

    strncpy(aux1, str1+1, strlen(str1)-1);
    strncpy(aux2, str2+1, strlen(str2)-1);
    //strcat(aux1, '\0');
    //strcat(aux2, '\0');
    aux1[strlen(str1)-2] = '\0';
    aux2[strlen(str2)-2] = '\0';
    
    return strstr(aux1, aux2) != NULL;
}
void maximo(){
    
    if(contadorAuxiliares > cantidadAuxiliares){
        cantidadAuxiliares = contadorAuxiliares;
    }
}