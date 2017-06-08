#include "drone.h"


void *droneThread(void *data)
{
	/*printf("Threadid : %ld\n", pthread_self());*/
	srand(time(NULL));
	Drone *drone;
	drone->capaciteActuel = 90;
	drone->poidsMaximum = rand_min_max(0,3);
	/*printf("capa = %d, poids = %d, thread = %ld\n",drone->capaciteActuel,drone->poidsMaximum, pthread_self());
	si 0 => leger, 1 => moyen, 2 => lourd*/
	return NULL;
}

int rechercheColis(Data* data,Drone* drone)
{
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
		idClient = data->colis[i].idClient;
		if (data->colis[i].livrer == 0 && (data->clients[idClient].dist * 0.5 + data->clients[idClient].dist * 1.5) < drone->capaciteActuel)
			impossible = 0;
		else
			i++;
	}
	if(i!=stop)
		return i;

	else
		return -1;

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

/*	else if()
	else()*/
}

/*
int rechercheColisLeger(Data* data)
{	
	int i = 0;
	while(i < NBCOLIS || data.leger[i] != 0)
	{
		i++;
	}
	if(i!=NBCOLIS)
	{
		return i;
	}
	else
	{
		-1;
	}
}

void rechercheColisMoyen(Data* data)
{
	int i = 0;
	while(i < NBCOLIS || data.moyen[i] != 0)
	{
		i++;
	}
	if(i!=NBCOLIS)
	{
		return i;
	}
	else
	{
		-1;
	}
}

void rechercheColisLourd(Data* data)
{
	int i = 0;
	while(i < NBCOLIS || data.lourd[i] != 0)
	{
		i++;
	}
	if(i!=NBCOLIS)
	{
		return i;
	}
	else
	{
		-1;
	}

}*/

void recharge(Data* data,Drone* drone)
{
	pthread_mutex_lock(&data->mutex_slotRecharge);
	
		if(data->nbSlotRecharge == 0)	
		{	
			pthread_cond_wait(&data->cond_slotRecharge,&data->mutex_slotRecharge);
		}
		data->nbSlotRecharge--;	
	pthread_mutex_unlock(&data->mutex_slotRecharge);	
	
		printf("le drone %ld se recharge\n",pthread_self());
		int tempRecharge = (CAPACITEBATTERY - drone->capaciteActuel)/1.5;
		sleep(tempRecharge);
		drone->capaciteActuel = 90;
		printf("le drone %ld a fini de se recharge\n",pthread_self());

	pthread_mutex_lock(&data->mutex_slotRecharge);
		data->nbSlotRecharge++;
		pthread_cond_signal(&data->cond_slotRecharge);
	pthread_mutex_unlock(&data->mutex_slotRecharge);
}


void livreColis(Data* data, int i,Drone* drone)
{
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

	sleep(temps);
	drone->capaciteActuel = drone->capaciteActuel - temps;
	
	pthread_mutex_lock(&client->mutex_client);
		client->couloir[0]=0;
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);
	
	if(donneColis(client))
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
		pthread_mutex_lock(&client->mutex_client);
	}

	pthread_mutex_lock(&client->mutex_client);
		if(client->couloir[2] == 1)
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		client->couloir[2] = 1;/* on descend*/
	pthread_mutex_unlock(&client->mutex_client);

	sleep(temps);
	drone->capaciteActuel = drone->capaciteActuel - temps;


	pthread_mutex_lock(&client->mutex_client);
		client->couloir[1]=0;
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);

	/*
	On a deja le colis et verifier quon avais la battery
	si(CouloirX.descente == 0)
	{
		CouloirXDescente.wait;
	}
	CouloirXDescente --;
	int temps = CouloirX.distance * 0.5;
	sleep(temps);
	capacitéActuel = capaciteActuel - temps;
	if(clientX.present)
	{
		colisX.livrer=vrai;	
		temps = CouloirX.distance * 1; 
	}			
	else
	{
		temps = CouloirX.distance * 1.5; 
	}
	si(CouloirX.monte == 0)
	{
		CouloirXMonte.wait;
	}
	CouloirXDescente ++;
	COuloirXMOnte --;
	sleep(temps)
	CouloirXmonte ++
	capacitéActuel = capaciteActuel - temps;
	*/


}

int donneColis(Client *c)
{
	int colisLivre = 0;
	int i = 0;
	while(!colisLivre || i<5)
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
	if(i == 5)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}




