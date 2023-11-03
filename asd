void resolverOpAritmetica(NodoA* padre){
    if(esHoja(padre->izq) && esHoja(padre->der)){
        if(strcmp(padre->simbolo, "+") == 0){
            fprintf(arch, "FLD %s\n", padre->izq->simbolo);
            fprintf(arch, "FLD %s\n", padre->der->simbolo);
            fprintf(arch, "FADD\n");

            fprintf(arch, "FSTP @aux%d\n", contOp);

            strcpy(auxAsm, "@aux");
            itoa(contOp, auxAsmOp, 10);
            strcat(auxAsm, auxAsmOp);

            strcpy(padre->simbolo, auxAsm);
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

    resolverOpAritmetica(padre->izq);

}