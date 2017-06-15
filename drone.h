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
void initDrone(Drone* drone);
int rechercheColis(Data* data,Drone* drone);
int prendreColis(Data* data, Drone* drone);
void attacheDock(Data* data);
void detacheDock(Data* data);
void recharge(Data* data,Drone* drone);
int attacheSlotRecharge(Data* data, Drone* drone);
void detacheSlotRecharge(Data* data,Drone* drone);
void livreColis(Data* data, int i,Drone* drone);
void descente(Client* client, Drone* drone);
void remonte(Data* data, Client* client, Drone* drone,int temps,int i);
int donneColis(Client *c);

#endif
