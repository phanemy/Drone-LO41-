#include "client.h"

void *clientThread(void *data)
{
	Client *c = (Client*) data;
	gestionClient(c);
	printf("Le client %d a recu tout ses colis\n", c->id);
	return NULL;
}

void gestionClient (Client *c)
{
	pthread_mutex_lock (&c->mutex_client);
	while (c->nbColis > 0)
	{
		pthread_cond_wait(&c->cond_client, &c->mutex_client);
		c->present = rand_min_max(0,11);
	}
	pthread_mutex_unlock(&c->mutex_client);
}
