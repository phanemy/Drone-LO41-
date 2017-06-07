#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#define NBCOLIS 10 /*nb colis par categorie*/
#define NBCLIENT 40

#include <pthread.h>

typedef struct Client{
	int id;
	int couloir;
	int dist;
	int present;
	int nbColis;
	int dronePresent;
	pthread_mutex_t mutex_client;
	pthread_cond_t cond_client;
} Client;

typedef struct Colis{
	int idClient;
	int poids;
	int livrer;/*0 pour non, -1 pour en cours, 1 pour livrer*/
} Colis;

typedef struct Data{
	Colis leger[NBCOLIS];
	Colis moyen[NBCOLIS];
	Colis lourd[NBCOLIS];
	Client clients[NBCLIENT];
} Data;

typedef struct Drone{
	int id;/*inutile*/
	int capaciteActuel;
	int poidsMaximum
} Drone;

#endif // STRUCTURES_H_INCLUDED
