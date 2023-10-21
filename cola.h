#include <stddef.h>

typedef struct SNodo
{
    void* dato;
    size_t tamElem;
    struct SNodo* sig;
}NodoC;


typedef struct
{
    NodoC* frente;
    NodoC* fondo;
}Cola;

void crearCola(Cola* pc);
int encolar(Cola* pc, const void* dato, size_t tamElem);
int desencolar(Cola* pc, void* dato, size_t tamElem);
int verFrenteDeCola(const Cola* pc, void* dato, size_t tamElem);
int colaVacia(const Cola* pc);
int colaLlena(const Cola* pc, size_t tamElem);
void vaciarCola(Cola* pc);