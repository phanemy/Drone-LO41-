#include "zepplin.h"

int main(){
	Data data = initData();
	pthread_t drones[NBDRONE], clients[NBCLIENT];
	int i, id;

	/*affData (data);*/
	
	for (i=0; i < NBDRONE; i++)
	{
		pthread_create (&drones[i], NULL, droneThread, &data);
	}
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
	
	for (i=0; i < NBDRONE; i++)
	{
		pthread_join (drones[i], NULL);
	}
/*
	for (i=0; i < NBCLIENT; i++)
	{
		pthread_join (clients[i], NULL);
	}
	*/
	return 0;
}

void livreColis (Client *c) /*marche pas :( -> interblocage si client pas là*/
{
	int colisLivre = 0;
	while(!colisLivre)
	{
		pthread_mutex_lock (&c->mutex_client);
		if (c->present)
		{
			c->nbColis--;
			colisLivre = 1;
			printf("Colis livre pour le client %d\n", c->id);
		}
		else
		{
			printf("Le client %d pas la\n", c->id);
			pthread_mutex_unlock(&c->mutex_client);
			sleep(1);
		}
		pthread_mutex_unlock(&c->mutex_client);
	}
}

Data initData()
{
	Data d;
	int i, id;

	srand(time(NULL));

	for (i=0; i < NBCLIENT; i++)
	{
		d.clients[i].id = i;
		d.clients[i].couloir = rand_min_max(0,5);
		d.clients[i].dist = rand_min_max(2,31);
		d.clients[i].present = rand_min_max(0,11);
		d.clients[i].nbColis = 0;
		d.clients[i].dronePresent = 0;
		d.clients[i].mutex_client = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	}
	
	for (i=0; i < NBCOLIS; i++)
	{
		id = rand_min_max(0,NBCLIENT);
		d.leger[i].idClient = id;
		d.clients[id].nbColis++;
		d.leger[i].poids = rand_min_max(0,3);
		d.leger[i] = 0;
		
		id = rand_min_max(0,NBCLIENT);
		d.moyen[i].idClient = id;
		d.clients[id].nbColis++;
		d.moyen[i].poids = rand_min_max(2,6);
		d.moyen[i] = 0;
		
		id = rand_min_max(0,NBCLIENT);
		d.lourd[i].idClient = id;
		d.clients[id].nbColis++;
		d.lourd[i].poids = rand_min_max(5,11);
		d.lourd[i] = 0;
	}

	for (i=0; i < NBDOCS; i++)
	{
		d.mutex_docs[i] = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
		d.cond_docs[i] = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
	}
	return d;
}
