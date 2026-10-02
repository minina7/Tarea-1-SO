#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>

struct Actividad{

    std::string id;
    std::string nombre;
    long tiempo;
    std::vector<std::string> dependencias;
};


bool leer_plan (const std::string& nombre_archivo);

#endif