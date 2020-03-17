#ifndef CELDAS_A_H
#define CELDAS_A_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum tipos { Int, Long, Double, Char };

// Operaciones con celdas
typedef struct Celda_A {
    union {
        int i;
        long l;
        double d;
    } tipo;
    struct Celda_A* izq;
    struct Celda_A* der;
} Celda_A;

Celda_A* CrearCelda_A(int tipo, va_list ap) {
    Celda_A* celda = (Celda_A*)malloc(sizeof(Celda_A));
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
            celda = NULL;
            return celda;
            break;
    }
    celda->izq = NULL;
    celda->der = NULL;
    return celda;
}

void DestruirCelda_A(Celda_A* celda) {
    if (!celda) return;
    celda->tipo.l = 0;
    celda->izq = NULL;
    celda->der = NULL;
    free(celda);
    celda = NULL;
}

void MostrarCelda_P(Celda_A* celda, int tipo) {
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