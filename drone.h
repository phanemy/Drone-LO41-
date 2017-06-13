#ifndef DRONE_H_INCLUDED
#define DRONE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include "structures.h"
#include "utiles.h"

void *droneThread(void *data);

#endif // DRONE_H_INCLUDED
