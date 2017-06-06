#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PARTAGECOLIS /colis
#define PARTAGECLIENT /client

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
