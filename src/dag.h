#ifndef DAG_H
#define DAG_H
#include <vector>
#include "parser.h"

struct NodoDAG{

    size_t grado_entrada;
    std::vector<size_t> dependientes;
};

bool construir_dag(const std::vector<Actividad>& actividades,

    std::vector<NodoDAG>& dag);

bool tiene_ciclo(const std::vector<NodoDAG>& dag);
#endif