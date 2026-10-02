#include "procesos.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
using namespace std;

pid_t iniciar_actividad(const Actividad& actividad){

    pid_t pid = fork();

    if (pid < 0){

        cerr << "Error al crear proceso para la actividad "
             << actividad.id << endl;

        return -1;
    }

    if (pid == 0){

        cout << "Iniciando actividad " << actividad.id
             << ": " << actividad.nombre << endl;

        usleep(actividad.tiempo * 1000);

        cout << "Finalizando actividad " << actividad.id
             << ": " << actividad.nombre << endl;

        _exit(0);
    }

    return pid;
}