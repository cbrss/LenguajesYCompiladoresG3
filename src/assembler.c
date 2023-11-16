#include "../include/assembler.h"
#include "../include/pila.h"

int contOp = 1;
char auxAsm[STRING_LARGO_MAX + 1], auxAsmOp[STRING_LARGO_MAX + 1], auxAsmSimbolo[STRING_LARGO_MAX + 1];
char auxAsmTipo[STRING_LARGO_MAX + 1], auxAsmIf[ID_LARGO_MAX+ 1];
char auxTipo[STRING_LARGO_MAX + 1], auxValor[STRING_LARGO_MAX + 1];
int existeElse = 0; //si es 1 entonces tengo doble condicion
int contFalso = 0; //
int contVerdadero = 0;
int contOr = 0;
int contCiclo = 0;
char etiquetaFalso[100]; //apilar etiquetas true y false
char etiquetaVerdadero[100];
char etiquetaOr[100];
char etiquetaCiclo[100];
char nro[100];
int operadorOr = 0;
Pila ifFalso, ifVerdadero, ifOr, cicloAnidados;

void imprimirEncabezado(Lista* listaSimbolos, int cantidadAuxiliares);

void incializarPilas(){
    crearPila(&ifFalso);
    crearPila(&ifVerdadero);
    crearPila(&ifOr);
    crearPila(&cicloAnidados);

}

void generarAssembler(Lista* listaSimbolos, Arbol* arbol, int cantAuxiliares){
    FILE *arch = fopen("final.asm", "a");
    incializarPilas(ifFalso, ifVerdadero, ifOr, cicloAnidados);

    imprimirEncabezado(listaSimbolos, cantAuxiliares);

    generarCodigo(arch, listaSimbolos, arbol);

    imprimirFin(arch);

    fclose(arch);
}

void generarCodigo(FILE* arch, Lista* listaSimbolos, Arbol* arbol){
    NodoA* padre = padreMasIzq(arbol);
    NodoA* condicion;
    NodoA* bloque;
    
    while(padre!= NULL){
        printf("\n*%s*\n", padre->simbolo);

        if(strcmp(padre->simbolo, "BloEjec") == 0){
            generarCodigo(arch, listaSimbolos, &padre->der);
        }

        if(strcmp(padre->simbolo, "=") == 0){
      
            strcpy(auxTipo, obtenerTipo(listaSimbolos, padre->izq->simbolo));
            
         
            generarCodigo(arch, listaSimbolos, &padre->der);
            if(strcmp(auxTipo, "String") == 0){
                strncpy(auxValor, padre->der->simbolo + 1, strlen(padre->der->simbolo) - 2); // substring del simbolo sin las ""
                fprintf(arch, "LEA EAX, %s\nMOV %s, EAX\n", obtenerNombre(listaSimbolos, auxValor, TSTRING), padre->izq->simbolo);
            } else if(strcmp(auxTipo, "Float") == 0){
                fprintf(arch, "FLD %s\n", padre->der->simbolo);
                fprintf(arch, "FSTP %s\n", padre->izq->simbolo);
            } else if(strcmp(auxTipo, "Int") == 0){
                fprintf(arch, "FRNDINT\n");
                fprintf(arch, "FSTP %s\n", padre->izq->simbolo);
            } 
            //TODO: si es String usar _2_+_42, posiblemente buscar en TS

            

            contOp = 1;
          
        }
        if(esOperacionAritmetica(padre->simbolo)  == 1){

            //generarCodigo(arch, listaSimbolos, &padre->izq);
            generarCodigo(arch, listaSimbolos, &padre->der);
            if(strcmp(padre->simbolo, "+") == 0){
                
                fprintf(arch, "FLD %s\n", padre->izq->simbolo);
                fprintf(arch, "FLD %s\n", padre->der->simbolo);
                fprintf(arch, "FADD\n");
                if(strcmp(padre->izq->simbolo, "@_contI") == 0){
                    fprintf(arch, "FSTP @_contI\n");

                } else{
                    fprintf(arch, "FSTP @aux%d\n", contOp);

                    strcpy(auxAsm, "@aux");
                    itoa(contOp, auxAsmOp, 10);
                    strcat(auxAsm, auxAsmOp);

                    strcpy(padre->simbolo, auxAsm);
                }
                
            }
            else if(strcmp(padre->simbolo, "-") == 0){
                fprintf(arch, "FLD %s\n", padre->izq->simbolo);
                fprintf(arch, "FLD %s\n", padre->der->simbolo);
                fprintf(arch, "FSUB\n");

                fprintf(arch, "FSTP @aux%d\n", contOp);

                strcpy(auxAsm, "@aux");
                itoa(contOp, auxAsmOp, 10);
                strcat(auxAsm, auxAsmOp);

                strcpy(padre->simbolo, auxAsm);
            }
            else if(strcmp(padre->simbolo, "*") == 0){
                fprintf(arch, "FLD %s\n", padre->izq->simbolo);
                fprintf(arch, "FLD %s\n", padre->der->simbolo);
                fprintf(arch, "FMUL\n");

                fprintf(arch, "FSTP @aux%d\n", contOp);

                strcpy(auxAsm, "@aux");
                itoa(contOp, auxAsmOp, 10);
                strcat(auxAsm, auxAsmOp);

                strcpy(padre->simbolo, auxAsm);
            }
            else if(strcmp(padre->simbolo, "/") == 0){
                fprintf(arch, "FLD %s\n", padre->izq->simbolo);
                fprintf(arch, "FLD %s\n", padre->der->simbolo);
                fprintf(arch, "FDIV\n");

                fprintf(arch, "FSTP @aux%d\n", contOp);

                strcpy(auxAsm, "@aux");
                itoa(contOp, auxAsmOp, 10);
                strcat(auxAsm, auxAsmOp);

                strcpy(padre->simbolo, auxAsm);
                   
            }
           
            contOp++;
            
        }
       
        if(strcmp(padre->simbolo, "Write") == 0){
            
      
            if(strcmp(padre->der->simbolo, "Int") == 0){
                fprintf(arch, "DisplayInteger %s\n", padre->izq->simbolo);
            } else if (strcmp(padre->der->simbolo, "Float") == 0){
                fprintf(arch, "DisplayFloat %s , 2\n", padre->izq->simbolo);
            } else if (strcmp(padre->der->simbolo, "String") == 0){
                fprintf(arch, "displayString %s\n", padre->izq->simbolo);
            } else{ //este caso imprimo un string literal
                obtenerSinComillas(auxValor, padre->izq->simbolo);
                fprintf(arch, "displayString %s\n", obtenerNombre(listaSimbolos, auxValor, TSTRING));
            }
            fprintf(arch, "newLine 1\n");
           
            
        }
        
        if(strcmp(padre->simbolo, "READ") == 0){
            if(strcmp(padre->der->simbolo, "Int") == 0){
                fprintf(arch, "getInteger %s\n", padre->izq->simbolo);
            } else if(strcmp(padre->der->simbolo, "Float") == 0){
                fprintf(arch, "getFloat %s\n", padre->izq->simbolo);
            } else if(strcmp(padre->der->simbolo, "String") == 0){
                fprintf(arch, "getString %s\n", padre->izq->simbolo);
            }
        }

        if(strcmp(padre->simbolo, "if") == 0 ){
            if(strcmp(padre->der->simbolo, "Cuerpo") == 0){
                existeElse = 1;
            }
         
            if(esComparador(padre->izq->simbolo) == 1){   //condicion simple
           
                fprintf(arch, "fld %s\n", padre->izq->izq->simbolo);
                fprintf(arch, "fcomp %s\n", padre->izq->der->simbolo);

                
                fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                
                strcpy(etiquetaFalso, "falso");

                if(strcmp(padre->izq->simbolo, "<") == 0){
                    
                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JNB %s\n", etiquetaFalso);   //ini
                    
                } else if (strcmp(padre->izq->simbolo, ">") == 0){

                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JBE %s\n", etiquetaFalso);
                } else if (strcmp(padre->izq->simbolo, "<=") == 0){

                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JNBE %s\n", etiquetaFalso);
                } else if (strcmp(padre->izq->simbolo, ">=") == 0){
                
                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JNAE %s\n", etiquetaFalso);
                } else if (strcmp(padre->izq->simbolo, "!=") == 0){

                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JE %s\n", etiquetaFalso);
                } else if (strcmp(padre->izq->simbolo, "==") == 0){

                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JNE %s\n", etiquetaFalso);
                }

                apilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));

                contFalso++;
            } else if(esOperadorLogico(padre->izq->simbolo) == 1){ //condicion multiple
                NodoA* opLogico = padre->izq;

                if(strcmp(padre->izq->simbolo, "&") == 0){
                    fprintf(arch, "fld %s\n", opLogico->izq->izq->simbolo);
                    fprintf(arch, "fcomp %s\n", opLogico->izq->der->simbolo);

                    
                    fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                    fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                    
                    strcpy(etiquetaFalso, "falso");

                    if(strcmp(opLogico->izq->simbolo, "<") == 0){
                        
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNB %s\n", etiquetaFalso);   //ini
                        
                    } else if (strcmp(opLogico->izq->simbolo, ">") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, "<=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, ">=") == 0){
                    
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNAE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, "!=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, "==") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNE %s\n", etiquetaFalso);
                    }

                    //2da condicion
                
                    fprintf(arch, "fld %s\n", opLogico->der->izq->simbolo);
                    fprintf(arch, "fcomp %s\n", opLogico->der->der->simbolo);
                    
                    
                    fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                    fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                    
                    strcpy(etiquetaFalso, "falso");

                    if(strcmp(opLogico->der->simbolo, "<") == 0){
                        
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNB %s\n", etiquetaFalso);   //ini
                        
                    } else if (strcmp(opLogico->der->simbolo, ">") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, "<=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, ">=") == 0){
                    
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNAE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, "!=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, "==") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNE %s\n", etiquetaFalso);
                    }

                    apilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));
                    contFalso++;
                } else if(strcmp(padre->izq->simbolo, "||") == 0){ 
                    invertirCondicion(opLogico->izq);
                  
                    fprintf(arch, "fld %s\n", opLogico->izq->izq->simbolo);
                    fprintf(arch, "fcomp %s\n", opLogico->izq->der->simbolo);
                    
                    fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                    fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                    
                    strcpy(etiquetaFalso, "falso");

                    if(strcmp(opLogico->izq->simbolo, "<") == 0){
                        
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNB %s\n", etiquetaFalso);   //ini
                        
                    } else if (strcmp(opLogico->izq->simbolo, ">") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, "<=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, ">=") == 0){
                    
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNAE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, "!=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, "==") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNE %s\n", etiquetaFalso);
                    }
                    contFalso++;
                    //2da condicion ///ETIQUETA OR
                
                    fprintf(arch, "fld %s\n", opLogico->der->izq->simbolo);
                    fprintf(arch, "fcomp %s\n", opLogico->der->der->simbolo);
                
                    fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                    fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                    
                    strcpy(etiquetaOr, "etiquetaOr");

                    if(strcmp(opLogico->der->simbolo, "<") == 0){
                        
                        itoa(contOr, nro, 10);
                        strcat(etiquetaOr, nro);
                        fprintf(arch, "JNB %s\n", etiquetaOr);   //ini
                        
                    } else if (strcmp(opLogico->der->simbolo, ">") == 0){

                        itoa(contOr, nro, 10);
                        strcat(etiquetaOr, nro);
                        fprintf(arch, "JBE %s\n", etiquetaOr);
                    } else if (strcmp(opLogico->der->simbolo, "<=") == 0){

                        itoa(contOr, nro, 10);
                        strcat(etiquetaOr, nro);
                        fprintf(arch, "JNBE %s\n", etiquetaOr);
                    } else if (strcmp(opLogico->der->simbolo, ">=") == 0){
                    
                        itoa(contOr, nro, 10);
                        strcat(etiquetaOr, nro);
                        fprintf(arch, "JNAE %s\n", etiquetaOr);
                    } else if (strcmp(opLogico->der->simbolo, "!=") == 0){

                        itoa(contOr, nro, 10);
                        strcat(etiquetaOr, nro);
                        fprintf(arch, "JE %s\n", etiquetaOr);
                    } else if (strcmp(opLogico->der->simbolo, "==") == 0){

                        itoa(contOr, nro, 10);
                        strcat(etiquetaOr, nro);
                        fprintf(arch, "JNE %s\n", etiquetaOr);
                    }

                    if(existeElse == 1){
                        desapilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));
                        fprintf(arch, "%s:\n", etiquetaFalso);  
                        existeElse = 0;
                    }

                    apilar(&ifOr, etiquetaOr, sizeof(etiquetaOr));
                    contOr++;
                  
                    operadorOr = 1;
                }
            }

            if(strcmp(padre->der->simbolo, "Cuerpo") == 0){ //if con else

                if(operadorOr == 1){    //si op logico es OR || 
                    generarCodigo(arch, listaSimbolos, &padre->der->izq);  //true
                    strcpy(etiquetaVerdadero, "verdadero");
                    itoa(contVerdadero, nro, 10);
                    strcat(etiquetaVerdadero, nro);

                    contVerdadero++;
                    apilar(&ifVerdadero, etiquetaVerdadero, sizeof(etiquetaVerdadero));
                    fprintf(arch, "BI %s\n", etiquetaVerdadero);

                    

                    desapilar(&ifOr, etiquetaOr, sizeof(etiquetaOr));
                    fprintf(arch, "%s:\n", etiquetaOr);

                    generarCodigo(arch, listaSimbolos, &padre->der->der);  //false
                    desapilar(&ifVerdadero, etiquetaVerdadero, sizeof(etiquetaVerdadero));
                    fprintf(arch, "%s:\n", etiquetaVerdadero);  

                    
                    
                    operadorOr = 0;
                } else{                 // cualquier otro caso
                                    
                    generarCodigo(arch, listaSimbolos, &padre->der->izq);  //true
                    strcpy(etiquetaVerdadero, "verdadero");
                    itoa(contVerdadero, nro, 10);
                    strcat(etiquetaVerdadero, nro);

                    contVerdadero++;
                    apilar(&ifVerdadero, etiquetaVerdadero, sizeof(etiquetaVerdadero));
                    fprintf(arch, "BI %s\n", etiquetaVerdadero);

                    desapilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));
                    fprintf(arch, "%s:\n", etiquetaFalso);  
                    generarCodigo(arch, listaSimbolos, &padre->der->der);  //false
                    desapilar(&ifVerdadero, etiquetaVerdadero, sizeof(etiquetaVerdadero));
                    fprintf(arch, "%s:\n", etiquetaVerdadero);  
                }
 
            } else{ //if sin else
                if(operadorOr == 1){
                    //  TODO: cambiar etiqueta a verdadero
                    desapilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));
                    fprintf(arch, "%s:\n", etiquetaFalso);
                    generarCodigo(arch, listaSimbolos, &padre->der);
                    
                    desapilar(&ifOr, etiquetaOr, sizeof(etiquetaOr));
                    fprintf(arch, "%s:\n", etiquetaOr);
                    
                    operadorOr = 0;
                } else{
                    generarCodigo(arch, listaSimbolos, &padre->der);
                    desapilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));
                    fprintf(arch, "%s:\n", etiquetaFalso);
                }
            }
        }

        if(strcmp(padre->simbolo, "ciclo") == 0){
            strcpy(etiquetaCiclo, "etiquetaCiclo");
            itoa(contCiclo, nro, 10);
            strcat(etiquetaCiclo, nro);
            fprintf(arch, "%s\n", etiquetaCiclo);
            apilar(&cicloAnidados, etiquetaCiclo, sizeof(etiquetaCiclo));
            contCiclo++;

            if(esComparador(padre->izq->simbolo) == 1){   //condicion simple
           
                fprintf(arch, "fld %s\n", padre->izq->izq->simbolo);
                fprintf(arch, "fcomp %s\n", padre->izq->der->simbolo);

                
                fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                
                strcpy(etiquetaFalso, "falso");

                if(strcmp(padre->izq->simbolo, "<") == 0){
                    
                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JNB %s\n", etiquetaFalso);   //ini
                    
                } else if (strcmp(padre->izq->simbolo, ">") == 0){

                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JBE %s\n", etiquetaFalso);
                } else if (strcmp(padre->izq->simbolo, "<=") == 0){

                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JNBE %s\n", etiquetaFalso);
                } else if (strcmp(padre->izq->simbolo, ">=") == 0){
                
                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JNAE %s\n", etiquetaFalso);
                } else if (strcmp(padre->izq->simbolo, "!=") == 0){

                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JE %s\n", etiquetaFalso);
                } else if (strcmp(padre->izq->simbolo, "==") == 0){

                    itoa(contFalso, nro, 10);
                    strcat(etiquetaFalso, nro);
                    fprintf(arch, "JNE %s\n", etiquetaFalso);
                }

                apilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));

                contFalso++;
            } else if(esOperadorLogico(padre->izq->simbolo) == 1){ //condicion multiple
                NodoA* opLogico = padre->izq;

                if(strcmp(padre->izq->simbolo, "&") == 0){
                    fprintf(arch, "fld %s\n", opLogico->izq->izq->simbolo);
                    fprintf(arch, "fcomp %s\n", opLogico->izq->der->simbolo);

                    
                    fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                    fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                    
                    strcpy(etiquetaFalso, "falso");

                    if(strcmp(opLogico->izq->simbolo, "<") == 0){
                        
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNB %s\n", etiquetaFalso);   //ini
                        
                    } else if (strcmp(opLogico->izq->simbolo, ">") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, "<=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, ">=") == 0){
                    
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNAE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, "!=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->izq->simbolo, "==") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNE %s\n", etiquetaFalso);
                    }

                    //2da condicion
                
                    fprintf(arch, "fld %s\n", opLogico->der->izq->simbolo);
                    fprintf(arch, "fcomp %s\n", opLogico->der->der->simbolo);
                    
                    
                    fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                    fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                    
                    strcpy(etiquetaFalso, "falso");

                    if(strcmp(opLogico->der->simbolo, "<") == 0){
                        
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNB %s\n", etiquetaFalso);   //ini
                        
                    } else if (strcmp(opLogico->der->simbolo, ">") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, "<=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, ">=") == 0){
                    
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNAE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, "!=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, "==") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNE %s\n", etiquetaFalso);
                    }

                    apilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));
                    contFalso++;
                } else if(strcmp(padre->izq->simbolo, "||") == 0){ 
                    
                    invertirCondicion(opLogico->izq);
                  
                    fprintf(arch, "fld %s\n", opLogico->izq->izq->simbolo);
                    fprintf(arch, "fcomp %s\n", opLogico->izq->der->simbolo);
                    
                    fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                    fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                    
                    strcpy(etiquetaVerdadero, "verdadero");

                    if(strcmp(opLogico->izq->simbolo, "<") == 0){
                        
                        itoa(contVerdadero, nro, 10);
                        strcat(etiquetaVerdadero, nro);
                        fprintf(arch, "JNB %s\n", etiquetaVerdadero);   //ini
                        
                    } else if (strcmp(opLogico->izq->simbolo, ">") == 0){

                        itoa(contVerdadero, nro, 10);
                        strcat(etiquetaVerdadero, nro);
                        fprintf(arch, "JBE %s\n", etiquetaVerdadero);
                    } else if (strcmp(opLogico->izq->simbolo, "<=") == 0){

                        itoa(contVerdadero, nro, 10);
                        strcat(etiquetaVerdadero, nro);
                        fprintf(arch, "JNBE %s\n", etiquetaVerdadero);
                    } else if (strcmp(opLogico->izq->simbolo, ">=") == 0){
                    
                        itoa(contVerdadero, nro, 10);
                        strcat(etiquetaVerdadero, nro);
                        fprintf(arch, "JNAE %s\n", etiquetaVerdadero);
                    } else if (strcmp(opLogico->izq->simbolo, "!=") == 0){

                        itoa(contVerdadero, nro, 10);
                        strcat(etiquetaVerdadero, nro);
                        fprintf(arch, "JE %s\n", etiquetaVerdadero);
                    } else if (strcmp(opLogico->izq->simbolo, "==") == 0){

                        itoa(contVerdadero, nro, 10);
                        strcat(etiquetaVerdadero, nro);
                        fprintf(arch, "JNE %s\n", etiquetaVerdadero);
                    }

                    //apilar(&ifVerdadero, etiquetaVerdadero, sizeof(etiquetaVerdadero));

                    contVerdadero++;

                    //2da condicion ///ETIQUETA OR
                
                    fprintf(arch, "fld %s\n", opLogico->der->izq->simbolo);
                    fprintf(arch, "fcomp %s\n", opLogico->der->der->simbolo);
                
                    fprintf(arch, "fstsw ax\n");    //los flags del coprocesador en memoria
                    fprintf(arch, "sahf\n");        //guardo los flags que estan en memoria en el registro FLAG del cpu
                    
                    strcpy(etiquetaFalso, "falso");

                    if(strcmp(opLogico->der->simbolo, "<") == 0){
                        
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNB %s\n", etiquetaFalso);   //ini
                        
                    } else if (strcmp(opLogico->der->simbolo, ">") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, "<=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNBE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, ">=") == 0){
                    
                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNAE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, "!=") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JE %s\n", etiquetaFalso);
                    } else if (strcmp(opLogico->der->simbolo, "==") == 0){

                        itoa(contFalso, nro, 10);
                        strcat(etiquetaFalso, nro);
                        fprintf(arch, "JNE %s\n", etiquetaFalso);
                    }

                 
                    apilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));

                    //desapilar(&ifVerdadero, etiquetaVerdadero, sizeof(etiquetaVerdadero));
                    fprintf(arch, "%s:\n", etiquetaVerdadero);
                    contFalso++;
                }
            }
            generarCodigo(arch, listaSimbolos, &padre->der);
            
            desapilar(&cicloAnidados, etiquetaCiclo, sizeof(etiquetaCiclo));
            fprintf(arch, "BI %s\n", etiquetaCiclo);
           
            desapilar(&ifFalso, etiquetaFalso, sizeof(etiquetaFalso));
            fprintf(arch, "%s:\n", etiquetaFalso);

        }


        borrarHijos(padre);
        padre = padreMasIzq(arbol);
    }

}


void imprimirFin(FILE* arch){
	fprintf(arch, "\nMOV EAX, 4C00H\n");
	fprintf(arch, "INT 21h\n");
	fprintf(arch, "END START");

}

void invertirCondicion(NodoA* padre){
    if(strcmp(padre->simbolo, "<") == 0){
        strcpy(padre->simbolo, ">=");
    }
    else if(strcmp(padre->simbolo, ">") == 0){
        strcpy(padre->simbolo, "<=");
    }
    else if(strcmp(padre->simbolo, "<=") == 0){
        strcpy(padre->simbolo, ">");
    }
    else if(strcmp(padre->simbolo, ">=") == 0){
        strcpy(padre->simbolo, "<");
    }
    else if(strcmp(padre->simbolo, "==") == 0){
        strcpy(padre->simbolo, "!=");
    }
    else if(strcmp(padre->simbolo, "!=") == 0){
        strcpy(padre->simbolo, "==");
    }
}
int esComparador(char* op){
    if(strcmp(op, "<") == 0 ||
        strcmp(op, "<=") == 0 ||
        strcmp(op, ">") == 0 ||
        strcmp(op, ">=") == 0 ||
        strcmp(op, "==") == 0 ||
        strcmp(op, "!=") == 0 )
        return 1;

    return 0;
}
int esOperacionAritmetica(char* op){
    if(strcmp(op, "+") == 0 ||
        strcmp(op, "-") == 0 ||
        strcmp(op, "*") == 0 ||
        strcmp(op, "/") == 0)  
        return 1;
    
    return 0;
}
int esOperadorLogico(char* op){
    if(strcmp(op, "&") == 0 ||
        strcmp(op, "||") == 0)
        return 1;

    return 0;
}