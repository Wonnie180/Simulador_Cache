#include <stdio.h>
#include <stdlib.h>
#include "Booleanos.h"

#ifndef BOOLEANO_MATRIX_BIT_H
#define BOOLEANO_MATRIX_BIT_H

char** crearBool_Matrix(long n);
char get_bit_m(char** Booleanos, long fila, long columna);
void set_bit_m(char** Booleanos, long fila, long columna, char b);
void toggle_bit_m(char** Booleanos, long fila, long columna);

char** crearBool_Matrix(long n) {
    char** Booleanos;
    long aux = n / 8;
    if (n % 8 != 0) aux++;
    Booleanos = (char**)calloc(aux, sizeof(char*));
    for (long i = 0; i < aux; i++) {
        Booleanos[i] = (char*)calloc(aux, sizeof(char));
    }
    return Booleanos;
}

char get_bit_m(char** Booleanos, long fila, long columna) {
    return get_bit(Booleanos[fila / 8], columna);
}

void set_bit_m(char** Booleanos, long fila, long columna, char b) {
    if (fila == columna) return;  // No permitir valores en la diagonal
    set_bit(Booleanos[fila / 8], columna, b);
}

void toggle_bit_m(char** Booleanos, long fila, long columna) {
    if (fila == columna) return;  // No permitir valores en la diagonal
    set_bit_m(Booleanos, fila, columna, get_bit_m(Booleanos, fila, columna));
}

// IMPLEMENTAR EN BASE AL PROBLEMA
#endif