#include "zepplin.h"

int main(){
	Data data = initData();
	pthread_t drones[NBDRONE], clients[NBCLIENT];
	int i;

	/*affData (data);*/
	
	for (i=0; i < NBDRONE; i++)
	{
		pthread_create (&drones[i], NULL, droneThread, &data);
	}
	
	for (i=0; i < NBCLIENT; i++)
	{
		pthread_create (&clients[i], NULL, clientThread, &data.clients[i]);
	}
	
	for (i=0; i < NBDRONE; i++)
	{
		pthread_join (drones[i], NULL);
	}

	for (i=0; i < NBCLIENT; i++)
	{
		pthread_join (clients[i], NULL);
	}
	
	return 0;
}

Data initData()
{
	Data d;
	int i;

	srand(time(NULL));

	for (i=0; i < NBCLIENT; i++)
	{
		d.clients[i].id = i;
		d.clients[i].couloir = rand_min_max(0,5);
		d.clients[i].dist = rand_min_max(2,31);
		d.clients[i].present = rand_min_max(0,11);
	}
	
	for (i=0; i < NBCOLIS; i++)
	{	/*0<leger<2<3<moyen<5<6<lourd*/
		d.leger[i].idClient = rand_min_max(0,NBCLIENT);
		d.leger[i].poids = rand_min_max(0,3);
		d.moyen[i].idClient = rand_min_max(0,NBCLIENT);
		d.moyen[i].poids = rand_min_max(2,6);
		d.lourd[i].idClient = rand_min_max(0,NBCLIENT);
		d.lourd[i].poids = rand_min_max(5,11);
	}
	return d;
}
