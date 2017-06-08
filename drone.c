#include "drone.h"


void *droneThread(void *data)
{
	/*printf("Threadid : %ld\n", pthread_self());*/
	srand(time(NULL));
	Drone drone;
	drone.capaciteActuel = 90;
	drone.poidsMaximum = 0;/*rand_min_max(0,3);*/

	int idColis;
	int i =0;
	while(!toutFini(data) && i<100)
	{ 	
		/*printf("pls");*/
		idColis = rechercheColis(data,&drone);
		printf("idCOlis = %d\n",idColis);
		if(idColis == -1)
		{
			recharge(data,&drone);
		}
		else
		{
			livreColis(data,idColis,&drone);
		}
		/*printf("id colis = %d\n",idColis);*/
		/**/i++;
	}
	printf("Drone done\n");
	return NULL;
}

/* fonction qui decremente le poids focus par le drone si son ancien et totalement livrer.*/

int toutFini(Data* data)
{
	printf("toutfini\n");
	int i=0,bool=1;
	
	while(i < NBCOLIS && bool)
	{
		/*printf("%d  %d\n",i,NBCOLIS);*/
		if(data->colis[i].livrer == 0)
		{
			bool = 0;	
		}
		i++;
	}
	/*printf("bool = %d",bool);*/
printf("toutfini fin\n");
	return bool;
}

int rechercheColis(Data* data,Drone* drone)
{
	printf("rechercheCOlis\n");
	int id;
	int i,stop;
	if(drone->poidsMaximum == 0)
	{
		i = 0;
		stop = data->idMoyen;
	}
	else if(drone->poidsMaximum == 1)
	{
		i = data->idMoyen;
		stop = data->idLourd;
	}
	else
	{
		i = data->idLourd;
		stop = NBCOLIS;
	}
	
	int impossible = 1, idClient;

	while(i < stop && impossible)
	{	
		/*printf("%d\n",i);*/
		idClient = data->colis[i].idClient;
		if (data->colis[i].livrer == 0 && (data->clients[idClient].dist * 0.5 + data->clients[idClient].dist * 1.5) < drone->capaciteActuel)
			impossible = 0;
		else
			i++;
	}
	if(i!=stop)
	{	
	printf("rechercheCOlis fin\n");	
	return i;	
	}
	else
	{
printf("rechercheCOlis fin\n");	
		return -1;
	
	}
	/*if(drone->capaciteActuel == 0)
	{
		id = rechercheColisLeger(data);
	}
	if(drone->capaciteActuel == 1)
	{
		id = rechercheColisMoyen(data);
	}
	if(drone->capaciteActuel == 2)
	{
		id = rechercheColisLourd(data);
	}*/

}


void recharge(Data* data,Drone* drone)
{
	printf("recharge\n");
	pthread_mutex_lock(&data->mutex_slotRecharge);
	
		if(data->nbSlotRecharge == 0)	
		{	
			pthread_cond_wait(&data->cond_slotRecharge,&data->mutex_slotRecharge);
		}
		data->nbSlotRecharge--;	
	pthread_mutex_unlock(&data->mutex_slotRecharge);	
	
		printf("le drone %ld se recharge\n",pthread_self());
		int tempRecharge = (CAPACITEBATTERY - drone->capaciteActuel)/1.5;
		sleep(1);			
		/*sleep(tempRecharge);*/
		drone->capaciteActuel = 90;
		printf("le drone %ld a fini de se recharge\n",pthread_self());

	pthread_mutex_lock(&data->mutex_slotRecharge);
		data->nbSlotRecharge++;
		pthread_cond_signal(&data->cond_slotRecharge);
	pthread_mutex_unlock(&data->mutex_slotRecharge);
	printf("recharge fin\n");
}

void livreColis(Data* data, int i,Drone* drone)
{
	printf("livreCOlis\n");
	Client* client = &data->clients[data->colis[i].idClient];
	int temps;
	pthread_mutex_lock(&client->mutex_client);
		if(client->couloir[0] == 1)
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		client->couloir[0] = 1;/* on descend*/
		temps = client->dist *  0.5;
	pthread_mutex_unlock(&client->mutex_client);

	sleep(1);
	/*sleep(temps);*/
	drone->capaciteActuel = drone->capaciteActuel - temps;
	
	pthread_mutex_lock(&client->mutex_client);
		client->couloir[0]=0;
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);
	int donne = donneColis(client);
	printf("donne = %d\n",donne);
	if(donne)
	{	

		data->colis[i].livrer = 1;	
		pthread_mutex_lock(&client->mutex_client);
			temps = client->dist * 1; 
		pthread_mutex_unlock(&client->mutex_client);

	}
	else
	{	
		pthread_mutex_lock(&client->mutex_client);
			temps = client->dist * 1.5; 
		pthread_mutex_unlock(&client->mutex_client);
	}
	
	pthread_mutex_lock(&client->mutex_client);
		if(client->couloir[2] == 1)
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		client->couloir[2] = 1;/* on descend*/
	pthread_mutex_unlock(&client->mutex_client);
	sleep(1);
	/*sleep(temps);*/
	drone->capaciteActuel = drone->capaciteActuel - temps;


/* ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////

ON block  une fois qu'un client a fini desormais

///////////////////////////////
///////////////////////////////////////////
*/

	pthread_mutex_lock(&client->mutex_client);
		client->couloir[1]=0;
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);

	printf("livreCOlis fin\n");
}

int donneColis(Client *c)
{
	printf("donneCOlis\n");
	int colisLivre = 0;
	int i = 0;
	while(!colisLivre && i<5)
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
		i++;
	}
	if(i == 5)
	{printf("donneCOlis fin\n");
		return 0;
	}
	else
	{printf("donneCOlis fin\n");
		return 1;
	}

}




