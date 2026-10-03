#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cerrno>
#include <vector>
#include <queue>
#include <unordered_map>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#include "parser.h"
#include "dag.h"
#include "procesos.h"

using namespace std;

volatile sig_atomic_t interrupcion = 0;

void manejar_sigint(int){

    interrupcion = 1;
}

void abortar_rama(size_t indice,
                  const vector<NodoDAG>& dag,
                  const vector<Actividad>& actividades,
                  vector<bool>& abortadas,
                  size_t& terminadas){

    for (size_t i = 0; i < dag[indice].dependientes.size(); i++){

        size_t dependiente = dag[indice].dependientes[i];

        if (!abortadas[dependiente]){

            abortadas[dependiente] = true;
            terminadas++;

            cerr << "Actividad " << actividades[dependiente].id
                 << " abortada por dependencia fallida" << endl;

            abortar_rama(dependiente,
                         dag,
                         actividades,
                         abortadas,
                         terminadas);
        }
    }
}

int main (int argc, char* argv[]){

    if (argc != 3){

        cerr << "Ingresar formato correcto:\nEjemplo: ./planificador plan.txt K" << endl;
        
        return 1;
    }

    struct sigaction sa;

    sa.sa_handler = manejar_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, nullptr);

    string nombre_archivo = argv[1]; 
    char* fin;
    errno = 0;
    long k = strtol (argv[2], &fin, 10);

    if (errno == ERANGE){

        cerr << "Numero fuera de rango" << endl;

        return 1;
    }

    if (fin == argv[2]){

        cerr << "Numero invalido" << endl;

        return 1;
    }

    if (*fin != '\0'){
        
        cerr << "Numero invalido" << endl;

        return 1;
    }

    if (k <= 0){

        cerr << "Ingrese un numero valido." << endl;

        return 1;
    }

    srand (time(nullptr));

    vector<Actividad> actividades;

    if (!leer_plan(nombre_archivo, actividades)){

        return 1;
    }

    vector<NodoDAG> dag;

    if (!construir_dag(actividades, dag)){

        return 1;
    }

    if (tiene_ciclo(dag)){

        cerr << "El plan contiene un ciclo" << endl;

        return 1;
    }

    vector<size_t> grados_actuales(dag.size());
    queue<size_t> listas;

    for (size_t i = 0; i < dag.size(); i++){

        grados_actuales[i] = dag[i].grado_entrada;

        if (grados_actuales[i] == 0){

            listas.push(i);
        }
    }

    unordered_map<pid_t, size_t> actividad_por_pid;
    unordered_map<pid_t, int> pipe_por_pid;

    size_t procesos_activos = 0;
    size_t terminadas = 0;

    vector<bool> abortadas(actividades.size(), false);

    while (terminadas < actividades.size()){

        if (interrupcion){

            cerr << "\nSIGINT recibido. Abortando actividades..." << endl;

            for (auto& par : actividad_por_pid){

                kill(par.first, SIGTERM);
            }

            for (auto& par : actividad_por_pid){

                waitpid(par.first, nullptr, 0);
            }

            for (auto& par : pipe_por_pid){

                close(par.second);
            }

            return 1;
        }

        while (!listas.empty() &&
               procesos_activos < static_cast<size_t>(k) &&
               !interrupcion){

            size_t indice = listas.front();
            listas.pop();

            int fd_lectura;

            pid_t pid = iniciar_actividad(actividades[indice], fd_lectura);

            if (pid < 0){

                return 1;
            }

            actividad_por_pid[pid] = indice;
            pipe_por_pid[pid] = fd_lectura;

            procesos_activos++;
        }

        int estado;

        pid_t pid_terminado = waitpid(-1, &estado, 0);

        if (pid_terminado < 0){

            if (errno == EINTR && interrupcion){

                cerr << "\nSIGINT recibido. Abortando actividades..." << endl;

                for (auto& par : actividad_por_pid){

                    kill(par.first, SIGTERM);
                }

                for (auto& par : actividad_por_pid){

                    waitpid(par.first, nullptr, 0);
                }

                for (auto& par : pipe_por_pid){

                    close(par.second);
                }

                return 1;
            }

            cerr << "Error esperando un proceso" << endl;

            return 1;
        }

        size_t indice_terminado =
            actividad_por_pid[pid_terminado];

        int fd_lectura =
            pipe_por_pid[pid_terminado];

        char mensaje[128];

        ssize_t leidos =
            read(fd_lectura, mensaje, sizeof(mensaje) - 1);

        if (leidos > 0){

            mensaje[leidos] = '\0';

            cout << "Mensaje recibido: "
                 << mensaje << endl;
        }

        close(fd_lectura);

        actividad_por_pid.erase(pid_terminado);
        pipe_por_pid.erase(pid_terminado);

        procesos_activos--;
        terminadas++;

        if (WIFEXITED(estado) &&
            WEXITSTATUS(estado) == 0){

            for (size_t i = 0;
                 i < dag[indice_terminado].dependientes.size();
                 i++){

                size_t dependiente =
                    dag[indice_terminado].dependientes[i];

                grados_actuales[dependiente]--;

                if (grados_actuales[dependiente] == 0 &&
                    !abortadas[dependiente]){

                    listas.push(dependiente);
                }
            }

        }else{

            cerr << "Fallo la actividad "
                 << actividades[indice_terminado].id
                 << endl;

            abortar_rama(indice_terminado,
                         dag,
                         actividades,
                         abortadas,
                         terminadas);
        }
    }

    cout << "actividades cargadas: " << actividades.size() << endl;

    cout << "Archivo: " << nombre_archivo << endl;

    cout << "K: " << k << endl;

    return 0;
}