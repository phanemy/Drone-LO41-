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
	Colis colis[NBCOLIS];
	Client clients[NBCLIENT];
};

typedef struct data data;
