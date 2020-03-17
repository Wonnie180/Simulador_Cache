/*
Autor: Luis Jose Llamas Perez
Fecha (dd/mm/yyyy): 05/09/2018
Descripcion: Ejercicio 5
*/

// Cabezera
// Librerias
#include <stdarg.h>
#include <stdio.h>
#include "../celdas/celdas_p.h"
// Constantes

// Tipos

typedef struct Cola_P {
    Celda_P* inicio;
    Celda_P* final;
    int tipo;
} Cola_P;

// Declaracion de procedimientos y funciones

// Operaciones con celdas

// Operaciones con colas
Cola_P* CrearCola_P();
void DestruirCola_P(Cola_P* cola);
void Encolar_P(Cola_P* cola, ...);
void Desencolar_P(Cola_P* cola);
void MostrarCola_P(Cola_P* cola);
Celda_P* FrenteCola_P(Cola_P* cola);
void AvanzarCola_P(Cola_P* cola);
void VaciarCola_P(Cola_P* cola);
int Es_vacia_Cola_P(Cola_P* cola);

Cola_P* CrearCola_P(int tipo) {
    Cola_P* cola = (Cola_P*)malloc(sizeof(Cola_P));
    if (!cola) return NULL;
    cola->inicio = NULL;
    cola->final = NULL;
    cola->tipo = tipo;
    return cola;
}

void DestruirCola_P(Cola_P* cola) {
    if (!Es_vacia_Cola_P(cola)) VaciarCola_P(cola);
    free(cola);
}

void Encolar_P(Cola_P* cola, ...) {
    va_list args;
    va_start(args, cola);
    Celda_P* celda = CrearCelda_P(cola->tipo, args);
    va_end(args);
    if (!celda) return;
    if (Es_vacia_Cola_P(cola)) {
        cola->inicio = celda;
        cola->final = celda;
        return;
    }
    if (cola->inicio->prioridad < celda->prioridad) {
        celda->sig = cola->inicio;
        cola->inicio = celda;
        return;
    } else if (cola->final->prioridad >= celda->prioridad) {
        cola->final->sig = celda;
        cola->final = celda;
        return;
    }
    Celda_P *aux_a = cola->inicio, *aux_s = cola->inicio->sig;
    while ((aux_s != cola->final) && (aux_s->prioridad >= celda->prioridad)) {
        aux_a = aux_s;
        aux_s = aux_s->sig;
    }
    aux_a->sig = celda;
    celda->sig = aux_s;
}

void MostrarCola_P(Cola_P* cola) {
    Celda_P* aux = cola->inicio;
    if (!aux) return;
    printf("\n");
    while (aux != cola->final) {
        MostrarCelda_P(aux, cola->tipo);
        printf(" - ");
        aux = aux->sig;
    }
    MostrarCelda_P(aux, cola->tipo);
}

void AvanzarCola_P(Cola_P* cola) {
    if (Es_vacia_Cola_P(cola)) return;
    printf("\nDesencolando: ");
    MostrarCelda_P(FrenteCola_P(cola), cola->tipo);
    printf("\n\n");
    Desencolar_P(cola);
}

Celda_P* FrenteCola_P(Cola_P* cola) {
    if (Es_vacia_Cola_P(cola)) return NULL;
    return cola->inicio;
}

void Desencolar_P(Cola_P* cola) {
    if (Es_vacia_Cola_P(cola)) return;
    Celda_P* celda_a_eliminar = cola->inicio;
    cola->inicio = cola->inicio->sig;
    DestruirCelda_P(celda_a_eliminar);
    if (Es_vacia_Cola_P(cola)) cola->final = NULL;
}

void VaciarCola_P(Cola_P* cola) {
    while (!(Es_vacia_Cola_P(cola))) AvanzarCola_P(cola);
}

int Es_vacia_Cola_P(Cola_P* cola) {
    if (!cola->inicio) return 1;
    return 0;
}