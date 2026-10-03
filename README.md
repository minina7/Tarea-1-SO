# Planificador Dieciochero

Tarea 1, Sistemas Operativos

Programa que debe simular actividades organizadas mediante un DAG, donde cada actividad tiene un ID, nombre, tiempo de ejecucion y dependencias.

## Integrante

- Daneza Parraguez

## Compilacion y uso

```bash

make
./planificador plan.txt K

```
- `plan.txt`: archivo con las actividades.
- `K`: entero >= 1, maximo de procesos que podran ejecutarse al mismo tiempo.

El programa se compila con C++17 utilizando las opciones:

`g++ -Wall -Wextra -std=c++17 ... -lpthread`

## Formato de plan.txt

Cada linea del archivo debe tener el siguiente formato:

ID : Nombre : tiempo_ms : dependencia1, dependencia2

Ejemplo:

1 : prender_carbon : 500 :
2 : comprar_carne : 1200 :
3 : asar_carne : 800 : 1, 2

Si el tiempo de una actividad viene vacio, el programa le asigna un tiempo aleatorio entre 100 y 5000 ms.

## Funciones implementadas

### Main

- Valida que se ingresen los argumentos necesarios.
- Valida que K sea un numero mayor que 0.
- Llama al parser para cargar las actividades.
- Construye el DAG a partir de las actividades leidas.

### Parser

- Lee las actividades desde el archivo.
- Separa los campos de cada linea.
- Valida que cada actividad tenga el formato correcto.
- Valida que los IDs sean alfanumericos y no esten repetidos.
- Valida los tiempos de ejecucion.
- Asigna un tiempo aleatorio cuando no se especifica uno.
- Lee las dependencias de cada actividad.
- Detecta dependencias a si misma.
- Verifica que las dependencias indicadas existan.

### DAG 

Se encuentra implementada la construccion del grafo y la deteccion de ciclos

- Se relaciona cada ID con su posicion dentro del vector de actividades.
- Se calcula el grado de entrada de cada actividad.
- Se guarda que actividades dependen de cada nodo.
- Se detectan ciclos utilizando el algoritmo de Kahn.
- Si el plan contiene un ciclo, el programa termina indicando error.


### Procesos y control de concurrencia

- Cada actividad se ejecuta utilizando un proceso creado con `fork()`.
- El proceso hijo simula el tiempo de ejecucion de la actividad.
- Se utiliza `waitpid()` para esperar la finalizacion de procesos sin busy-waiting.
- Se respeta el limite de concurrencia indicado por `K`.
- Las actividades listas se almacenan en una cola.
- Cuando una actividad termina correctamente, se actualizan las dependencias de las actividades que dependen de ella.

### Pipes

- Cada proceso hijo utiliza un pipe para comunicar su finalizacion.
- Al terminar una actividad se envia un mensaje con el ID de la actividad.
- El proceso padre recibe el mensaje y luego actualiza las dependencias correspondientes.
- Los descriptores de los pipes se cierran despues de ser utilizados.

### Manejo de fallas y señales

- Si una actividad falla, el programa no termina completamente.
- Se abortan las actividades que dependen de la actividad fallida.
- Las ramas independientes pueden continuar ejecutandose.
- Al presionar Ctrl+C se maneja la señal `SIGINT`.
- Cuando se recibe `SIGINT`, se terminan los procesos hijos activos y el programa finaliza.

## Decisiones de diseño

- El parser se encuentra separado de la construccion del DAG para mantener las funciones del programa mas ordenadas.
- Se utiliza `unordered_map` para relacionar rapidamente cada ID con su indice dentro del vector.
- Cada nodo del DAG guarda su grado de entrada y una lista de actividades dependientes.
- Para la deteccion de ciclos se utiliza el algoritmo de Kahn.
- Se utiliza una cola para almacenar las actividades que se encuentran listas para ejecutar.
- Se mantiene un contador de procesos activos para no superar el limite `K`.
- Se utiliza `waitpid()` de manera bloqueante para evitar busy-waiting.
- Se utilizan pipes para comunicar la finalizacion de los procesos hijos.

## Pruebas

Los archivos utilizados para probar el programa se encuentran en la carpeta: `tests/`

Actualmente se han realizado pruebas para:

- archivos validos
- IDs duplicados
- IDs invalidos
- dependencias inexistentes
- actividades sin tiempo definido
- auto-dependencias
- planes con ciclos
- pruebas de estres con 10000 actividades
- cadena de 10000 actividades
- 10000 actividades independientes
- pruebas con distintos valores de `K`
- manejo de `SIGINT`
- aislamiento de fallas

## Limitaciones

- El programa fue probado con planificaciones de hasta 10000 actividades.
- La comunicacion de finalizacion se realiza desde cada proceso hijo hacia el proceso padre mediante pipes.