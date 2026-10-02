#include "parser.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <cstdlib>
#include <cerrno>
#include <cctype>
using namespace std;
 
string trim (const string& texto){

    size_t inicio = texto.find_first_not_of (" \t");
    size_t fin = texto.find_last_not_of (" \t");

    if (inicio == string::npos){

        return "";
    }

    return texto.substr (inicio, fin - inicio + 1);
}

bool leer_plan (const string& nombre_archivo,

    vector<Actividad>& actividades){

    ifstream archivo (nombre_archivo);

    if (!archivo.is_open()){

        cerr << "No fue posible leer el archivo: " << nombre_archivo << endl;
        return false;
    }

    string linea;
    unordered_set<string> ids_vistos;

    

    while (getline (archivo, linea)){

        if (!linea.empty() && linea.back() == '\r'){

            linea.pop_back();
        }

        if (trim(linea).empty()){
            
            continue;
        }

        stringstream ss (linea);
        string campo;
        vector <string> campos;

        while (getline (ss, campo, ':')){

            campos.push_back (campo);

        }

        string linea_limpia = trim (linea);

        if (!linea_limpia.empty() && linea_limpia.back() == ':'){

            campos.push_back("");
        }

        for (size_t i = 0; i < campos.size(); i++){

            campos[i] = trim (campos[i]);
        }

        if (campos.size() != 4){

            cerr << "Formato ingresado invalido: " << linea << endl;

            return false;
        }

        string id = campos [0];

        if (id.empty()){

            cerr << "ID se encuentra vacio" << endl;

            return false;
        }

        for (char c : id){

            if (!isalnum (static_cast <unsigned char> (c))){

                cerr << "ID invalido: " << id << endl;

                return false;
            }
        }

        if (ids_vistos.find (id) != ids_vistos.end ()){

            cerr << "ID duplicado: " << id <<  endl;

            return false;
        }

        string nombre = campos[1];

        if (nombre.empty()){

            cerr << "Nombre se encuentra vacio" << endl;

            return false;
        }

        string tiempo_texto = campos[2];
        long tiempo;

        if (tiempo_texto.empty()){

            tiempo = rand() % 4901 + 100;

        }else{

            char* fin_tiempo;
            errno = 0;

            long valor = strtol (tiempo_texto.c_str(), &fin_tiempo, 10);

            if (errno == ERANGE){

                cerr << "Tiempo fuera de rango" << endl;

                return false;
            }

            if (fin_tiempo == tiempo_texto.c_str()){

                cerr << "Tiempo invalido" << endl;

                return false;
            }

            if (*fin_tiempo != '\0'){

                cerr << "Tiempo invalido" << endl;

                return false;
            }

            if (valor <= 0){

                cerr << "El tiempo debe ser mayor que cero" << endl;

                return false;
            }

            tiempo = valor;
        }

        vector<string> dependencias;

        if (!campos[3].empty()){

            stringstream ss_deps(campos[3]);
            string dependencia;

            while (getline(ss_deps, dependencia, ',')){

                dependencia = trim(dependencia);

                if (!dependencia.empty()){

                    dependencias.push_back(dependencia);
                }
            }
        }

        for (size_t i = 0; i < dependencias.size(); i++){

            if (dependencias[i] == id){

                cerr << "Una actividad no puede depender de si misma " << id << endl;

                return false;
            }
        }


        ids_vistos.insert(id);

        Actividad actividad;

        actividad.id = id;
        actividad.nombre = nombre;
        actividad.tiempo = tiempo;
        actividad.dependencias = dependencias;

        actividades.push_back (actividad);


    }


    for (size_t i = 0; i < actividades.size(); i++){

        for (size_t j = 0; j < actividades[i].dependencias.size(); j++){

            string dep = actividades[i].dependencias[j];

            if (ids_vistos.find(dep) == ids_vistos.end()){

                cerr << "Dependencia inexistente: " << dep << endl;

                return false;
            }
        }
    }
    return true;
}