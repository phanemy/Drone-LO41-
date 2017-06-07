#include "client.h"

void *clientThread(void *data)
{
	Client *c = (Client*) data;
	affClient(*c);
	return NULL;
}
