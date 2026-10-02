#include "parser.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

string trim (const string& texto){

    size_t inicio = texto.find_first_not_of (" \t");
    size_t fin = texto.find_last_not_of (" \t");

    if (inicio == string::npos){

        return "";
    }

    return texto.substr (inicio, fin - inicio + 1);
}

bool leer_plan (const string& nombre_archivo){

    ifstream archivo (nombre_archivo);

    if (!archivo.is_open()){

        cerr << "No fue posible leer el archivo: " << nombre_archivo << endl;
        return false;
    }

    string linea;

    while (getline (archivo, linea)){

        stringstream ss (linea);
        string campo;
        vector <string> campos;

        while (getline (ss, campo, ':')){

            campos.push_back (campo);

        }


        if (!linea.empty() && linea.back() == ':'){

            campos.push_back("");
        }

        for (size_t i = 0; i < campos.size(); i++){

            campos[i] = trim (campos[i]);
        }

        for (size_t i = 0; i < campos.size(); i++){

        cout << "Campo " << i << ": [" << campos[i] << "]" << endl;
        
        }


    }


    return true;
}