#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cerrno>
#include <vector>
#include "parser.h"
#include "dag.h"
using namespace std;


int main (int argc, char* argv[]){

    if (argc != 3){

        cerr << "Ingresar formato correcto:\nEjemplo: ./planificador plan.txt K" << endl;
        return 1;
    }

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

    if ( k <= 0){

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

    for (size_t i = 0; i < dag.size(); i++){

        cout << "actividad " << actividades[i].id
        << " grado entrada: " << dag[i].grado_entrada << endl;

        cout << "Dependientes: ";

        for (size_t j = 0; j < dag[i].dependientes.size(); j++){

            size_t indice_dependiente = dag[i].dependientes[j];

            cout << actividades[indice_dependiente].id << " ";
        }

        cout << endl;
    }

    cout << "actividades cargadas: " << actividades.size() << endl;

    cout << "Archivo: " << nombre_archivo << endl;
    cout << "K: " << k << endl;

    return 0;
    
}