/*
Autor: Luis Jose Llamas Perez
Fecha (dd/mm/yyyy): 05/09/2018
Descripcion: Ejercicio 5
*/

// Cabezera
// Librerias
#include <stdarg.h>
#include <stdio.h>
#include "../Celdas/celdas_d.h"

// Constantes

// Tipos

typedef struct Lista_D_Ord {
    Celda_D* inicio;
    Celda_D* final;
    int tamano;
    int tipo;
} Lista_D_Ord;

// Declaracion de procedimientos y funciones

// Operaciones con colas
Lista_D_Ord* CrearLista(int tipo);
void DestruirLista(Lista_D_Ord* lista);
void Enlistar(Lista_D_Ord* lista, ...);
void Desenlistar(Lista_D_Ord* lista, ...);
Celda_D* Buscar_en_lista_doblelig(Lista_D_Ord* lista, ...);
Celda_D* Donde_meter_en_lista_doblelig(Lista_D_Ord* lista, ...);
void VaciarLista(Lista_D_Ord* lista);
int Es_vacia_Lista(Lista_D_Ord* lista);
void MostrarListaAsc(Lista_D_Ord* lista);
void MostrarListaDesc(Lista_D_Ord* lista);

// FUNCIONES

Lista_D_Ord* CrearLista(int tipo) {
    Lista_D_Ord* lista = (Lista_D_Ord*)malloc(sizeof(Lista_D_Ord));
    if (!lista) return NULL;
    lista->inicio = NULL;
    lista->final = NULL;
    lista->tamano = 0;
    lista->tipo = tipo;
    return lista;
}

void DestruirLista(Lista_D_Ord* lista) {
    if (!Es_vacia_Lista(lista)) VaciarLista(lista);
    free(lista);
}

void Enlistar(Lista_D_Ord* lista, ...) {
    va_list ap;
    va_start(ap, lista);
    Celda_D* celda = CrearCelda_D(lista->tipo, ap);
    va_end(ap);
    if (!celda) return;
    lista->tamano++;
    if (Es_vacia_Lista(lista)) {
        lista->inicio = celda;
        lista->final = celda;
        return;
    }
    int resultado = -1;
    Celda_D* aux;
    switch (lista->tipo) {
        case Int:
        case Char:
            if (lista->inicio->tipo.i < celda->tipo.i) {
                resultado = 0;
                break;
            }
            if (lista->final->tipo.i >= celda->tipo.i) {
                resultado = 1;
                break;
            }
            aux = Donde_meter_en_lista_doblelig(lista, celda->tipo.i);
            while (aux && aux->tipo.i == celda->tipo.i) aux = aux->sig;
            break;
        case Long:
            if (lista->inicio->tipo.l < celda->tipo.l) {
                resultado = 0;
                break;
            }
            if (lista->final->tipo.l >= celda->tipo.l) {
                resultado = 1;
                break;
            }
            aux = Donde_meter_en_lista_doblelig(lista, celda->tipo.l);
            while (aux != NULL && aux->tipo.l == celda->tipo.l) aux = aux->sig;
            break;
        case Double:
            if (lista->inicio->tipo.d < celda->tipo.d) {
                resultado = 0;
                break;
            }
            if (lista->final->tipo.d >= celda->tipo.d) {
                resultado = 1;
                break;
            }
            aux = Donde_meter_en_lista_doblelig(lista, celda->tipo.d);
            while (aux != NULL && aux->tipo.d == celda->tipo.d) aux = aux->sig;
            break;
    }
    // Si es mayor que el primero
    if (resultado == 0) {
        celda->sig = lista->inicio;
        lista->inicio->ant = celda;
        lista->inicio = celda;
        return;
        // Si es menor o igual que el final
    } else if (resultado == 1) {
        celda->ant = lista->final;
        lista->final->sig = celda;
        lista->final = celda;
        return;
    } else if (aux) {
        celda->sig = aux;
        celda->ant = aux->ant;
        aux->ant = celda;
        celda->ant->sig = celda;
    } else {
        free(celda);
    }
}

void Desenlistar(Lista_D_Ord* lista, ...) {
    if (Es_vacia_Lista(lista)) return;
    va_list ap;
    va_start(ap, lista);
    Celda_D* celda;
    switch (lista->tipo) {
        case Int:
        case Char:
            celda = Buscar_en_lista_doblelig(lista, va_arg(ap, int));
            break;
        case Long:
            celda = Buscar_en_lista_doblelig(lista, va_arg(ap, long));
            break;
        case Double:
            celda = Buscar_en_lista_doblelig(lista, va_arg(ap, double));
            break;
    }
    va_end(ap);
    if (!celda) return;
    if (!celda->ant && !celda->sig) {
        lista->inicio = NULL;
        lista->final = NULL;
    } else if (!celda->ant) {
        lista->inicio = celda->sig;
        lista->inicio->ant = NULL;
    } else if (!celda->sig) {
        lista->final = celda->ant;
        lista->final->sig = NULL;
    } else {
        celda->ant->sig = celda->sig;
        celda->sig->ant = celda->ant;
    }
    lista->tamano--;
    DestruirCelda_D(celda);
}

void MostrarListaDesc(Lista_D_Ord* lista) {
    if (Es_vacia_Lista(lista)) return;
    Celda_D* aux = lista->inicio;
    printf("\nNumero de elementos: %d\n", lista->tamano);
    while (aux != lista->final) {
        printf("%d - ", aux->tipo.i);
        aux = aux->sig;
    }
    printf("%d\n", aux->tipo.i);
}

void MostrarListaAsc(Lista_D_Ord* lista) {
    if (Es_vacia_Lista(lista)) return;
    Celda_D* aux = lista->final;
    printf("\nNumero de elementos: %d\n", lista->tamano);
    while (aux != lista->inicio) {
        printf("%d - ", aux->tipo.i);
        aux = aux->ant;
    }
    printf("%d\n", aux->tipo.i);
}

Celda_D* Buscar_en_lista_doblelig(Lista_D_Ord* lista, ...) {
    if (Es_vacia_Lista(lista)) return NULL;
    va_list ap;
    va_start(ap, lista);
    Celda_D *aux_s = lista->inicio, *aux_a = lista->final;
    switch (lista->tipo) {
        case Int:
        case Char:;
            int e_i = va_arg(ap, int);
            va_end(ap);
            while (aux_s->tipo.i > e_i && aux_a->tipo.i < e_i &&
                   aux_s->tipo.i != aux_a->tipo.i) {
                aux_s = aux_s->sig;
                aux_a = aux_a->ant;
            }
            if (aux_s->tipo.i == e_i) return aux_s;
            if (aux_a->tipo.i == e_i) return aux_a;
            break;
        case Long:;
            long e_l = va_arg(ap, long);
            va_end(ap);
            while (aux_s->tipo.l > e_l && aux_a->tipo.l < e_l &&
                   aux_s->tipo.l != aux_a->tipo.l) {
                aux_s = aux_s->sig;
                aux_a = aux_a->ant;
            }
            if (aux_s->tipo.l == e_l) return aux_s;
            if (aux_a->tipo.l == e_l) return aux_a;
            break;
        case Double:;
            double e_d = va_arg(ap, double);
            va_end(ap);
            while (aux_s->tipo.d > e_d && aux_a->tipo.d < e_d &&
                   aux_s->tipo.d != aux_a->tipo.d) {
                aux_s = aux_s->sig;
                aux_a = aux_a->ant;
            }
            if (aux_s->tipo.d == e_d) return aux_s;
            if (aux_a->tipo.d == e_d) return aux_a;
            break;
    }
    return NULL;
}

Celda_D* Donde_meter_en_lista_doblelig(Lista_D_Ord* lista, ...) {
    if (Es_vacia_Lista(lista)) return NULL;
    va_list ap;
    va_start(ap, lista);
    Celda_D *aux_s = lista->inicio, *aux_a = lista->final;
    switch (lista->tipo) {
        case Int:
        case Char:;
            int e_i = va_arg(ap, int);
            va_end(ap);
            while (aux_s->tipo.i > e_i && aux_a->tipo.i < e_i &&
                   aux_s->tipo.i != aux_a->tipo.i) {
                aux_s = aux_s->sig;
                aux_a = aux_a->ant;
            }
            if (aux_s->tipo.i <= e_i) return aux_s;
            if (aux_a->tipo.i >= e_i) return aux_a;
            break;
        case Long:;
            long e_l = va_arg(ap, long);
            va_end(ap);
            while (aux_s->tipo.l > e_l && aux_a->tipo.l < e_l &&
                   aux_s->tipo.l != aux_a->tipo.l) {
                aux_s = aux_s->sig;
                aux_a = aux_a->ant;
            }
            if (aux_s->tipo.l <= e_l) return aux_s;
            if (aux_a->tipo.l >= e_l) return aux_a;
            break;
        case Double:;
            double e_d = va_arg(ap, double);
            va_end(ap);
            while (aux_s->tipo.d > e_d && aux_a->tipo.d < e_d &&
                   aux_s->tipo.d != aux_a->tipo.d) {
                aux_s = aux_s->sig;
                aux_a = aux_a->ant;
            }
            if (aux_s->tipo.d <= e_d) return aux_s;
            if (aux_a->tipo.d >= e_d) return aux_a;
            break;
    }
    return NULL;
}

void VaciarLista(Lista_D_Ord* lista) {
    while (!(Es_vacia_Lista(lista))) {
        switch (lista->tipo) {
            case Int:
            case Char:
                Desenlistar(lista, lista->inicio->tipo.i);
                break;
            case Long:
                Desenlistar(lista, lista->inicio->tipo.l);
                break;
            case Double:
                Desenlistar(lista, lista->inicio->tipo.d);
                break;
        }
    }
}

int Es_vacia_Lista(Lista_D_Ord* lista) {
    if (!lista->inicio) return 1;
    return 0;
}