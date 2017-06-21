#include "client.h"

void *clientThread(void *data)
{
	/*Initialisation des client*/
	Client *c = (Client*) data;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	gestionClient(c);
	pthread_exit(NULL);
}

/*fonction qui mets a jour l'état du client et verifie si 
tous ses colis ont ete livrer*/
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
