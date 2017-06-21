#include "utiles.h"

/*fonction affichant les donnée client et colis*/
void affData (Data d)
{
	int i;
	printf("-----------Data-----------\n");
	for (i=0; i < d.nbClientsTot; i++)
	{
		affClient(d.clients[i]);
	}
	
	printf("\nColis\n");
	for (i=0; i < d.nbColisTot; i++)
	{
		affColis(d.colis[i]);
	}
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
	char chaine[100];
	sprintf(chaine, "idCli : %d, poids : %d, etat : ", c.idClient, c.poids);
	if (c.livrer == 1)
	{
		strcat(chaine, "Livrer\n");
		green(chaine);
	}
	else if(c.livrer == 0)
	{
		strcat(chaine, "Pas livrer\n");
		red(chaine);
	}
	else
	{
		strcat(chaine, "En cours\n");
		yellow(chaine);
	}
}

/*permet d'afficher le deroulement des actions des drones*/
void afficherTableau(char *chaine, int index, int largeur, char color)
{
	int i, j, taille;
	for (i=0; i < largeur; i++)
	{
		if (i == index)
		{
			taille = (ESPACEMENTTAB * 8 - strlen(chaine) + 6)/ 8;
			printf("|");
			afficherEnFonctionCouleur(chaine, color);
			for (j=0; j < taille; j++)
			{
				printf("\t");
			}
		}
		else
		{
			printf("|");
			for(j=0; j < ESPACEMENTTAB; j++)
			{
				printf("\t");
			}
		}
	}
	printf("|\n");
}

void afficherEnFonctionCouleur(char *chaine, char color)
{
	if (color == 'r')
		red(chaine);
	else if (color == 'b')
		blue(chaine);
	else if (color == 'g')
		green(chaine);
	else if (color == 'y')
		yellow(chaine);
	else if (color == 'c')
		cyan(chaine);
	else
		printf("%s", chaine);
}

/*affiche l'entete du tableaux*/
void afficherEntete(int nbDrone)
{
	char chaine[50];
	int i, j, taille;
	
	for (i=0; i <  nbDrone * ESPACEMENTTAB * 8; i++)
	{
		printf("-");
	}
	printf("\n");

	for(i=0; i < nbDrone; i++)
	{
		sprintf(chaine, "Drone %d", i);
		taille = (ESPACEMENTTAB * 8 - strlen(chaine) + 6) / 8;
		printf("|%s", chaine);
		for (j=0; j < taille; j++)
		{
			printf("\t");
		}
	}
	printf("|\n");

	for (i=0; i <  nbDrone * ESPACEMENTTAB * 8; i++)
	{
		printf("-");
	}
	printf("\n");
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
