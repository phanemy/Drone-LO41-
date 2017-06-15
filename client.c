#include "client.h"

void *clientThread(void *data)
{
	char chaine[50];

	Client *c = (Client*) data;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
		
	gestionClient(c);
	sprintf(chaine, "Le client %d a recu tout ses colis\n", c->id);
	green(chaine);
	/*printf("Le client %d a recu tout ses colis\n", c->id);
	printf("blabla\n");*/
	pthread_exit(NULL);
}

void gestionClient (Client *c)
{
	while (c->nbColis > 0)
	{
		pthread_mutex_lock (&c->mutex_client);
		c->present = rand_min_max(0,11);
		pthread_mutex_unlock(&c->mutex_client);
		sleep(6 * TIMESCALE);
	}
}
