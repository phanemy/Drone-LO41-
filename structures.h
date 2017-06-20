#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#define NBSLOTS 4
#define NBDOCKS 2
#define CAPACITEBATTERY 90

#define TAILLELEGERCOLIS 3
#define TAILLEMOYENCOLIS 6

#define TIMESCALE 0.25

#include <pthread.h>

struct Client{
	int id;
	int couloir[2]; /* 0 : couloir libre, 1 : couloir occupé */
	int dist;
	int present;
	int nbColis;
	int dronePresent;
	pthread_mutex_t mutex_client;/* peut etre deux*/
	pthread_cond_t cond_client;
};
typedef struct Client Client;

struct Colis{
	int idClient;
	int poids;
	int livrer;/*0 pour non, -1 pour en cours, 1 pour livrer*/
};
typedef struct Colis Colis;

struct Data{
	Colis *colis;
	Client *clients;
	pthread_mutex_t mutex_docs;
	pthread_cond_t cond_docs;
	int nbColisTot;
	int nbClientsTot;
	
	int nbDronesTot; /*utile pour l'affichage*/
	int numDrone;

	int nbSlotRecharge;
	pthread_mutex_t mutex_slotRecharge;
	pthread_cond_t cond_slotRecharge;

	int nbDocksAppro;
	pthread_mutex_t mutex_docksAppro;
	pthread_cond_t cond_docksAppro;
	
	int idMoyen;
	int idLourd;
	pthread_mutex_t mutex_collis;

	pthread_mutex_t mutex_affichage;
};
typedef struct Data Data;

struct Drone{
	int id;
	int capaciteActuel;
	int poidsMaximum;
};
typedef struct Drone Drone;

#endif
