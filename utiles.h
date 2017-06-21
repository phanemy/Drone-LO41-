#ifndef UTILES_H_INCLUDED
#define UTILES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "structures.h"

#define RED "\x1B[1;31m"
#define BLUE "\x1B[1;34m"
#define CYAN "\x1B[1;36m"
#define GREEN "\x1B[1;32m"
#define YELLOW "\x1B[1;33m"
#define NC "\x1B[0m"

#define ESPACEMENTTAB 3

void affData (Data d);
void affClient (Client c);
void affColis (Colis c);
void afficherTableau(char *chaine, int index, int largeur, char color);
void afficherEntete(int nbDrone);
void afficherEnFonctionCouleur(char *chaine, char color);
int rand_min_max (int min, int max);
void red(char *chaine);
void blue(char *chaine);
void cyan(char *chaine);
void green(char *chaine);
void yellow(char *chaine);

#endif
