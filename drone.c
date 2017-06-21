#include "drone.h"


void *droneThread(void *data)
{
	
	int idColis;
	Drone drone;
	Data *d = (Data *) data;
	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	initDrone(&drone, d);

	while(drone.poidsMaximum >= 0)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		idColis = prendreColis(data, &drone);
		if(idColis == -1)
			recharge(d,&drone);
		else if (idColis == -2)
			drone.poidsMaximum--;
		else
			livreColis(d,idColis,&drone);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	}

	afficherInfo(d, "Fin thread drone", drone.id, d->nbDronesTot, 'g');
	pthread_exit(NULL);
}

void initDrone(Drone* drone, Data* data)
{
	char chaine[50];
	drone->capaciteActuel = 90;
	drone->poidsMaximum = rand_min_max(0,100);
	if(drone->poidsMaximum > 60)
	{
		drone->poidsMaximum = 2;
	}
	else if(drone->poidsMaximum >25 )
	{
		drone->poidsMaximum = 1;
	}
	else
	{
		drone->poidsMaximum = 0;
	}

	pthread_mutex_lock(&data->mutex_collis);

	drone->id = data->numDrone;
	data->numDrone++;

	pthread_mutex_unlock(&data->mutex_collis);

	sprintf(chaine, "poidMax : %d",drone->poidsMaximum);
	afficherInfo(data, chaine, drone->id, data->nbDronesTot, 'c');
}

int rechercheColis(Data* data,Drone* drone)
{
	int i,stop = 0;
	int impossible = 1, plusCollis = 1, idClient,tps;
	
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
		stop = data->nbColisTot;
	}

	pthread_mutex_lock(&data->mutex_collis);

	while(i < stop && impossible )
	{
		idClient = data->colis[i].idClient;
		tps =(data->clients[idClient].dist * 0.5 + data->clients[idClient].dist * 1.5);
		if(data->colis[i].livrer == 0 )
		{
			plusCollis = 0;
		}

		if (data->colis[i].livrer == 0 && tps<drone->capaciteActuel)
		{		
			impossible = 0;
		}
		else
			i++;
	}

	pthread_mutex_unlock(&data->mutex_collis);

	if (plusCollis)
		i = -2;
	else if (i == stop)
		i = -1;

	return i;
}

int prendreColis(Data* data, Drone* drone)
{
	int idColis;
	attacheDock(data, drone);

	idColis = rechercheColis(data,drone);
	
	if(idColis >=0)
	{	
		pthread_mutex_lock(&data->mutex_collis);
			data->colis[idColis].livrer = -1;
		pthread_mutex_unlock(&data->mutex_collis);
	}
	detacheDock(data, drone);
	return idColis;
}

void attacheDock(Data* data, Drone* drone)
{
	pthread_mutex_lock(&data->mutex_docksAppro);
		while (data->nbDocksAppro <= 0)
		{
			pthread_cond_wait(&data->cond_docksAppro, &data->mutex_docksAppro);
		}
		data->nbDocksAppro--;
		afficherInfo(data, "Connection au dock", drone->id, data->nbDronesTot, 'y');
	pthread_mutex_unlock(&data->mutex_docksAppro);
}

void detacheDock(Data* data, Drone* drone)
{
	pthread_mutex_lock(&data->mutex_docksAppro);
		afficherInfo(data, "Deconnection du dock", drone->id, data->nbDronesTot, 'y');
		data->nbDocksAppro++;
		pthread_cond_signal(&data->cond_docksAppro);
	pthread_mutex_unlock(&data->mutex_docksAppro);
}

void recharge(Data* data,Drone* drone)
{
	
	int tempRecharge;
	tempRecharge = attacheSlotRecharge(data, drone);

	sleep(tempRecharge * TIMESCALE);

	detacheSlotRecharge(data,drone);
	
}

int attacheSlotRecharge(Data* data, Drone* drone)
{
	pthread_mutex_lock(&data->mutex_slotRecharge);
		while (data->nbSlotRecharge <= 0)
		{
			pthread_cond_wait(&data->cond_slotRecharge, &data->mutex_slotRecharge);
		}
		data->nbSlotRecharge--;
		afficherInfo(data, "Se recharge", drone->id, data->nbDronesTot, 'y');		
	pthread_mutex_unlock(&data->mutex_slotRecharge);
	return ((CAPACITEBATTERY - drone->capaciteActuel)/1.5);
}

void detacheSlotRecharge(Data* data,Drone* drone)
{
	pthread_mutex_lock(&data->mutex_slotRecharge);
		drone->capaciteActuel = 90;
		afficherInfo(data, "Fin recharge", drone->id, data->nbDronesTot, 'y');
		data->nbSlotRecharge++;
		pthread_cond_signal(&data->cond_slotRecharge);
	pthread_mutex_unlock(&data->mutex_slotRecharge);
}

void livreColis(Data* data, int i,Drone* drone)
{
	Client* client = &data->clients[data->colis[i].idClient];
	int temps, donne;

	descente(client, data, drone);

	donne = donneColis(client, data, drone);

	if(donne)
	{
		pthread_mutex_lock(&data->mutex_collis);
			data->colis[i].livrer = 1;
		pthread_mutex_unlock(&data->mutex_collis);

		pthread_mutex_lock(&client->mutex_client);	
			temps = client->dist;
		pthread_mutex_unlock(&client->mutex_client);

	}
	else
	{	
		pthread_mutex_lock(&client->mutex_client);
			temps = client->dist * 1.5; 
		pthread_mutex_unlock(&client->mutex_client);
	}
	
	remonte(data,client,drone,temps,i);

}

void descente(Client* client, Data* data, Drone* drone)
{
	int temps;
	char chaine[50];
	pthread_mutex_lock(&client->mutex_client);
		while(client->couloir[0] == 1)
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		client->couloir[0] = 1;
		temps = client->dist *  0.5;
	pthread_mutex_unlock(&client->mutex_client);
	sprintf(chaine, "Temps descente : %2.2f", temps * TIMESCALE);
	afficherInfo(data, chaine, drone->id, data->nbDronesTot, 'w');
	sleep(temps * TIMESCALE);
	drone->capaciteActuel = drone->capaciteActuel - temps;
}

void remonte(Data* data, Client* client, Drone* drone,int temps,int i)
{
	char chaine[50];
	pthread_mutex_lock(&client->mutex_client);
		client->couloir[0]=0;
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);

	pthread_mutex_lock(&client->mutex_client);
		while(client->couloir[1] == 1)
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		client->couloir[1] = 1;
	pthread_mutex_unlock(&client->mutex_client);

	sprintf(chaine, "Temps monter : %2.2f", temps * TIMESCALE);
	afficherInfo(data, chaine, drone->id, data->nbDronesTot, 'w');
	sleep(temps * TIMESCALE);
	drone->capaciteActuel = drone->capaciteActuel - temps;

	pthread_mutex_lock(&client->mutex_client);
		client->couloir[1]=0;
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);
	
	pthread_mutex_lock(&data->mutex_collis);
		if(data->colis[i].livrer == -1)	
			data->colis[i].livrer = 0;
	pthread_mutex_unlock(&data->mutex_collis);
	
}

int donneColis(Client *c, Data* d, Drone* drone)
{
	int colisLivre = 0;
	int i = 0;
	char chaine[50];
	while(!colisLivre && i<5)
	{
		pthread_mutex_lock (&c->mutex_client);
		if (c->present)
		{
			c->nbColis--;
			colisLivre = 1;
			sprintf(chaine, "Client %d livre", c->id);
			afficherInfo(d, chaine, drone->id, d->nbDronesTot, 'b');
		}
		else
		{
			sprintf(chaine, "Client %d absent", c->id);
			afficherInfo(d, chaine, drone->id, d->nbDronesTot, 'r');
			pthread_mutex_unlock(&c->mutex_client);
			sleep(4 * TIMESCALE);
		}
		pthread_mutex_unlock(&c->mutex_client);
		i++;
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

void afficherInfo(Data *d, char *chaine, int index, int nbDrone, char color)
{
	pthread_mutex_lock (&d->mutex_affichage);
		afficherTableau(chaine, index, nbDrone, color);
	pthread_mutex_unlock(&d->mutex_affichage);
}
