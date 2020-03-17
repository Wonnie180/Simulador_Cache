#ifndef CELDAS_D_H
#define CELDAS_D_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum tipos { Int, Long, Double, Char };

// Operaciones con celdas
typedef struct Celda_D {
    union {
        int i;
        long l;
        double d;
    } tipo;
    struct Celda_D* sig;
    struct Celda_D* ant;
} Celda_D;

Celda_D* CrearCelda_D(int tipo, va_list ap) {
    Celda_D* celda = (Celda_D*)malloc(sizeof(Celda_D));
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
    celda->ant = NULL;
    return celda;
}

void DestruirCelda_D(Celda_D* celda) {
    if (!celda) return;
    celda->tipo.l = 0;
    celda->sig = 0;
    celda->ant = 0;
    free(celda);
}

void MostrarCelda_P(Celda_D* celda, int tipo) {
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