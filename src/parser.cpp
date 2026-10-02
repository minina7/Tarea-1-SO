#include "parser.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

bool leer_plan (const string& nombre_archivo){

    ifstream archivo (nombre_archivo);

    if (!archivo.is_open()){

        cerr << "No fue posible leer el archivo: " << nombre_archivo << endl;
        return false;
    }

    string linea;

    while (getline (archivo, linea)){

        cout << linea << endl;
    }

    return true;
}