#ifndef DRONE_H_INCLUDED
#define DRONE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#define CapaciteBattery 90

typedef struct Drone{
	int id;
	int capaciteActuel;
	int poidsMaximum
};

void *droneThread(void *data);

#endif // DRONE_H_INCLUDED
