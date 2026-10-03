#include "procesos.h"
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <csignal>
using namespace std;

pid_t iniciar_actividad(const Actividad& actividad, int& fd_lectura){

    int canal[2];

    if (pipe(canal) == -1){

        cerr << "Error creando pipe para actividad "
             << actividad.id << endl;

        return -1;
    }

    pid_t pid = fork();

    if (pid < 0){

        cerr << "Error al crear proceso para actividad "
             << actividad.id << endl;

        close(canal[0]);
        close(canal[1]);

        return -1;
    }

    if (pid == 0){

        signal(SIGINT, SIG_IGN);

        close(canal[0]);

        cout << "Iniciando actividad " << actividad.id
             << ": " << actividad.nombre << endl;

        usleep(actividad.tiempo * 1000);

        cout << "Finalizando actividad " << actividad.id
             << ": " << actividad.nombre << endl;

        char mensaje[128];

        snprintf(mensaje, sizeof(mensaje),
                 "FIN:%s", actividad.id.c_str());

        write(canal[1], mensaje, strlen(mensaje));

        close(canal[1]);

        _exit(0);
    }

    close(canal[1]);

    fd_lectura = canal[0];

    return pid;
}