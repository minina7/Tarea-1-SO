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

`g++ -Wall -Wextra -std=c++17...`

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

Por ahora se encuentra implementada la construccion basica del grafo

- Se relaciona cada ID con su posicion dentro del vector actividades.
- Se calcula el grado de entrada de cada actividad.
- Se guarda que actividades dependen de cada nodo.

Deteccion de ciclos en desarrollo


### Procesos y control de concurrencia

pendiente

### Pipes

pendiente

### Manejo de fallas y señales

pendiente

## Decisiones de diseño

- El parser se encuentra separado de la construccion del DAG para mantener las funciones del programa mas ordenadas.
- Se utiliza `unordered_map` para relacionar rapidamente cada ID con su indice dentro del vector.
- Cada nodo del DAG guarda su grado de entrada y una lista de actividades dependientes.
- Para la deteccion de ciclos se utilizara el algoritmo de Kahn.

## Pruebas

Los archivos utilizados para probar el programa se encuentran en la carpeta: `tests/`

Actualmente se han realizado pruebas para:

- archivos validos
- IDs duplicados
- IDs invalidos
- dependencias inexistentes
- actividades sin tiempo definido
- auto-dependencias

## Limitaciones

Todavia falta implementar:

- deteccion de ciclos
- creacion de procesos
- limite de concurrencia `K`
- comunicacion mediante pipes
- manejo de fallas
- manejo de `SIGINT`
- pruebas de estres