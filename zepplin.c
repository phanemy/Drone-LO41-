#include "zepplin.h"

int main(){
	Data data = initData();
	pthread_t drones[NBDRONE], clients[NBCLIENT];
	int i, id;

	affData(data);
	
	/*for (i=0; i < NBDRONE; i++)
	{
		pthread_create (&drones[i], NULL, droneThread, &data);
	}*/
	/*
	for (i=0; i < NBCLIENT; i++)
	{
		pthread_create (&clients[i], NULL, clientThread, &data.clients[i]);
	}

	for (i=0; i < NBCOLIS; i++)
	{
		sleep(1);
		id = data.leger[i].idClient;
		livreColis(&data.clients[id]);

		sleep(1);
		id = data.moyen[i].idClient;
		livreColis(&data.clients[id]);

		sleep(1);
		id = data.lourd[i].idClient;
		livreColis(&data.clients[id]);
	}*/
	
	/*for (i=0; i < NBDRONE; i++)
	{
		pthread_join (drones[i], NULL);
	}*/
/*
	for (i=0; i < NBCLIENT; i++)
	{
		pthread_join (clients[i], NULL);
	}
	*/

	destroyTout(&data);

	return 0;
}


Data initData()
{
	Data d;
	int i, id;

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
		d.clients[i].mutex_client = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
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

	int premierApa = d.colis[0].poids, fin = 0;
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

	d.mutex_docs = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	d.cond_docs = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

	d.nbSlotRecharge = NBSLOTS;
	d.mutex_slotRecharge = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	d.cond_slotRecharge = (pthread_cond_t) PTHREAD_COND_INITIALIZER;	
	return d;
}

void destroyTout (Data *d)
{
	int i;

	for (i=0; i < NBCLIENT; i++)
	{
		pthread_mutex_destroy(&d->clients[i].mutex_client);
		pthread_cond_destroy(&d->clients[i].cond_client);
	}
	pthread_mutex_destroy(&d->mutex_docs);
	pthread_cond_destroy(&d->cond_docs);
}

void triColis (Data *d)
{
	int i, j, changement;
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
