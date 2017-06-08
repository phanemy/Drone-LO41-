#ifndef UTILES_H_INCLUDED
#define UTILES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

#define RED "\e[1;31m"
#define BLUE "\e[1;34m"
#define CYAN "\e[1;36m"
#define GREEN "\e[1;32m"
#define YELLOW "\e[1;33m"
#define NC "\e[0m"

void affData (Data d);
void affClient (Client c);
void affColis (Colis c);
int rand_min_max (int min, int max); /*min inclus et max exclus*/
void red(char *chaine);
void blue(char *chaine);
void cyan(char *chaine);
void green(char *chaine);
void yellow(char *chaine);

#endif // UTILES_H_INCLUDED
