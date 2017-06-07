#include "client.h"

void *clientThread(void *data)
{
	Client *c = (Client*) data;
	int i;
	affClient(*c);
	if (c->nbColis > 0)
	{
		for (i=0; i < 5; i++)
		{
			sleep(rand_min_max(0,4));
			c->present = rand_min_max(0,11);
			affClient(*c);
		}
	}
	return NULL;
}
