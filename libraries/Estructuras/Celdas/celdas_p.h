#ifndef CELDAS_P_H
#define CELDAS_P_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum tipos { Int, Long, Double, Char };

// Operaciones con celdas
typedef struct Celda_P {
    union {
        int i;
        long l;
        double d;
    } tipo;
    int prioridad;
    struct Celda_P* sig;
} Celda_P;

void MostrarCelda_P(Celda_P* celda, int tipo) {
    if (!celda) return;
    printf("(E:");
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
    printf(" - P:%d)", celda->prioridad);
}

Celda_P* CrearCelda_P(int tipo, va_list ap) {
    Celda_P* celda = (Celda_P*)malloc(sizeof(Celda_P));
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
    celda->prioridad = va_arg(ap, int);
    celda->sig = NULL;
    return celda;
}

void DestruirCelda_P(Celda_P* celda) {
    if (!celda) return NULL;
    celda->tipo.l = 0;
    celda->prioridad = 0;
    celda->sig = 0;
    free(celda);
}
#endif