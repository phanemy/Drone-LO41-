#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#define NBCOLIS 10 /*nb colis par categorie*/
#define NBCLIENT 40

typedef struct Client{
	int id;
	int couloir;
	int dist;
	int present;
} Client;

typedef struct Colis{
	int idClient;
	int poids;
} Colis;

typedef struct Data{
	Colis leger[NBCOLIS];
	Colis moyen[NBCOLIS];
	Colis lourd[NBCOLIS];
	Client clients[NBCLIENT];
} Data;

#endif // STRUCTURES_H_INCLUDED
