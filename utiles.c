#include "utiles.h"

void affData (Data d)
{
	int i;
	printf("-----------Data-----------\n");
	for (i=0; i < NBCLIENT; i++)
	{
		affClient(d.clients[i]);
	}
	
	printf("\nColis\n");
	for (i=0; i < NBCOLIS; i++)
	{
		affColis(d.colis[i]);
	}

	printf("\nIndex\n");
	printf("idMoyen : %d\nidLourd : %d\n", d.idMoyen, d.idLourd);
}

void affClient (Client c)
{
	char chaine[100];
	sprintf(chaine, "id : %d, couloir : %d, dist : %d, nbColis : %d, present : ", c.id, c.couloir, c.dist, c.nbColis);
	if (c.present)
		strcat(chaine, "OUI\n");
	else
		strcat(chaine, "NON\n");
	printf("%s", chaine);
}

void affColis (Colis c)
{
	printf("idCli : %d, poids : %d\n", c.idClient, c.poids);
}

int rand_min_max (int min, int max)
{
	return rand()%(min - max) + min;
}
