#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NBCOLIS 50
#define NBCLIENT 40

struct Client{
	int id;
	int couloir;
	int dist;
};

struct Colis{
	int idClient;
	int poids;
};

typedef struct Client Client;
typedef struct Colis Colis;

struct data{
	//creer 4 tableaux de colis. 1 par categorie de poids et reparti sur les doc d'appro, 1 ou plusieur docs / categorie de poids
	Colis colis[NBCOLIS];
	Client clients[NBCLIENT];
};

typedef struct data data;
