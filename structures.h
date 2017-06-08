#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#define NBCOLIS 40
#define NBCLIENT 40
#define NBDOCS 4
#define NBSLOTS 10

#define TAILLELEGERCOLIS 3
#define TAILLEMOYENCOLIS 6

#include <pthread.h>

typedef struct Client{
	int id;
	int couloir[2]; /* 0 : couloir libre, 1 : couloir occupé */
	int dist;
	int present;
	int nbColis;
	int dronePresent;
	pthread_mutex_t mutex_client;/* peut etre deux*/
	pthread_cond_t cond_client;
} Client;

typedef struct Colis{
	int idClient;
	int poids;
	int livrer;/*0 pour non, -1 pour en cours, 1 pour livrer*/
} Colis;

typedef struct Data{
	Colis colis[NBCOLIS];
	Client clients[NBCLIENT];
	pthread_mutex_t mutex_docs;
	pthread_cond_t cond_docs;
	int nbDocs;
	pthread_mutex_t mutex_slotRecharge;
	pthread_cond_t cond_slotRecharge;
	int nbSlotRecharge;
	int idMoyen;
	int idLourd;
} Data;

typedef struct Drone{
	int id;/*inutile*/
	int capaciteActuel;
	int poidsMaximum
} Drone;

#endif // STRUCTURES_H_INCLUDED
