#include "utiles.h"

void affData (Data d)
{
	int i;
	printf("-----------Data-----------\n");
	for (i=0; i < NBCLIENT; i++)
	{
		affClient(d.clients[i]);
	}
	
	printf("\nColis leger\n");
	
	for (i=0; i < NBCOLIS; i++)
	{
		affColis(d.leger[i]);
	}

	printf("\nColis moyen\n");
	
	for (i=0; i < NBCOLIS; i++)
	{
		affColis(d.moyen[i]);
	}

	printf("\nColis lourd\n");
	
	for (i=0; i < NBCOLIS; i++)
	{
		affColis(d.lourd[i]);
	}
}

void affClient (Client c)
{
	char chaine[100];
	sprintf(chaine, "id : %d, couloir : %d, dist : %d, present : ", c.id, c.couloir, c.dist);
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
