#ifndef PROCESOS_H
#define PROCESOS_H
#include "parser.h"
#include <sys/types.h>

pid_t iniciar_actividad(const Actividad& actividad);

#endif