
#ifndef CELDAS_H
#define CELDAS_H
#include <stdio.h>
#include <stdarg.h>
enum tipos { Int, Long, Double, Char };

// Operaciones con celdas
typedef struct Celda {
    union {
        int i;
        long l;
        double d;
    } tipo;
    struct Celda* sig;
} Celda;

Celda* CrearCelda(int tipo, va_list ap) {
    Celda* celda = (Celda*)malloc(sizeof(Celda));
    if (!celda) return NULL;
    switch (tipo) {
        case Int:
        case Char:
            celda->tipo.i = va_arg(ap, int);
            break;
        case Long:
            celda->tipo.l = va_arg(ap, long);
            break;
        case Double:
            celda->tipo.d = va_arg(ap, double);
            break;
        default:
            free(celda);
            return NULL;
            break;
    }
    celda->sig = NULL;
    return celda;
}

void DestruirCelda(Celda* celda) {
    if (!celda) return NULL;
    celda->tipo.l = 0;
    celda->sig = 0;
    free(celda);
}

void MostrarCelda(Celda* celda, int tipo) {
    if (!celda) return;
    switch (tipo) {
        case Int:
            printf("%d", celda->tipo.i);
            break;
        case Long:
            printf("%l", celda->tipo.l);
            break;
        case Double:
            printf("%f", celda->tipo.d);
            break;
        case Char:
            printf("%c", celda->tipo.i);
            break;
    }
}

#endif