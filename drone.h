#ifndef DRONE_H_INCLUDED
#define DRONE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "utiles.h"
#include <pthread.h>
#define CAPACITEBATTERY 90


void *droneThread(void *data);

#endif // DRONE_H_INCLUDED
