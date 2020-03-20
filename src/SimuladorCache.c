#include <stdio.h>
#include <stdlib.h>
#include "../libraries/Ficheros.h"

//--------------------- ESTRUCTURAS ----------------------------
typedef struct Linea {
    unsigned long TAG;         // TAG de la linea
    unsigned long Antiguedad;  // Tiempo que lleva en cache
    unsigned char Activa;      // Indica si la linea contiene informacion
} Linea;

typedef struct Conjunto {
    unsigned long ID;       // ID del conjunto (0 - N-1)
    unsigned long nLineas;  // Número de lineas del conjunto
    Linea** lineas;         // Las lineas del conjunto
} Conjunto;

typedef struct Cache {
    unsigned long NumConjuntos;  // Numero de conjuntos
    Conjunto** C;                // Array de dichos conjuntos
} Cache;

//--------------------- VAR GLOBALES ----------------------------
#define N 16777216   // Numero de trazas maximo
#define TAM_DIR 48   // Numero de bits maximos posibles en la direccion
unsigned long Miss;  // Numero de fallos/misses
unsigned long Hit;   // Numero de aciertos/hit
long traza[N];       // Array de trazas
Fichero* f;          // Lectura de las configuraciones y las trazas
Cache *cache,        // La cache a simular
    *vcache;         // La VictimCache a simular
unsigned char
    bit_max_conjunto,  // Posicion (0 - N-1) del bit de mayor posicion de linea
    bit_max_linea;     // Posicion (0 - N-1) del bit de mayor posicion
                       // del conjunto
char DirConfig[] = "./config.txt";  // Direccion del fichero de configuracion
char DirTraza[] = "./traza.txt";    // Direccion del fichero con la traza

//---------------- DECLARACION FUNCIONES ------------------------
/**************************
 * CREACION DE ESTRUCTURAS *
 ***************************/
Linea* CrearLinea();
Conjunto* CrearConjunto(long nlineas);
Cache* crearCache(unsigned long nConjuntos, unsigned long Asociatividad);
/**************************
 *   LEER CONFIGS/TRAZAS   *
 ***************************/
void leerConfigCache();
void leerTrazas();
/**************************
 *    CALCULOS DE BITS     *
 ***************************/
unsigned char esPotenciade2(unsigned long num);
unsigned char numBits_para_N_Posiciones(unsigned long NUM);
void calcular_bits_linea_y_conjunto(long TamLineas, unsigned long nConjuntos);
unsigned long desplazarBitsIzq(unsigned long a, unsigned long b);
unsigned long desplazarBitsDer(unsigned long a, unsigned long b);
unsigned long int rangobits(unsigned long int n, int bitmenor, int bitmayor);
/**************************
 *          LRU            *
 ***************************/
void Actualizar_Antiguedad(Conjunto* C);
unsigned long ObtenerLineaAntigua(Conjunto* C);
void LRU(Conjunto* C, unsigned long TAG);
unsigned long calcularTAG_VC(unsigned long TAG, unsigned long ID_Conjunto);
void LRU_VC_H(Conjunto* C, Linea* linea, unsigned long TAG);
void LRU_VC_M(Conjunto* C, unsigned long TAG);
/**************************
 *        BUSQUEDAS        *
 ***************************/
Conjunto* obtenerConjunto(unsigned long direccion);
void buscarEnConjunto(Conjunto* C, unsigned long direccion);
Linea* buscarLinea(Conjunto* C, unsigned long TAG);
Linea* buscarEnConjuntoVC(Conjunto* C, unsigned long direccion);
/***************************
 * MOSTRAR DATOS/RESULTADOS *
 ****************************/
void MostrarCache();
void mostrarTraza();
void mostrarResultados();
void mostrarContenidoCache(Cache* Ca);
void mostrarContenidoConjunto(Conjunto* C, unsigned char nCon);
void mostrarBits(Conjunto* C, Linea* linea, unsigned char nCon);

//----------------------- MAIN -------------------------------
int main() {
    f = crearFichero();
    leerConfigCache();
    leerTrazas();
    // Procesar las trazas
    printf("----------------------\n");
    Conjunto* C;
    for (unsigned long i = 0; i < N && traza[i] != -1; i++) {
        C = obtenerConjunto(traza[i]);
        buscarEnConjunto(C, traza[i]);
    }
    // Fin del procesamiento
    mostrarResultados();
    mostrarContenidoCache(cache);
    // mostrarContenidoCache(vcache);
    return 0;
}

//--------------------- FUNCIONES ----------------------------

/**************************
 * CREACION DE ESTRUCTURAS *
 ***************************/
/******************************************************************************
    FUNCION: CrearLinea()
    DESCRIPCION: Crea una linea
 *******************************************************************************/
Linea* CrearLinea() {
    Linea* L = (Linea*)malloc(sizeof(Linea));
    L->Antiguedad = 0;
    L->TAG = 0;
    L->Activa = 0;
    return L;
}

/******************************************************************************
    FUNCION: CrearConjunto(nlineas)
    DESCRIPCION: Crea un conjunto con nlineas lineas
 *******************************************************************************/
Conjunto* CrearConjunto(long nlineas) {
    Conjunto* C = (Conjunto*)malloc(sizeof(Conjunto));
    C->ID = 0;
    C->nLineas = nlineas;
    C->lineas = (Linea**)malloc(sizeof(Linea*) * nlineas);
    for (unsigned long i = 0; i < nlineas; i++) C->lineas[i] = CrearLinea();
    return C;
}

/******************************************************************************
    FUNCION: crearCache(nConjuntos, Asociatividad)
    DESCRIPCION: Crea una cache Asociativa de N conjuntos con M Asociativa
 *******************************************************************************/
Cache* crearCache(unsigned long nConjuntos, unsigned long Asociatividad) {
    Cache* cache_aux = (Cache*)malloc(sizeof(Cache));
    cache_aux->NumConjuntos = nConjuntos;
    cache_aux->C = (Conjunto**)malloc(sizeof(Conjunto*) * nConjuntos);
    for (unsigned long i = 0; i < nConjuntos; i++) {
        cache_aux->C[i] = CrearConjunto(Asociatividad);
        cache_aux->C[i]->ID = i;
    }
    return cache_aux;
}

/****************************
 *       LEER CONFIGS        *
 *****************************/
/******************************************************************************
    FUNCION: leerTrazas()
    DESCRIPCION: Lee el archivo de trazas que simularan los accesos a memoria de
                 la cache.
 *******************************************************************************/
void leerTrazas() {
    inicializarFicheroLectura(f, DirTraza);
    long a = 0;
    for (long i = 0; i < N && a != -1; i++) {
        a = leerHexLongFichero(f);
        traza[i] = a;
    }
    cerrarFichero(f);
}

/******************************************************************************
    FUNCION: leerConfigCache()
    DESCRIPCION: Lee el archivo de configuracion para generar la cache y la VC
*******************************************************************************/
void leerConfigCache() {
    inicializarFicheroLectura(f, DirConfig);
    unsigned long nLineas = leerIntFichero(f), TamLineas = leerIntFichero(f),
                  Asociatividad = leerIntFichero(f),
                  nLineas_VictimCache = leerIntFichero(f);
    if (!esPotenciade2(nLineas) || !esPotenciade2(TamLineas) ||
        !esPotenciade2(Asociatividad) || Asociatividad > nLineas) {
        printf("Error en config.txt\n");
        exit(1);
    }
    Miss = 0;
    Hit = 0;
    calcular_bits_linea_y_conjunto(TamLineas, nLineas / Asociatividad);
    cache = crearCache(nLineas / Asociatividad, Asociatividad);
    if (nLineas_VictimCache > 0) {
        if (!esPotenciade2(nLineas_VictimCache)) {
            printf("Error en config.txt\n");
            exit(1);
        }
        vcache = crearCache(1, nLineas_VictimCache);
    } else
        vcache = NULL;
    cerrarFichero(f);
}

/****************************
 *     CALCULOS DE BITS     *
 *****************************/
/******************************************************************************
    FUNCION: esPotenciade2(num)
    DESCRIPCION: Devuelve si el número introducido es o no potencia de 2.
    FUNCIONAMIENTO: El número sera potencia de 2 si el número es 0 o si al
                    hacer la AND logica con su anterior el resultado es
                    distinto de 0.
    EJEMPLO: esPotenciade2(128) -> 1000000 & 01111111 = 00000000
             esPotenciade2(127) -> 0111111 & 01111110 = 01111110
             Por tanto, 128 es potencia de 2 y 127 no lo es.
*******************************************************************************/
unsigned char esPotenciade2(unsigned long num) {
    if ((num != 0) && ((num & (num - 1)) == 0)) return 1;
    return 0;
}

/*****************************************************************************
    FUNCION: numBits_para_N_Posiciones(NUM)
    DESCRIPCION: Devuelve el número de bits necesarios
                 para guardar NUM de estados.
    FUNCIONAMIENTO: La funcion recibe el numero de estados
                    que se quieren almacenar, si es menor
                    que 1, devuelve 0. Si es multiplo de 2
                    le resta 1 y comienza a contar el número
                    de bits que tiene.
    EJEMPLO: numBits_para_N_Posiciones(1)   -> 1
             numBits_para_N_Posiciones(2)   -> 1
             numBits_para_N_Posiciones(10)  -> 4
             numBits_para_N_Posiciones(128) -> 7
******************************************************************************/
unsigned char numBits_para_N_Posiciones(unsigned long NUM) {
    if (NUM < 1) return 0;
    if (NUM % 2 == 0) NUM--;
    char bit = 1;
    while ((NUM >>= 1) != 0) bit++;
    return bit;
}

/*****************************************************************************
    FUNCION: desplazarBitsDer(a, b)
    DESCRIPCION: Desplaza los bits de a hacia la derecha b veces
******************************************************************************/
unsigned long desplazarBitsDer(unsigned long a, unsigned long b) {
    return (a >>= b);
}

/*****************************************************************************
    FUNCION: desplazarBitsIzq(a, b)
    DESCRIPCION: Desplaza los bits de a hacia la izquierda b veces
******************************************************************************/
unsigned long desplazarBitsIzq(unsigned long a, unsigned long b) {
    return (a <<= b);
}

/*****************************************************************************
    FUNCION: calcular_bits_linea_y_conjunto(TamLineas, nConjuntos)
    DESCRIPCION: Obtiene los bits que separan las lineas de los conjuntos y
                 los conjuntos del TAG
    FUNCIONAMIENTO: Primero calcula el número de bits necesarios para
                    almacenar la linea, y una vez hecho, calcula los bits
                    necesarios para almacenar nConjuntos.
******************************************************************************/
void calcular_bits_linea_y_conjunto(long TamLineas, unsigned long nConjuntos) {
    bit_max_linea = (numBits_para_N_Posiciones(TamLineas));
    if (bit_max_linea > 0)
        bit_max_linea--;  // Los bits van de 0 a N-1, asi que hay que quitar 1
    bit_max_conjunto = numBits_para_N_Posiciones(nConjuntos) + bit_max_linea;
}

/*****************************************************************************
    FUNCION: rangobits(n, bitmenor, bitmayor)
    DESCRIPCION: Calcula el valor de los bits que hay entre bitmenor y
                 bitmayor
    FUNCIONAMIENTO:
    EJEMPLO: rangobits(10101110, 1, 4) -> 0111 -> 7
******************************************************************************/
unsigned long int rangobits(unsigned long int n, int bitmenor, int bitmayor) {
    unsigned long int bit2 = 1, bit1 = 1;
    if (bitmenor < 0 || bitmenor > TAM_DIR - 1) {
        printf("Error en bitmenor: %d\n", bitmenor);
        exit(0);
    } else if (bitmayor < 0 || bitmayor > TAM_DIR - 1) {
        printf("Error en bitmayor: %d\n", bitmayor);
        exit(0);
    } else if (bitmayor < bitmenor) {
        printf("Error en orden\n");
        exit(0);
    } else if (bitmayor - bitmenor + 1 < TAM_DIR) {
        /* calcula valor 2 elevado a bitmenor */
        bit1 = bit1 << bitmenor;
        /* calcula valor 2 elevado a (bitmayor-bitmenor + 1) **/
        bit2 = bit2 << (bitmayor - bitmenor + 1);
        n = n / bit1;
        n = n % bit2;
    }
    return n;
}

/****************************
 *            LRU           *
 *****************************/

/*****************************************************************************
    FUNCION: Actualizar_Antiguedad(C)
    DESCRIPCION: Aumenta en 1 la antiguedad de todas las lineas del conjunto
******************************************************************************/
void Actualizar_Antiguedad(Conjunto* C) {
    for (unsigned long i = 0; i < C->nLineas; i++) {
        C->lineas[i]->Antiguedad++;
    }
}

/*****************************************************************************
    FUNCION: ObtenerLineaAntigua(C)
    DESCRIPCION: Devuelve la posicion de la linea que lleve mas tiempo sin
                 ser usada
    FUNCIONAMIENTO: Realiza una busqueda con recorrido de todas las lineas
                    del conjunto y devuelve aquella que tenga mas Antiguedad.
******************************************************************************/
unsigned long ObtenerLineaAntigua(Conjunto* C) {
    long l = 0;
    for (unsigned long i = 1; i < C->nLineas; i++) {
        if (C->lineas[i]->Antiguedad > C->lineas[l]->Antiguedad) l = i;
    }
    return l;
}

/*****************************************************************************
    FUNCION: LRU(C, TAG)
    DESCRIPCION: Sustituye la linea con mas antiguedad (la que hace mas tiempo
                 que no se ha usado) y la sustituye por la nueva. Para la
                 simulación esto se hace cambiando unicamente el tag. Una vez
                 con el nuevo TAG, se pone su antiguedad a 0 y se marca como
                 Activa (es decir, que tiene datos relevantes para la traza)
******************************************************************************/
void LRU(Conjunto* C, unsigned long TAG) {
    long l = ObtenerLineaAntigua(C);
    C->lineas[l]->TAG = TAG;
    C->lineas[l]->Antiguedad = 0;
    C->lineas[l]->Activa = 1;
}

/*****************************************************************************
    FUNCION: calcularTAG_VC(TAG, ID_Conjunto)
    DESCRIPCION: Funcion que concatena los bits de TAG y Conjunto para obtener
                 el TAG de la VC.
    FUNCIONAMIENTO: Se desplazan hacia la izquierda tantos bits como bits
                    tenga los conjuntos de la cache principal y se hace
                    la operación OR del TAG desplazado y el conjunto.
    EJEMPLO: calcularTAG_VC(101, 001) -> (Desplazamiento) 101000 ->
             (OR) 101001
******************************************************************************/
unsigned long calcularTAG_VC(unsigned long TAG, unsigned long ID_Conjunto) {
    unsigned long TAG_VC;
    TAG_VC =
        desplazarBitsIzq(TAG, (numBits_para_N_Posiciones(cache->NumConjuntos)));
    TAG_VC = TAG_VC | ID_Conjunto;
    return TAG_VC;
}

/*****************************************************************************
    FUNCION: LRU_VC_H(C, linea, TAG)
    DESCRIPCION: Ejecuta LRU con VictimCache en caso de acierto en la VC
    FUNCIONAMIENTO: Obtiene la linea mas antigua de la Cache principal, se
                    calcula el TAG para la VC de dicha linea y se mete en la
                    VC poniendo su antiguedad a 0. Y en la cache principal se
                    actualiza el TAG y la antiguedad de la linea "nueva" que
                    estaba en la VC.
******************************************************************************/
void LRU_VC_H(Conjunto* C, Linea* linea, unsigned long TAG) {
    unsigned long l = ObtenerLineaAntigua(C);
    // linea contiene la linea de la VC
    linea->TAG = calcularTAG_VC(C->lineas[l]->TAG, C->ID);
    linea->Antiguedad = 0;
    // Linea en la Cache principal
    C->lineas[l]->TAG = TAG;
    C->lineas[l]->Antiguedad = 0;
}

/*****************************************************************************
    FUNCION: LRU_VC_M(C, TAG)
    DESCRIPCION: Ejecuta LRU con VictimCache en caso de fallo en la VC.
    FUNCIONAMIENTO: Obtiene la linea mas antigua de la Cache principal y si la
                    linea esta Activa, la guarda en la VC, ejecutando LRU en
                    la VC (mismo proceso ya explicado anteriormente).
******************************************************************************/
void LRU_VC_M(Conjunto* C, unsigned long TAG) {
    unsigned long l = ObtenerLineaAntigua(C);
    // Si la linea de la Cache esta activa, guardarla en la VC
    if (C->lineas[l]->Activa) {
        unsigned long l_VC = ObtenerLineaAntigua(vcache->C[0]);
        vcache->C[0]->lineas[l_VC]->TAG =
            calcularTAG_VC(C->lineas[l]->TAG, C->ID);
        vcache->C[0]->lineas[l_VC]->Antiguedad = 0;
        vcache->C[0]->lineas[l_VC]->Activa = 1;
    }
    // Reemplazar la linea de la cache principal con la nueva linea
    C->lineas[l]->TAG = TAG;
    C->lineas[l]->Antiguedad = 0;
    C->lineas[l]->Activa = 1;
}

/****************************
 *         BUSQUEDAS        *
 ****************************/
/*****************************************************************************
    FUNCION: obtenerConjunto(direccion)
    DESCRIPCION: Obtiene el conjunto al que pertenece una dirección de memoria
    FUNCIONAMIENTO: Utilizando la funcion rangobits, obtiene el valor que hay
                    entre el primer bit de conjunto y el ultimo. En caso de
                    solo existir un conjunto, devuelve este.
******************************************************************************/
Conjunto* obtenerConjunto(unsigned long direccion) {
    if (cache->NumConjuntos == 1) return cache->C[0];
    return cache->C[rangobits(direccion, bit_max_linea + 1, bit_max_conjunto)];
}

/*****************************************************************************
    FUNCION: buscarEnConjunto(C, direccion)
    DESCRIPCION: Busca en el conjunto si existe la linea, en caso de existir,
                 sera un acierto (hit), en caso contrario sera un fallo (miss)
    FUNCIONAMIENTO: Obtiene el TAG de la direccion (primero obtiene el mayor
                    bit de la direccion) y utiliza dicho TAG para buscar la
                    linea en el conjunto y la VC si existe.
******************************************************************************/
void buscarEnConjunto(Conjunto* C, unsigned long direccion) {
    unsigned long TAG;
    // bits_TAG contiene el numero de bits total de la direccion
    unsigned char bits_TAG = numBits_para_N_Posiciones(direccion);
    // Si la cache es totalmente Asociativa
    if (cache->NumConjuntos < 2)
        TAG = rangobits(direccion, bit_max_linea + 1, bits_TAG);
    // Si los bits de conjunto y del tag se solapan, es que el TAG es 0
    else if (bit_max_conjunto >= bits_TAG)
        TAG = 0;
    // En caso "Normal", obtener el TAG desde donde terminan los bits
    // del conjunto, hasta el ultimo bit de la direccion
    else
        TAG = rangobits(direccion, bit_max_conjunto + 1, bits_TAG);
    Linea* linea = buscarLinea(C, TAG);
    Actualizar_Antiguedad(C);
    if (linea) {
        linea->Antiguedad = 0;
        Hit++;
    } else if (vcache) {
        linea = buscarEnConjuntoVC(vcache->C[0], direccion);
        if (linea) {
            Hit++;
            LRU_VC_H(C, linea, TAG);
        } else {
            Miss++;
            LRU_VC_M(C, TAG);
        }
    } else {
        Miss++;
        LRU(C, TAG);
    }
}

/*****************************************************************************
    FUNCION: buscarLinea(C, TAG)
    DESCRIPCION: Busca la linea en el Conjunto y en caso de que exista, la
                 devuelve.
    FUNCIONAMIENTO: Realiza una busqueda de la linea (TAG), y en cuanto la
                    encuentra, la devuelve.
******************************************************************************/
Linea* buscarLinea(Conjunto* C, unsigned long TAG) {
    Linea* linea = NULL;
    for (long i = 0; i < C->nLineas && !linea; i++)
        if (C->lineas[i]->Activa && C->lineas[i]->TAG == TAG)
            linea = C->lineas[i];
    return linea;
}

/*****************************************************************************
    FUNCION: buscarEnConjuntoVC(C, direccion)
    DESCRIPCION: A efectos practicos, es lo mismo que buscarLinea, pero en
                 este caso, se construye el TAG de la VC para buscar la linea.
    FUNCIONAMIENTO: Realiza una busqueda de la linea (TAG), y en cuanto la
                    encuentra, la devuelve.
******************************************************************************/
Linea* buscarEnConjuntoVC(Conjunto* C, unsigned long direccion) {
    unsigned long TAG;
    unsigned char bits_TAG = numBits_para_N_Posiciones(direccion);
    if (bit_max_linea < 1)
        TAG = direccion;
    else if (bit_max_linea >= bits_TAG)
        TAG = 0;
    else
        TAG = rangobits(direccion, bit_max_linea + 1, bits_TAG);
    Linea* linea = buscarLinea(C, TAG);
    Actualizar_Antiguedad(C);
    if (linea) linea->Antiguedad = 0;
    return linea;
}

/****************************
 * MOSTRAR DATOS/RESULTADOS *
 ****************************/
/*****************************************************************************
    FUNCION:  mostrarBits(C, linea, nCon)
    DESCRIPCION: Muestra una linea en formato binario y hexadecimal separando
                 el TAG, el conjunto y los bits de linea.
    FUNCIONAMIENTO: Se crea la variable 'i' la cual tiene tantos bits como 
                    el maximo tamaño posible de direcciones (constante TAM_DIR)
                    menos el tamaño actual de las direccion a mostrar.
                    Despues se concatenan los bits del tag y del conjunto.
                    Se ponen los bits de linea a 0 y se guarda esa dirección
                    en la variable 'dir_l' la cual es la direccion mas baja
                    de la linea. Tras esto, se ponen los bits de linea a 1 y 
                    se guarda en 'dir_h', la cual es la direccion mas alta de
                    la linea.
******************************************************************************/
void mostrarBits(Conjunto* C, Linea* linea, unsigned char nCon) {
    unsigned long long i;
    i = (unsigned long)1 << TAM_DIR - (bit_max_conjunto + bit_max_linea + 1);
    // Se recorre la direccion bit a bit (dividiendo entre 2) y mostrando 1 o 0
    // dependiendo del valor de dicho bit.
    for (; i > 0; i = i / 2) {
        if (linea->TAG & i)
            printf("1");
        else
            printf("0");
    }
    unsigned long long dir_l, dir_h;
    dir_l = linea->TAG;
    // Caso especial en el que solo haya un conjunto (VC por ejemplo)
    if (nCon == 1) {
        dir_l = calcularTAG_VC(linea->TAG, C->ID);
        printf("|");
        i = (unsigned long long)1 << (bit_max_conjunto - bit_max_linea - 1);
        for (; i > 0; i = i / 2) {
            if (C->ID & i)
                printf("1");
            else
                printf("0");
        }
    }
    dir_l = desplazarBitsIzq(dir_l, bit_max_linea + 1);
    dir_h = dir_l;
    dir_h |= ((1 << bit_max_linea + 1) - 1);
    // Poner a 1 los bit_maxlinea+1 ultimos bits de dir_l
    if (bit_max_linea > 0) {
        printf("|");
        for (i = 0; i <= bit_max_linea; i++)
            printf("x");
    }
    printf(" -> 0x%x - 0x%x\n", dir_l, dir_h);
}

/*****************************************************************************
    FUNCION: mostrarContenidoConjunto(C, nCon)
    DESCRIPCION: Muestra las lineas que hay en el conjunto
******************************************************************************/
void mostrarContenidoConjunto(Conjunto* C, unsigned char nCon) {
    printf("Conjunto %lu:\n", C->ID);
    for (unsigned long i = 0; i < C->nLineas; i++) {
        if (C->lineas[i]->Activa)
            mostrarBits(C, C->lineas[i], nCon);
        else
            printf("-\n");
    }
}

/*****************************************************************************
    FUNCION: mostrarContenidoCache()
    DESCRIPCION: Muestra el contenido de la cache
******************************************************************************/
void mostrarContenidoCache(Cache* Ca) {
    if (!Ca) return;
    printf("Contenido Cache con %lu Conjuntos:\n", Ca->NumConjuntos);
    for (unsigned long i = 0; i < Ca->NumConjuntos; i++) {
        mostrarContenidoConjunto(Ca->C[i], (Ca->NumConjuntos > 1));
    }
    printf("------------\n\n");
}

/*****************************************************************************
    FUNCION: mostrarTraza()
    DESCRIPCION: Muestra todas las direcciones de la traza en hexadecimal
******************************************************************************/
void mostrarTraza() {
    for (long i = 0; i < N && traza[i] != -1; i++) {
        printf("0x%lx ", traza[i]);
    }
    printf("\n");
}

/*****************************************************************************
    FUNCION: mostrarResultados()
    DESCRIPCION: Muestra el número de aciertos y fallos, asi como la tasa de
                 fallos.
******************************************************************************/
void mostrarResultados() {
    float Tasa_Fallos = (Miss / (float)(Hit + Miss)) * 100;
    printf(
        "\n---RESULTADOS---\n"
        "Aciertos: %lu\nFallos: %lu\n"
        "Tasa de fallos: %.4f%%"
        "\n----------------\n\n",
        Hit, Miss, Tasa_Fallos);
}
//  --- FIN ---
