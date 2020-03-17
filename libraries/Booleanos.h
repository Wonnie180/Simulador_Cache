#include <stdio.h>
#include <stdlib.h>

#ifndef BOOLEANO_BIT_H
#define BOOLEANO_BIT_H

char* crearBool(long n);
char get_bit(char* Booleanos, long posicion);
void set_bit(char* Booleanos, long posicion, char b);
char toggle_bit(char* Booleanos, long posicion);

char* crearBool(long n) {
    char* Booleanos;
    long aux = n / 8;
    if (n % 8 != 0) aux++;
    Booleanos = (char*)calloc(aux, sizeof(char));
    return Booleanos;
}

char get_bit(char* Booleanos, long posicion) {
    return (Booleanos[posicion / 8] & (1 << (posicion % 8)))
               ? 1
               : 0;  // obtener el bit
}

void set_bit(char* Booleanos, long posicion, char b) {
    if (b == get_bit(Booleanos, posicion)) return;
    Booleanos[posicion / 8] ^= 1
                               << (posicion % 8);  // XOR bit posicion Tipo % 8
}

char toggle_bit(char* Booleanos, long posicion) {
    set_bit(Booleanos, posicion, get_bit(Booleanos, posicion));
}

// IMPLEMENTAR EN BASE AL PROBLEMA
#endif