#include <stdio.h>
#include <stdlib.h>

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
