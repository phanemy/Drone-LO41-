#ifndef ZEPPZLIN_H_INCLUDED
#define ZEPPZLIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include "drone.h"
#include "client.h"
#include "structures.h"
#include "utiles.h"
#define NBDRONE 10

Data initData ();
void livreColis (Client *c);
void destroyTout (Data *d);
void triColis (Data *d);

#endif // ZEPPZLIN_H_INCLUDED
