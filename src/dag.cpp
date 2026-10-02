#include "dag.h"
#include <unordered_map>
#include <string>
using namespace std;

bool construir_dag (const vector<Actividad>& actividades,
    vector<NodoDAG>& dag){

    unordered_map<string, size_t> indice_por_id;

    for (size_t i = 0; i < actividades.size(); i++){

        indice_por_id[actividades[i].id] = i;
    }

     dag.resize(actividades.size());

    for (size_t i = 0; i < actividades.size(); i++){

        dag[i].grado_entrada = actividades[i].dependencias.size();
    }

    for (size_t i = 0; i < actividades.size(); i++){

        for (size_t j = 0; j < actividades[i].dependencias.size(); j++){

            string id_dependencia = actividades[i].dependencias[j];

            size_t indice_dependencia = indice_por_id[id_dependencia];

            dag[indice_dependencia].dependientes.push_back(i);
        }
    }


    return true;

}