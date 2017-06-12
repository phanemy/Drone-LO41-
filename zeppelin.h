#ifndef ZEPPZLIN_H_INCLUDED
#define ZEPPZLIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "drone.h"
#include "client.h"
#include "structures.h"
#include "utiles.h"

Data initData ();
void livreColis (Client *c);
void destroyTout (Data *d);
void triColis (Data *d);
void traitantSIGINT(int num);

#endif // ZEPPZLIN_H_INCLUDED
