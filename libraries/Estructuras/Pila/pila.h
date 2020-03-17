/*
Autor: Luis Jose Llamas Perez
Fecha (dd/mm/yyyy): 05/09/2018
Descripcion: Ejercicio 5
*/

// Cabezera
// Librerias
#include <stdarg.h>
#include <stdio.h>
#include "../Celdas/celdas.h"

typedef struct Pila {
    Celda* cima;
    int tamano;
    int tipo;
} Pila;

// Declaracion de procedimientos y funciones

// Operaciones con pilas
Pila* CrearPila(int tipo);
void DestruirPila(Pila* pila);
void Apilar(Pila* pila, ...);
void Desapilar(Pila* pila);
Celda* Cima(Pila* pila);
int Es_vacia_pila(Pila* pila);

Pila* CrearPila(int tipo) {
    Pila* pila = (Pila*)malloc(sizeof(Pila));
    if (!pila) return NULL;
    pila->cima = NULL;
    pila->tamano = 0;
    pila->tipo = tipo;
    return pila;
}

void DestruirPila(Pila* pila) {
    while (!(Es_vacia_pila(pila))) Desapilar(pila);
    free(pila);
}

void Apilar(Pila* pila, ...) {
    va_list ap;
    va_start(ap, pila);
    Celda* celda = CrearCelda(pila->tipo, ap);
    va_end(ap);
    if (!celda) return;
    celda->sig = pila->cima;
    pila->cima = celda;
    pila->tamano++;
}

void Desapilar(Pila* pila) {
    if (Es_vacia_pila(pila)) return;
    Celda* celda_a_eliminar = pila->cima;
    pila->cima = celda_a_eliminar->sig;
    pila->tamano--;
    DestruirCelda(celda_a_eliminar);
}

void MostrarPila(Pila* pila) {
    if (Es_vacia_pila(pila)) return;
    Celda* aux = pila->cima;
    printf("\n");
    while (aux) {
        MostrarCelda(aux, pila->tipo);
        printf(" - ");
        aux = aux->sig;
    }
    printf("\n");
}

Celda* Cima(Pila* pila) {
    if (Es_vacia_pila(pila)) return NULL;
    return pila->cima;
}

int Es_vacia_pila(Pila* pila) {
    if (!(pila->cima)) return 1;
    return 0;
}