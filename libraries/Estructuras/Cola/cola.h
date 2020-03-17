/*
Autor: Luis Jose Llamas Perez
Fecha (dd/mm/yyyy): 05/09/2018
Descripcion: Ejercicio 5
*/

#ifndef COLA_H
#define COLA_H
#include <stdarg.h>
#include <stdio.h>
#include "../Celdas/celdas.h"

typedef struct Cola {
    Celda* inicio;
    Celda* final;
    int tipo;
} Cola;

// Operaciones con colas
Cola* CrearCola(int tipo);
void DestruirCola(Cola* cola);
void Encolar(Cola* cola, ...);
void Desencolar(Cola* cola);
Celda* Frente(Cola* cola);
void VaciarCola(Cola* cola);
int Es_vacia_Cola(Cola* cola);
void MostrarCola(Cola* cola);

Cola* CrearCola(int tipo) {
    Cola* cola = (Cola*)malloc(sizeof(Cola));
    if (!cola) return NULL;
    cola->inicio = NULL;
    cola->final = NULL;
    cola->tipo = tipo;
    return cola;
}

void DestruirCola(Cola* cola) {
    VaciarCola(cola);
    free(cola);
}

void Encolar(Cola* cola, ...) {
    va_list ap;
    va_start(ap, cola);
    Celda* celda = CrearCelda(cola->tipo, ap);
    va_end(ap);
    if (!celda) return;
    if (Es_vacia_Cola(cola)) {
        cola->inicio = celda;
        cola->final = celda;
        return;
    }
    cola->final->sig = celda;
    cola->final = celda;
}

void MostrarCola(Cola* cola) {
    if (Es_vacia_Cola(cola)) return;
    Celda* aux = cola->inicio;
    printf("\n");
    while (aux) {
        MostrarCelda(aux, cola->tipo);
        printf(" - ");
        aux = aux->sig;
    }
    printf("\n");
}

void Desencolar(Cola* cola) {
    if (Es_vacia_Cola(cola)) return;
    Celda* celda_a_eliminar = cola->inicio;
    cola->inicio = cola->inicio->sig;
    DestruirCelda(celda_a_eliminar);
}

Celda* Frente(Cola* cola) {
    if (Es_vacia_Cola(cola)) return NULL;
    return cola->inicio;
}

void VaciarCola(Cola* cola) {
    while (!Es_vacia_Cola(cola)) Desencolar(cola);
}

int Es_vacia_Cola(Cola* cola) {
    if (!cola->inicio) return 1;
    return 0;
}

#endif