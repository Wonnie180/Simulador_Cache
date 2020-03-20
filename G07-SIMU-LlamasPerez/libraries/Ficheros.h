#include <stdio.h>
#include <stdlib.h>

#ifndef FICHEROS_H
#define FICHEROS_H

#define MAX_BUFFER_Fichero 100000
#define F_R 1
#define F_W 2

typedef struct Fichero {
    FILE* f;
    char Buffer[MAX_BUFFER_Fichero];
    char modo;
    int max_buffer;
    int index;
} Fichero;

Fichero* crearFichero();
void inicializarFicheroLectura(Fichero* f, char* Direccion);
void inicializarFicheroEscritura(Fichero* f, char* Direccion);
void cerrarFichero(Fichero* f);
int leerIntFichero(Fichero* f);
long leerLongFichero(Fichero* f);
long long leerLongLongFichero(Fichero* f);
void escribirIntFichero(Fichero* f, int i);
void escribirLongFichero(Fichero* f, long i);
void escribirLongLongFichero(Fichero* f, long long i);
void escribirCharFichero(Fichero* f, char c);
void escribirStringFichero(Fichero* f, char* c);

Fichero* crearFichero() {
    Fichero* f = (Fichero*)malloc(sizeof(Fichero));
    if (!f) return NULL;
    f->f = NULL;
    f->Buffer[0] = '\0';
    f->max_buffer = MAX_BUFFER_Fichero;
    f->index = 0;
    f->modo = 0;
    return f;
}

void inicializarFicheroLectura(Fichero* f, char* Direccion) {
    if (!f || f->modo != 0) return;
    f->f = fopen(Direccion, "r");
    f->index = 0;
    f->Buffer[0] = '\0';
    f->modo = 1;
}

void inicializarFicheroEscritura(Fichero* f, char* Direccion) {
    if (!f || f->modo != 0) return;
    f->f = fopen(Direccion, "w");
    f->index = 0;
    f->Buffer[0] = '\0';
    f->modo = 2;
}

void cerrarFichero(Fichero* f) {
    if (!f) return;
    if (f->f) fclose(f->f);
    f->f = NULL;
    f->index = 0;
    f->Buffer[0] = '\0';
    f->max_buffer = 0;
    f->modo = 0;
}

int leerIntFichero(Fichero* f) {
    if (!(f && f->f && f->modo == F_R)) return -1;
    int i;
    if (fscanf(f->f, "%*s %d\n", &i) != EOF) return i;
    return -1;
}

long leerHexLongFichero(Fichero* f) {
    if (!(f && f->f && f->modo == F_R)) return -1;
    long i;
    if (fscanf(f->f, "%lx", &i) != EOF) return i;
    return -1;
}

long leerLongFichero(Fichero* f) {
    if (!(f && f->f && f->modo == F_R)) return -1;
    long i;
    if (fscanf(f->f, "%li", &i) != EOF) return i;
    return -1;
}

long long leerLongLongFichero(Fichero* f) {
    if (!(f && f->f && f->modo == F_R)) return -1;
    long long i;
    if (fscanf(f->f, "%lli", &i) != EOF) return i;
    return -1;
}

void escribirIntFichero(Fichero* f, int i) {
    if (!(f && f->f && f->modo == F_W)) return;
    fprintf(f->f, "%d", i);
}

void escribirLongFichero(Fichero* f, long i) {
    if (!(f && f->f && f->modo == F_W)) return;
    fprintf(f->f, "%li", i);
}

void escribirLongLongFichero(Fichero* f, long long i) {
    if (!(f && f->f && f->modo == F_W)) return;
    fprintf(f->f, "%lli", i);
}

void escribirCharFichero(Fichero* f, char c) {
    if (!(f && f->f && f->modo == F_W)) return;
    fprintf(f->f, "%c", c);
}

void escribirStringFichero(Fichero* f, char* c) {
    if (!(f && f->f && f->modo == F_W)) return;
    for (int i = 0; c[i] != '\0'; i++) fprintf(f->f, "%c", c[i]);
}

// IMPLEMENTAR EN BASE AL PROBLEMA
#endif