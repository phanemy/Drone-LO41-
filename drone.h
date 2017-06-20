#ifndef DRONE_H_INCLUDED
#define DRONE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include "structures.h"
#include "utiles.h"

void *droneThread(void *data);
void initDrone(Drone* drone, Data* data);
int rechercheColis(Data* data,Drone* drone);
int prendreColis(Data* data, Drone* drone);
void attacheDock(Data* data, Drone* drone);
void detacheDock(Data* data, Drone* drone);
void recharge(Data* data,Drone* drone);
int attacheSlotRecharge(Data* data, Drone* drone);
void detacheSlotRecharge(Data* data,Drone* drone);
void livreColis(Data* data, int i,Drone* drone);
void descente(Client* client, Data* data, Drone* drone);
void remonte(Data* data, Client* client, Drone* drone,int temps,int i);
int donneColis(Client *c, Data* d, Drone* drone);
void afficherInfo(Data *d, char *chaine, int index, int nbDrone, char color);

#endif
