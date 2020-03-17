/*
Autor: Luis Jose Llamas Perez
Fecha (dd/mm/yyyy): 05/09/2018
Descripcion: Ejercicio 5
*/

// Cabezera
// Librerias
#ifndef ARBOL_AST_H
#define ARBOL_AST_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Celdas/celdas_a.h"
// Constantes

typedef struct Arbol_AST {
    Celda_A* raiz;
    int tipo;
} Arbol_AST;

// Operaciones con Arboles binarios
Arbol_AST* CrearArbol_AST(int tipo);
void DestruirArbol_AST(Arbol_AST** arbol);
void _DestruirArbol_AST(Celda_A** raiz);
void VaciarArbol_AST(Arbol_AST* arbol);

void Insertar_en_Arbol_AST(Arbol_AST* arbol, ...);
void _Insertar_en_arbol_int_AST(Celda_A** raiz, Celda_A* celda);
void _Insertar_en_arbol_long_AST(Celda_A** raiz, Celda_A* celda);
void _Insertar_en_arbol_double_AST(Celda_A** raiz, Celda_A* celda);

int Existe_en_Arbol_AST(Arbol_AST* arbol, ...);
int _Existe_en_Arbol_int_AST(Celda_A* raiz, int e);
int _Existe_en_Arbol_long_AST(Celda_A* raiz, long e);
int _Existe_en_Arbol_double_AST(Celda_A* raiz, double e);

int Es_vacio_Arbol_AST(Arbol_AST* arbol);
int _Es_vacio_Arbol_AST(Celda_A* raiz);

// Representan el arbol en diferentes formatos
void Preorden(Arbol_AST* arbol);
void _Preorden_int(Celda_A* raiz);
void _Preorden_long(Celda_A* raiz);
void _Preorden_double(Celda_A* raiz);

void Inorden(Arbol_AST* arbol);
void _Inorden_int(Celda_A* raiz);
void _Inorden_long(Celda_A* raiz);
void _Inorden_double(Celda_A* raiz);

void Postorden(Arbol_AST* arbol);
void _Postorden_int(Celda_A* raiz);
void _Postorden_long(Celda_A* raiz);
void _Postorden_double(Celda_A* raiz);

int AlturaArbol(Arbol_AST* arbol);
int _AlturaArbol(Celda_A* raiz, int n);

int Altura_MinimaArbol(Arbol_AST* arbol);
int _Altura_MinimaArbol(Celda_A* raiz, int n);

void Eliminar_apartirdenivel_Arbol(Arbol_AST* arbol, int n);
void _Eliminar_apartirdenivel_Arbol(Celda_A* raiz, int n_actual, int n);

void Podar_mismo_nivel_Arbol(Arbol_AST* arbol);

// FUNCIONES
Arbol_AST* CrearArbol(int tipo) {
    Arbol_AST* arbol = (Arbol_AST*)malloc(sizeof(Arbol_AST));
    if (!arbol) return NULL;
    arbol->raiz = NULL;
    arbol->tipo = tipo;
    return arbol;
}

void DestruirArbol(Arbol_AST** arbol) {
    VaciarArbol(*arbol);
    free(*arbol);
    *arbol = NULL;
}

void VaciarArbol(Arbol_AST* arbol) {
    if (Es_vacio_Arbol(arbol)) return;
    _DestruirArbol(&arbol->raiz);
}

void _DestruirArbol(Celda_A** raiz) {
    if (_Es_vacio_Arbol(*raiz)) return;
    if ((*raiz)->der) _DestruirArbol(&(*raiz)->der);
    if ((*raiz)->izq) _DestruirArbol(&(*raiz)->izq);
    DestruirCelda_A(*raiz);
    free(*raiz);
    *raiz = NULL;
}

void Insertar_en_Arbol(Arbol_AST* arbol, ...) {
    va_list args;
    va_start(args, arbol);
    Celda_A* celda = CrearCelda_A(arbol->tipo, args);
    va_end(args);
    if (!celda) return;
    switch (arbol->tipo) {
        case Int:
        case Char:
            _Insertar_en_arbol_int(&arbol->raiz, celda);
            break;
        case Long:
            _Insertar_en_arbol_long(&arbol->raiz, celda);
            break;
        case Double:
            _Insertar_en_arbol_double(&arbol->raiz, celda);
            break;
        default:
            return;
            break;
    }
}

void _Insertar_en_arbol_int(Celda_A** raiz, Celda_A* celda) {
    if (_Es_vacio_Arbol(*raiz)) {
        *raiz = celda;
        return;
    }
    /*if ((*raiz)->tipo.i == celda->tipo.i)
        _DestruirArbol(&celda);
    else */if ((*raiz)->tipo.i > celda->tipo.i)
        _Insertar_en_arbol_int(&(*raiz)->izq, celda);
    else if ((*raiz)->tipo.i <= celda->tipo.i)
        _Insertar_en_arbol_int(&(*raiz)->der, celda);
}

void _Insertar_en_arbol_long(Celda_A** raiz, Celda_A* celda) {
    if (_Es_vacio_Arbol(*raiz)) {
        *raiz = celda;
        return;
    }
    if ((*raiz)->tipo.l == celda->tipo.l)
        _DestruirArbol(&celda);
    else if ((*raiz)->tipo.l > celda->tipo.l)
        _Insertar_en_arbol_long(&(*raiz)->izq, celda);
    else if ((*raiz)->tipo.l < celda->tipo.l)
        _Insertar_en_arbol_long(&(*raiz)->der, celda);
}

void _Insertar_en_arbol_double(Celda_A** raiz, Celda_A* celda) {
    if (_Es_vacio_Arbol(*raiz)) {
        *raiz = celda;
        return;
    }
    if ((*raiz)->tipo.d == celda->tipo.d)
        _DestruirArbol(&celda);
    else if ((*raiz)->tipo.d > celda->tipo.d)
        _Insertar_en_arbol_double(&(*raiz)->izq, celda);
    else if ((*raiz)->tipo.d < celda->tipo.d)
        _Insertar_en_arbol_double(&(*raiz)->der, celda);
}

int Es_vacio_Arbol(Arbol_AST* arbol) {
    if (!arbol->raiz) return 1;
    return 0;
}

int _Es_vacio_Arbol(Celda_A* raiz) {
    if (!raiz) return 1;
    return 0;
}

int Existe_en_Arbol(Arbol_AST* arbol, ...) {
    if (Es_vacio_Arbol(arbol)) return 0;
    va_list args;
    va_start(args, arbol);
    switch (arbol->tipo) {
        case Int:
        case Char:;
            int i = va_arg(args, int);
            va_end(args);
            return _Existe_en_Arbol_int(arbol->raiz, i);
            break;
        case Long:;
            long l = va_arg(args, long);
            va_end(args);
            return _Existe_en_Arbol_long(arbol->raiz, l);
            break;
        case Double:;
            double d = va_arg(args, double);
            va_end(args);
            return _Existe_en_Arbol_double(arbol->raiz, d);
            break;
        default:
            va_end(args);
            return 0;
            break;
    }
}

int _Existe_en_Arbol_int(Celda_A* raiz, int e) {
    if (_Es_vacio_Arbol(raiz))
        return 0;
    else if (raiz->tipo.i == e)
        return 1;
    else if (raiz->tipo.i < e)
        return _Existe_en_Arbol_int(raiz->der, e);
    else if (raiz->tipo.i > e)
        return _Existe_en_Arbol_int(raiz->izq, e);
}

int _Existe_en_Arbol_long(Celda_A* raiz, long e) {
    if (_Es_vacio_Arbol(raiz))
        return 0;
    else if (raiz->tipo.l == e)
        return 1;
    else if (raiz->tipo.l < e)
        return _Existe_en_Arbol_int(raiz->der, e);
    else if (raiz->tipo.l > e)
        return _Existe_en_Arbol_int(raiz->izq, e);
}

int _Existe_en_Arbol_double(Celda_A* raiz, double e) {
    if (_Es_vacio_Arbol(raiz))
        return 0;
    else if (raiz->tipo.d == e)
        return 1;
    else if (raiz->tipo.d < e)
        return _Existe_en_Arbol_int(raiz->der, e);
    else if (raiz->tipo.d > e)
        return _Existe_en_Arbol_int(raiz->izq, e);
}

int AlturaArbol(Arbol_AST* arbol) {
    if (Es_vacio_Arbol(arbol)) return 0;
    int nivel_izq = _AlturaArbol(arbol->raiz->izq, 1),
        nivel_der = _AlturaArbol(arbol->raiz->der, 1);
    if (nivel_izq < nivel_der) return nivel_der;
    return nivel_izq;
}

int _AlturaArbol(Celda_A* raiz, int n) {
    if (_Es_vacio_Arbol(raiz)) return n;
    n++;
    if (!raiz->der && !raiz->izq) return n;
    int nivel_izq = _AlturaArbol(raiz->izq, n),
        nivel_der = _AlturaArbol(raiz->der, n);
    if (nivel_izq < nivel_der) return nivel_der;
    return nivel_izq;
}

int Altura_MinimaArbol(Arbol_AST* arbol) {
    if (Es_vacio_Arbol(arbol)) return 0;
    int nivel_izq = _Altura_MinimaArbol(arbol->raiz->izq, 1),
        nivel_der = _Altura_MinimaArbol(arbol->raiz->der, 1);
    if (nivel_izq > nivel_der) return nivel_der;
    return nivel_izq;
}

int _Altura_MinimaArbol(Celda_A* raiz, int n) {
    if (_Es_vacio_Arbol(raiz)) return n;
    n++;
    if (_Es_vacio_Arbol(raiz->izq) || _Es_vacio_Arbol(raiz->der)) return n;
    int nivel_izq = _AlturaArbol(raiz->izq, n),
        nivel_der = _AlturaArbol(raiz->der, n);
    if (nivel_izq > nivel_der) return nivel_der;
    return nivel_izq;
}

void Podar_mismo_nivel_Arbol(Arbol_AST* arbol) {
    if (Es_vacio_Arbol(arbol)) return;
    Eliminar_apartirdenivel_Arbol(arbol, Altura_MinimaArbol(arbol));
}

void Eliminar_apartirdenivel_Arbol(Arbol_AST* arbol, int nivel) {
    if (Es_vacio_Arbol(arbol)) return;
    if (nivel < 1) {
        VaciarArbol(arbol);
        return;
    }
    _Eliminar_apartirdenivel_Arbol(arbol->raiz, 1, nivel);
}

void _Eliminar_apartirdenivel_Arbol(Celda_A* raiz, int n_actual, int n){
    if (_Es_vacio_Arbol(raiz)) return;
    if (n_actual < n) {
        n_actual++;
        _Eliminar_apartirdenivel_Arbol(raiz->izq, n_actual, n);
        _Eliminar_apartirdenivel_Arbol(raiz->der, n_actual, n);
    } else if (n_actual >= n) {
        if (!(_Es_vacio_Arbol(raiz->izq))) {
            _Eliminar_apartirdenivel_Arbol(raiz->izq, n_actual, n);
            _DestruirArbol(&(raiz)->izq);
        }
        if (!(_Es_vacio_Arbol(raiz->der))) {
            _Eliminar_apartirdenivel_Arbol(raiz->der, n_actual, n);
            _DestruirArbol(&(raiz)->der);
        }
    }
}

void Preorden(Arbol_AST* arbol) {
    if (!arbol->raiz) return;
    switch (arbol->tipo) {
        case Int:
        case Char:
            _Preorden_int(arbol->raiz);
            break;
        case Long:
            _Preorden_long(arbol->raiz);
            break;
        case Double:
            _Preorden_double(arbol->raiz);
            break;
    }
}

void _Preorden_int(Celda_A* raiz) {
    if (!raiz)
        printf("- ");
    else {
        printf("( %d ", raiz->tipo.i);
        _Preorden_int(raiz->izq);
        _Preorden_int(raiz->der);
        printf(")");
    }
}
void _Preorden_double(Celda_A* raiz) {
    if (!raiz)
        printf("- ");
    else {
        printf("( %lf ", raiz->tipo.d);
        _Preorden_double(raiz->izq);
        _Preorden_double(raiz->der);
        printf(")");
    }
}
void _Preorden_long(Celda_A* raiz) {
    if (!raiz)
        printf("- ");
    else {
        printf("( %ld ", raiz->tipo.l);
        _Preorden_long(raiz->izq);
        _Preorden_long(raiz->der);
        printf(")");
    }
}

void Inorden(Arbol_AST* arbol) {
    if (!arbol->raiz) return;
    switch (arbol->tipo) {
        case Int:
        case Char:
            _Inorden_int(arbol->raiz);
            break;
        case Long:
            _Inorden_long(arbol->raiz);
            break;
        case Double:
            _Inorden_double(arbol->raiz);
            break;
    }
}

void _Inorden_int(Celda_A* raiz) {
    if (!raiz)
        printf(" - ");
    else {
        printf("( ");
        _Inorden_int(raiz->izq);
        printf(" %d ", raiz->tipo.i);
        _Inorden_int(raiz->der);
        printf(" )");
    }
}

void _Inorden_long(Celda_A* raiz) {
    if (!raiz)
        printf(" - ");
    else {
        printf("( ");
        _Inorden_long(raiz->izq);
        printf(" %ld ", raiz->tipo.l);
        _Inorden_long(raiz->der);
        printf(" )");
    }
}

void _Inorden_double(Celda_A* raiz) {
    if (!raiz)
        printf(" - ");
    else {
        printf("( ");
        _Inorden_double(raiz->izq);
        printf(" %lf ", raiz->tipo.d);
        _Inorden_double(raiz->der);
        printf(" )");
    }
}

void Postorden(Arbol_AST* arbol) {
    if (!arbol->raiz) return;
    switch (arbol->tipo) {
        case Int:
        case Char:
            _Postorden_int(arbol->raiz);
            break;
        case Long:
            _Postorden_long(arbol->raiz);
            break;
        case Double:
            _Postorden_double(arbol->raiz);
            break;
    }
}

void _Postorden_int(Celda_A* raiz) {
    if (!raiz)
        printf(" -");
    else {
        printf("( ");
        _Postorden_int(raiz->izq);
        _Postorden_int(raiz->der);
        printf(" %d )", raiz->tipo.i);
    }
}

void _Postorden_long(Celda_A* raiz) {
    if (!raiz)
        printf(" -");
    else {
        printf("( ");
        _Postorden_int(raiz->izq);
        _Postorden_int(raiz->der);
        printf(" %ld )", raiz->tipo.l);
    }
}

void _Postorden_double(Celda_A* raiz) {
    if (!raiz)
        printf(" -");
    else {
        printf("( ");
        _Postorden_int(raiz->izq);
        _Postorden_int(raiz->der);
        printf(" %lf )", raiz->tipo.d);
    }
}
#endif