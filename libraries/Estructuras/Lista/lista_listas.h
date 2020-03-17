/*
Autor: Luis Jose Llamas Perez
Fecha (dd/mm/yyyy): 05/09/2018
Descripcion: Ejercicio 5
*/

// Cabezera
// Librerias
#include <stdio.h>
#include <stdlib.h>
// Constantes
enum state {a,b,c,d,e};
// Tipos
typedef struct Celda{
	int num;
	struct Celda *sig;
	struct Celda *ant;
} Celda;

typedef struct Lista{
	Celda* inicio;
	Celda* final;
	int	   tamano;
} Lista;

// Declaracion de procedimientos y funciones       

// Operaciones con celdas
Celda* CrearCelda(int elemento);
void DestruirCelda(Celda* celda);

// Operaciones con colas
Lista* CrearLista();
void DestruirLista(Lista* lista);
void Enlistar(Lista* lista, int elemento);
void Desenlistar(Lista* lista, int elemento);
Celda* Buscar_en_lista_doblelig(Lista* lista, int elemento);
void VaciarLista(Lista* lista);
int Es_vacia_Lista(Lista* lista);
void MostrarListaAsc(Lista* lista);
void MostrarListaDesc(Lista* lista);

// FUNCIONES

Celda* CrearCelda(int elemento){
	Celda* celda = (Celda *)malloc(sizeof(Celda));
	celda->num = elemento;
	celda->sig = NULL;
	celda->ant = NULL;
	return celda;
}

void DestruirCelda(Celda* celda){
	celda->num = 0;
	celda->sig = NULL;
	celda->ant = NULL;
	free(celda);
}

Lista* CrearLista(){
	Lista* lista = (Lista *)malloc(sizeof(Lista));
	lista->inicio = NULL; lista->final=NULL;
	lista->tamano = 0;
	return lista;
}

void DestruirLista(Lista* lista){
	if(Es_vacia_Lista(lista)){
		free(lista);
	} else {
		VaciarLista(lista);
		free(lista);
	}	
}

void Enlistar(Lista* lista, int elemento){
	Celda* aux;
	Celda* nueva_celda = CrearCelda(elemento);
	// Si la cola no tiene elementos, asignar el primer elemento
	// como incio y final de la cola
	if (Es_vacia_Lista(lista)){
		lista->inicio = nueva_celda;
		lista->final = nueva_celda;
		lista->tamano++;
	// Como la cola tiene elementos, habra que comprobar donde 
	// hay que introducir el nuevo elemento dependiendo de su
	// valor, en este caso se hara en orden descendente (los
	// elementos duplicados se insertaran detras de los existentes).
	} else {
		// Si el nuevo elemento es mayor que el maximo existente
		// en la lista
		if (lista->inicio->num < nueva_celda->num){
			nueva_celda->sig = lista->inicio;
			lista->inicio->ant = nueva_celda;
			lista->inicio = nueva_celda;
			lista->tamano++;
		// Si el nuevo elemento es menor o gual que el minimo existente
		// en la lista, insertarlo al final
		} else if (lista->final->num >= nueva_celda->num){
			nueva_celda->ant = lista->final;
			lista->final->sig = nueva_celda;
			lista->final = nueva_celda;
			lista->tamano++;
		// Si no se da ninguno de los casos anteriores habra que
		// buscar el elemento menor al que se desea introducir
		} else {
			// Encuentra el primer elemento igual al elemento que
			// se desea introducir
			aux = Buscar_en_lista_doblelig(lista, elemento);
			// Encuentra el primer elemento MENOR que el nuevo a
			// introducir.
			while (aux != NULL && aux->num == elemento)
				aux = aux->sig;
			// Comprobacion final antes de añadir
			if (aux != NULL){
				nueva_celda->sig = aux;
				nueva_celda->ant = aux->ant;
				aux->ant = nueva_celda;
				nueva_celda->ant->sig = nueva_celda;
				lista->tamano++;
			}
		}	
	}
}

void Desenlistar(Lista* lista, int elemento){
	Celda* celda_a_eliminar;
	if (!(Es_vacia_Lista(lista))){
		celda_a_eliminar = Buscar_en_lista_doblelig(lista, elemento);
		// Si ha encontrado el elemento, proceder a borrarlo
		if (celda_a_eliminar != NULL){
			// Si el elemento a eliminar es el unico elemento de la lista
			if (celda_a_eliminar->ant == NULL && celda_a_eliminar->sig == NULL){
				lista->inicio = NULL;
				lista->final = NULL;
			// Si el elemento a eliminar es el inicio
			} else if (celda_a_eliminar->ant == NULL){
				lista->inicio = celda_a_eliminar->sig;
				lista->inicio->ant = NULL;
			// Si el elemento a eliminar es el final
			} else if (celda_a_eliminar->sig == NULL){
				lista->final = celda_a_eliminar->ant;
				lista->final->sig = NULL;
			// Caso general
			} else {
				// Elemento anterior apunta a elemento siguiente
				celda_a_eliminar->ant->sig = celda_a_eliminar->sig;
				// Elemento anterior siguiente a elemento anterior
				celda_a_eliminar->sig->ant = celda_a_eliminar->ant;
			}
			lista->tamano--;
			printf("El elemento %d se ha eliminado de la lista\n",elemento);
			DestruirCelda(celda_a_eliminar);
		} else {
			printf("El elemento %d no existe en la lista\n",elemento);
		}
	}
}

void MostrarListaDesc(Lista* lista){
	Celda* aux;
	int i=0;
	aux=lista->inicio;
	printf("\nNumero de elementos: %d\n",lista->tamano);
	while (aux != NULL){
		printf("%d - ",aux->num);
		aux = aux->sig;
		i++;
	}
	printf("\n");
}

void MostrarListaAsc(Lista* lista){
	Celda* aux;
	int i=0;
	aux=lista->final;
	printf("\nNumero de elementos: %d\n",lista->tamano);
	while (aux != NULL){
		printf("%d - ",aux->num);
		aux = aux->ant;
		i++;
	}
	printf("\n");
}

Celda* Buscar_en_lista_doblelig(Lista* lista, int elemento){
	Celda* aux_s;
	Celda* aux_a;
	aux_s=lista->inicio;
	aux_a=lista->final;
	// Recorrer la lista mientras no se llegue a NULL o
	// se encuentre un elemento el cual es menor al buscado
	while (aux_s->num > elemento && aux_a->num < elemento && aux_s->num != aux_a->num){
		aux_s = aux_s->sig;
		aux_a = aux_a->ant;
	}
	// Si has encontrado el elemento por la izquierda (prioridad por
	// como almaceno los elementos en la cola (mayor a menor))
	if (aux_s->num == elemento){
		return aux_s;
	// Si has encontrado el elemento por la derecha
	} else if (aux_a->num == elemento){
		return aux_a;
	} else 
		return NULL;	
}

void VaciarLista(Lista* lista){
	while (!(Es_vacia_Lista(lista))){
		Desenlistar(lista, lista->inicio->num);
	}
}

int Es_vacia_Lista(Lista* lista){
	if(!lista->inicio){
		return 1;
	} else {
		return 0;
	}
}