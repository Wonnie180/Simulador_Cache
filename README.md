# Simulador_Cache
Para compilar basta con que se respete la estructura de carpetas (src/SimuladorCache.c libraries/Ficheros.h)

Al ejecutar se PUEDE especificar por parametros la direccion del fichero de configuracion, la de las trazas y un valor 
para mostrar los contenidos de la cache (1 muestra la cache principal, 2 muestra la VictimCache y 3 muestra ambas).

En caso de no especificar las direcciones de los archivos, se usaran por defecto los archivos config.txt y traza.txt que
deberan estar en la misma carpeta que el ejecutable.

EJEMPLO de ejecucción: ./SimuladorCache ./configuraciones/config1.txt ./trazas/traza1.txt 3 
En este caso se ejecutara con la configuración ./configuraciones/config1.txt, la traza ./trazas/traza1.txt y mostrara los contenidos tanto de la cache
como de la VictimCache.