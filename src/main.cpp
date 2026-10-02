#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cerrno>
#include "parser.h"
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

    cout << "actividades cargadas: " << actividades.size() << endl;

    cout << "Archivo: " << nombre_archivo << endl;
    cout << "K: " << k << endl;

    return 0;
    
}