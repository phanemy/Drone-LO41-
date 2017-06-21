#include "zeppelin.h"

pthread_t *drones, *clients;
int controlC = 1, nbDrones = 4, nbClients = 4, nbColisTotal = 20;
Data data;

int main(int argc, char *argv[]){
	int i;
	/*on initialise les données*/
	if (argc > 1)
	{
		nbDrones = atoi(argv[1]);
		if (argc > 2)
		{
			nbClients = atoi(argv[2]);
			if (argc > 3)
				nbColisTotal = atoi(argv[3]);
		}
	}

	printf("Nombre de drones : %d\n", nbDrones);
	printf("Nombre de clients : %d\n", nbClients);
	printf("Nombre de colis : %d\n", nbColisTotal);

	drones = malloc(sizeof(pthread_t) * nbDrones);
	clients = malloc(sizeof(pthread_t) * nbClients);
	
	data = initData();
	

	affData(data);

	afficherEntete(nbDrones);
	
	/*on defini les actions a faire sur les signaux*/
	signal(SIGTSTP, traitantSIGTSTP);
	signal(SIGINT, traitantSIGINT);
	
	/*on lance les threads*/
	for (i=0; i < nbClients; i++)
	{
		if (pthread_create (&clients[i], NULL, clientThread, &data.clients[i]))
			exit(EXIT_FAILURE);
	}

	for (i=0; i < nbDrones; i++)
	{	
		if (pthread_create (&drones[i], NULL, droneThread, &data))
			exit(EXIT_FAILURE);
	}
	
	
	
	/*on attend qu'ils soit tous fini pour s'arreter*/
	for (i=0; i < nbDrones; i++)
	{
		pthread_join (drones[i], NULL);
	}
	for (i=0; i < nbClients; i++)
	{
		pthread_join (clients[i], NULL);
	}

	affData(data);
	
	/*on detruit tout les objet posix et data créer*/
	destroyTout(&data);

	free(drones);
	free(clients);

	green("Fin de la simulation\n");

	return 0;
}

/*fonction initalisant toute les donnée en fonction de parametre de lancement et des define*/
Data initData()
{
	Data d;
	int i, id;
	int premierApa, fin;

	d.colis = malloc(sizeof(Colis) * nbColisTotal);
	d.clients = malloc(sizeof(Client) * nbClients);
	d.nbColisTot = nbColisTotal;
	d.nbClientsTot = nbClients;

	d.nbDronesTot = nbDrones;

	d.numDrone = 0;

	srand(time(NULL));

	for (i=0; i < nbClients; i++)
	{
		d.clients[i].id = i;
		d.clients[i].couloir[0] = 0;
		d.clients[i].couloir[1] = 0;
		d.clients[i].dist = rand_min_max(2,31);
		d.clients[i].present = rand_min_max(0,11);
		d.clients[i].nbColis = 0;
		d.clients[i].dronePresent = 0;
		pthread_mutex_init(&d.clients[i].mutex_client, NULL);
		d.clients[i].cond_client = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
	}
	
	for (i=0; i < nbColisTotal; i++)
	{
		id = rand_min_max(0,nbClients);
		d.clients[id].nbColis++;
		d.colis[i].idClient = id;
		d.colis[i].poids = rand_min_max(0,11);
		d.colis[i].livrer = 0;
	}

	triColis(&d);

	premierApa = d.colis[0].poids;
	fin = 0;
	i = 1;
	
	d.idMoyen = 0;
	d.idLourd = 0;

	while (fin < 2 && i < nbColisTotal)
	{
		if (d.colis[i].poids != premierApa)
		{
			premierApa = d.colis[i].poids;
			if (premierApa == TAILLELEGERCOLIS)
			{
				d.idMoyen = i;
				fin++;
			}
			else if (premierApa == TAILLEMOYENCOLIS)
			{
				d.idLourd = i;
				fin++;
			}
		}
		i++;
	}
	if (i == nbColisTotal && d.idMoyen == 0)
		d.idMoyen = nbColisTotal;
	if (i == nbColisTotal && d.idLourd == 0)
		d.idLourd = nbColisTotal;

	pthread_mutex_init(&d.mutex_docs, NULL);
	d.cond_docs = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

	d.nbSlotRecharge = NBSLOTS;
	pthread_mutex_init(&d.mutex_slotRecharge, NULL);
	d.cond_slotRecharge = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

	d.nbDocksAppro = NBDOCKS;
	pthread_mutex_init(&d.mutex_docksAppro, NULL);
	d.cond_docksAppro = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

	pthread_mutex_init(&d.mutex_collis, NULL);
	pthread_mutex_init(&d.mutex_affichage, NULL);
	return d;
}

/*fonction liberant tout les object créer( mutex, cond)*/
void destroyTout (Data *d)
{
	int i;

	yellow("Nettoyage\n");

	for (i=0; i < nbClients; i++)
	{
		pthread_mutex_destroy(&d->clients[i].mutex_client);
		pthread_cond_destroy(&d->clients[i].cond_client);
	}
	
	pthread_mutex_destroy(&d->mutex_docs);
	pthread_cond_destroy(&d->cond_docs);

	pthread_mutex_destroy(&d->mutex_docksAppro);
	pthread_cond_destroy(&d->cond_docksAppro);	

	pthread_mutex_destroy(&d->mutex_slotRecharge);
	pthread_cond_destroy(&d->cond_slotRecharge);

	pthread_mutex_destroy(&d->mutex_collis);
	pthread_mutex_destroy(&d->mutex_affichage);

	free(d->clients);
	free(d->colis);
}

/*fonction triant les colis par poids croissant*/
void triColis (Data *d)
{
	int i = 0, j, changement;
	Colis temps;
	do
	{
		changement = 0;
		for (j=0; j < nbColisTotal - i - 1; j++)
		{
			if (d->colis[j].poids > d->colis[j+1].poids)
			{
				changement = 1;
				temps = d->colis[j];
				d->colis[j] = d->colis[j+1];
				d->colis[j+1] = temps;
			}
		}
		i++;
	}while (changement && i < nbColisTotal);
}

/*fonction definissant l'action a realiser en cas de CTRL+C:
On arrete tous les threds se qui permet au programme de se finir
normalement en supprimant les objets créer*/
void traitantSIGINT(int num){
	if (controlC)
	{
		int i;
		if (num != SIGINT)
			fprintf(stderr, "Probleme sur SigInt\n");

		for (i=0; i<nbDrones; i++)
		{
			pthread_cancel(drones[i]);
		}

		for (i=0; i<nbClients; i++)
		{
			pthread_cancel(clients[i]);
		}
		controlC = 0;
		red("Control C\n");
	}
}

/*fonction definissant l'action a realiser en cas de CTRL+Z:
mets en pause la simulation et affiche les data*/
void traitantSIGTSTP (int num)
{
	if (num != SIGTSTP)
		fprintf(stderr, "Probleme sur SigTstp\n");

	affData(data);

	raise(SIGSTOP);
}
