#include "dag.h"
#include <unordered_map>
#include <string>
#include <queue>
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

bool tiene_ciclo(const vector<NodoDAG>& dag){

    vector<size_t> grados;
    queue<size_t> cola;

    grados.resize(dag.size());

    for (size_t i = 0; i < dag.size(); i++){

        grados[i] = dag [i].grado_entrada;

        if (grados[i] == 0){

            cola.push(i);
        }
    }
    
    size_t procesados = 0;
    while (!cola.empty()){

    size_t actual = cola.front();
    cola.pop();

    procesados++;

    for (size_t i = 0; i < dag[actual].dependientes.size(); i++){

        size_t dependiente = dag[actual].dependientes[i];

        grados[dependiente]--;

        if (grados[dependiente] == 0){

            cola.push(dependiente);
        }
    }
}
    

    return procesados != dag.size();
}