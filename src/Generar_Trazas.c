#include <stdio.h>
#include <stdlib.h>
#include "../libraries/Ficheros.h"

Fichero* f;
char DirTraza[] = "./traza_VC.txt";
/*
#define N 1024
int main() {
    double v[N], res;
    f = crearFichero();
    inicializarFicheroEscritura(f, DirTraza);
    for (unsigned long i = 0; i < N; i++) {
        res = res + v[i];
        // fprintf(f->f, "%p ", &res);
        fprintf(f->f, "%p ", &v[i]);
        // fprintf(f->f, "%p ", &res);
    }
    for (unsigned long i = 0; i < N; i++) {
        res = res + v[i];
        // fprintf(f->f, "%p ", &res);
        fprintf(f->f, "%p ", &v[i]);
        // fprintf(f->f, "%p ", &res);
    }
    cerrarFichero(f);
    return 0;
}
*/

#define N 8
#define M 1024
int main() {
    double A[N][M];
    double x;
    f = crearFichero();
    inicializarFicheroEscritura(f, DirTraza);
    for (unsigned long j = 0; j < M; j++) {
        for (unsigned long i = 0; i < N; i++) {
            x = A[i][j] + x;
            fprintf(f->f, "%p ", &A[i][j]);
        }
    }
    cerrarFichero(f);
    return 0;
}

/*
#define N 6
#define M 128
int main() {
    float A[N][M], res;
    f = crearFichero();
    inicializarFicheroEscritura(f, DirTraza);
    for (unsigned long j = 0; j < M; j++) {
        for (unsigned long i = 0; i < N; i++) {
            res = A[i][j];
            fprintf(f->f, "%p ", &res);
            fprintf(f->f, "%p ", &A[i][j]);
        }
    }
    cerrarFichero(f);
    return 0;
}
*/
/*
#define N 256
#define M 64
int main() {
    float A[N][M], res;
    f = crearFichero();
    inicializarFicheroEscritura(f, DirTraza);
    for (unsigned long j = 0; j < M; j++) {
        for (unsigned long i = 0; i < N; i++) {
            res = res + A[i][j];
           // fprintf(f->f, "%p ", &res);
            fprintf(f->f, "%p ", &A[i][j]);
           // fprintf(f->f, "%p ", &res);
        }
    }
    cerrarFichero(f);
    return 0;
}
*/
/*
#define N 256
#define M 64
int main() {
    float A[N][M], res;
    f = crearFichero();
    inicializarFicheroEscritura(f, DirTraza);
    for (unsigned long j = 0; j < M; j++) {
        for (unsigned long i = 0; i < N / 2; i++) {
            res = res + A[i][j];
            fprintf(f->f, "%p ", &A[i][j]);
        }
    }
    for (unsigned long j = 0; j < M; j++) {
        for (unsigned long i = N / 2; i < N; i++) {
            res = res + A[i][j];
            fprintf(f->f, "%p ", &A[i][j]);
        }
    }
    cerrarFichero(f);
    return 0;
}
*/