#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "structures.h"
#include "utiles.h"

void *clientThread(void *data);
void gestionClient (Client *c);

#endif // CLIENT_H_INCLUDED
