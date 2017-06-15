#include "zeppelin.h"

pthread_t drones[NBDRONE], clients[NBCLIENT];
int controlC = 1;
Data data;

int main(){
	int i;
	data = initData();

	signal(SIGTSTP, traitantSIGTSTP);

	affData(data);

	for (i=0; i < NBCLIENT; i++)
	{
		if (pthread_create (&clients[i], NULL, clientThread, &data.clients[i]))
			exit(EXIT_FAILURE);
	}

	for (i=0; i < NBDRONE; i++)
	{	
		printf("Initialisation du drone n°%d\n\n",i);
		if (pthread_create (&drones[i], NULL, droneThread, &data))
			exit(EXIT_FAILURE);
	}

	signal(SIGINT, traitantSIGINT);
	
	for (i=0; i < NBDRONE; i++)
	{
		pthread_join (drones[i], NULL);
	}
	for (i=0; i < NBCLIENT; i++)
	{
		pthread_join (clients[i], NULL);
	}

	affData(data);
	
	destroyTout(&data);

	green("Fin de la simulation\n");

	return 0;
}


Data initData()
{
	Data d;
	int i, id;
	int premierApa, fin;

	srand(time(NULL));

	for (i=0; i < NBCLIENT; i++)
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
	
	for (i=0; i < NBCOLIS; i++)
	{
		id = rand_min_max(0,NBCLIENT);
		d.clients[id].nbColis++;
		d.colis[i].idClient = id;
		d.colis[i].poids = rand_min_max(0,11);
		d.colis[i].livrer = 0;
	}

	triColis(&d);

	premierApa = d.colis[0].poids;
	fin = 0;
	i = 1;
	
	d.nbDocs = NBDOCS;
	d.idMoyen = 0;
	d.idLourd = 0;

	while (fin < 2 && i < NBCOLIS)
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
	if (i == NBCOLIS && d.idMoyen == 0)
		d.idMoyen = NBCOLIS;
	if (i == NBCOLIS && d.idLourd == 0)
		d.idLourd = NBCOLIS;

	pthread_mutex_init(&d.mutex_docs, NULL);
	d.cond_docs = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

	d.nbSlotRecharge = NBSLOTS;
	pthread_mutex_init(&d.mutex_slotRecharge, NULL);
	d.cond_slotRecharge = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

	d.nbDocksAppro = NBDOCKS;
	pthread_mutex_init(&d.mutex_docksAppro, NULL);
	d.cond_docksAppro = (pthread_cond_t) PTHREAD_COND_INITIALIZER;



	pthread_mutex_init(&d.mutex_collis, NULL);
	return d;
}

void destroyTout (Data *d)
{
	int i;

	yellow("Nettoyage\n");

	for (i=0; i < NBCLIENT; i++)
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
}

void triColis (Data *d)
{
	int i = 0, j, changement;
	Colis temps;
	do
	{
		changement = 0;
		for (j=0; j < NBCOLIS - i - 1; j++)
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
	}while (changement && i < NBCOLIS);
}

void traitantSIGINT(int num){
	if (controlC)
	{
		int i;
		if (num != SIGINT)
			fprintf(stderr, "Probleme sur SigInt\n");

		for (i=0; i<NBDRONE; i++)
		{
			if (pthread_cancel(drones[i]))
				printf("Pb liberation drones\n");
		}

		for (i=0; i<NBCLIENT; i++)
		{
			if (pthread_cancel(clients[i]))
				printf("Pb liberation clients\n");
		}
		controlC = 0;
		red("Control C\n");
	}
}

void traitantSIGTSTP (int num)
{
	if (num != SIGTSTP)
		fprintf(stderr, "Probleme sur SigTstp\n");

	affData(data);

	raise(SIGSTOP);
}
