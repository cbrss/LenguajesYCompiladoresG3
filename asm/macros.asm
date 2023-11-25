   
    
 ;macros2.asm
;These are macros for Assembly Language Programming
;Myron Berg
;Dickinson State University
;4/5/99


;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
getString                    macro  string        ;read string from keyboard
local  label1, label2, label3, label4, label5, label6, label7, label8

                        pushad
                        push    di
                        push    si
                      

                        lea    si, string
                        mov    bx, si

label1:                mov    ah, 1
                        int    21h
                        cmp    al, 0Dh
                        je      label2

                        cmp    al, 8
                        je      label8
                        jmp    label7

label8:                dec    si
                        cmp    si, bx
                        jl      label6
                      jmp    label1

label6:                mov    si, bx
                        jmp    label1
                        

label7:                mov    [si], al
                        inc    si
                        jmp    label1
label2:                mov    byte ptr [si], '$'

                        pop    si
                        pop    di
                        popad

endm    

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
displayString                  macro  string          ;write string on screen

                        push    dx
                        push    ax

                        lea    dx, string
                        mov    ah, 9
                        int    21h

                        pop    ax
                        pop    dx

endm

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
scrollup                macro      number    ;scroll screen up

                        pusha 
                        
                        mov    ah, 6          ;up
                        mov    al, number    ;lines
                        mov    ch, 0          ;starting at 0,0
                        mov    cl, 0
                        mov    dh, 24        ;ending at 24, 79
                        mov    dl, 79
                        mov    bh, 15          ;black background, white fore
                        int    10h

                        popa

endm

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
clearScreen            macro
                        scrollup 0
endm

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
newLine                macro  number        ;line feed and carriage return
local  Repeat

                        pusha

                        ifnb <number>
                        mov    cx, number
                        endif

Repeat:                mov    al, 0Dh
                        mov    ah, 0Eh
                        int    10h
                        mov    al, 0Ah  
                        mov    ah, 0Eh
                        int    10h
  
                        ifnb    <number>
                            loop    Repeat
                        endif

                        popa
      
endm

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
getdate                macro  near            ;get date
                        
                        mov    ah, 2Ah
                        Int    21h    
                        ;AL = day of week (0-7)
                        ;CX = year (1980-2099)
                        ;DL = month (1-12)
                        ;DH = day (1-31)
                        

endm
;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
gettime                macro  near            ;get time

                        mov    ah, 2Ch
                        Int    21h
                        ;AL = hour (0-23)
                        ;CX = minute (0-59)
                        ;DL = second (0-59)
                        ;DH = hundredth (0-99)

endm

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
setCurPos                macro  row, column    ;sets cursor position        

                        mov    dl, row
                        mov    dh, column
                        mov    ah, 2
                      int    10h

endm

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

beep                    macro                    ;beeps speaker

                        pusha

                        mov    dl, 7
                        mov    ah, 2
                        int    21h

                        popa

endm

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

STRCPY MACRO
LOCAL @@OK
	STRLEN
	CMP BX, 31
	JLE @@OK
	MOV BX, 31
@@OK:
	MOV CX, BX
	CLD
	REP MOVSB
	MOV AL, '$'
	MOV BYTE PTR[DI], AL
ENDM

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ESTACONTENIDO MACRO
LOCAL @@CICLO, @@SUBCICLO, @@DISTINTO, @@FIN, @@TERMINA
    ; inicializo bx
    mov bx, 0

@@CICLO:
    ; verifico que ambas cadenas no terminaron
    cmp byte ptr [si], '$'
    je @@FIN
    cmp byte ptr [di], '$'
    je @@FIN

    push si
    push di

    ; Compara los caracteres actuales de las cadenas
    mov al, [si]
    cmp al, [di]
    jne @@DISTINTO

    ; si son iguales, inicia un subciclo para comparar el resto de la cadena
@@SUBCICLO:
    inc si
    inc di
    cmp byte ptr [si], '$'
    je @@FIN
    cmp byte ptr [di], '$'
    je @@FIN
    mov al, [si]
    cmp al, [di]
    jne @@DISTINTO
    jmp @@SUBCICLO

@@DISTINTO:
    ; Restaura los índices
    pop di
    pop si

    ; Avanza a los siguientes caracteres
    inc si
    jmp @@CICLO

@@FIN:
    cmp byte ptr [di], '$'
    jne @@TERMINA
    mov bx, 1
@@TERMINA:

ENDM


;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;en bx guardo cantidad

CONCATENARCONRECORTE MACRO
LOCAL @@CONCAT, @@CICLO, @@FINERROR, @@TERMINAR

mov cx, bx
push si
push di
cmp bx, 0
JNAE @@FINERROR

cmp bx, 0
JE @@CONCAT

@@CICLO:
	inc si
	cmp byte ptr [si], '$'
	je @@FINERROR           
	inc di
	cmp byte ptr [di], '$'
	je @@FINERROR

	dec bx
    
	cmp bx, 1
	JNB @@CICLO
    JMP @@CONCAT
@@FINERROR:
    displayString _errorConCat
    newline 1
    JMP @@TERMINAR
@@CONCAT:


    strcat
  
    STRCPY
    
  
@@TERMINAR:

ENDM

;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

strcatMIO MACRO
LOCAL @@LOOP, @@END
   
  

@@LOOP:
    lodsb           ; Load string byte at address DS:SI into AL
    or al, al       ; Check if AL is zero (end of string)
    jz @@END        ; If it is, jump to @@END
    stosb           ; Store AL at address ES:DI and increment DI
    jmp @@LOOP      ; Repeat the process

@@END:
    stosb           ; Store the null terminator at the end of the destination string
ENDM


;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
STRNCPY MACRO
LOCAL @@OK, @@LOOP, @@END
    PUSH SI
    PUSH DI
    MOV CX, BX  ; Utiliza el valor en BX como la cuenta
    CLD
@@LOOP:
    LODSB
    OR AL, AL
    JZ @@END
    STOSB
    LOOP @@LOOP
@@END:
    MOV AL, '$'
    STOSB
    POP DI
    POP SI
ENDM





;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

STRLEN MACRO
LOCAL @@STRL01, @@STREND
	;DEJA EN BX LA CANTIDAD DE CARACTERES DE UNA CADENA
	MOV BX, 0
@@STRL01:
	CMP BYTE PTR[SI + BX], '$'
	JE @@STREND
	INC BX
	JMP @@STRL01
@@STREND:
	NOP
ENDM  
  
STRCAT MACRO
LOCAL @@CONCATSIZEMAL, @@CONCATSIZEOK, @@CONCATSIGO
	PUSH DS
	PUSH SI
	STRLEN
	MOV DX, BX
	MOV SI, DI
	PUSH ES
	POP DS
	STRLEN
	ADD DI, BX
	ADD BX, DX
	CMP BX, 31
	JG @@CONCATSIZEMAL
@@CONCATSIZEOK:
	MOV CX, DX
	JMP @@CONCATSIGO
@@CONCATSIZEMAL:
	SUB BX, 31
	SUB DX, BX
	MOV CX, DX
@@CONCATSIGO:
	PUSH DS
	POP ES
	POP SI
	POP DS
	CLD
	REP MOVSB
	MOV AL,'$'
	MOV BYTE PTR [DI], AL
ENDM
 
STRCMP MACRO
LOCAL @@CICLO, @@NOTEQUAL, @@BYE
    DEC DI

@@CICLO:
    INC DI 			;DS:DI -> SIGUIENTE CHAR EN CAD2
    LODSB 			;CARGA AL CON EL SIGUIENTE CHAR DE CAD1
    CMP [DI], AL 	;COMPARA CHARS
    JNE @@NOTEQUAL 	;SALTA DEL LOOP SI NO SON LOS MISMOS
    CMP AL, '$' 	;SON LOS MISMOS, VERIFICA EOF
    JNE @@CICLO 		;NO ES EOF, PASA A LOS SIGUIENTES

    MOV BL, 0
	TEST BL, BL
    JMP @@BYE 		;LOS STRING SON IGUALES (ZF = 1)
@@NOTEQUAL:
    MOV BL, 1		;LOS STRING NO SON IGUALES (ZF = 0)
	TEST BL, BL
@@BYE:
	NOP
ENDM 
  
