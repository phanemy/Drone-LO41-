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
	sprintf(chaine, "id : %d, couloir1 : %d, couloir2 : %d, dist : %d, nbColis : %d, present : ", c.id, c.couloir[0], c.couloir[1], c.dist, c.nbColis);
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

void red(char *chaine)
{
	printf("%s%s%s", RED, chaine, NC);
}

void blue(char *chaine)
{
	printf("%s%s%s", BLUE, chaine, NC);
}

void cyan(char *chaine)
{
	printf("%s%s%s", CYAN, chaine, NC);
}

void green(char *chaine)
{
	printf("%s%s%s", GREEN, chaine, NC);
}

void yellow(char *chaine)
{
	printf("%s%s%s", YELLOW, chaine, NC);
}
