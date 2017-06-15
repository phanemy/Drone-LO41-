#include "drone.h"


void *droneThread(void *data)
{
	/*printf("Threadid : %ld\n", pthread_self());*/
	/*srand(time(pthread_self()));*/
	
	int idColis;
	Drone drone;
	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	initDrone(&drone);

	while(drone.poidsMaximum >= 0)
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		/*printf("pls");*/
		idColis = prendreColis(data, &drone);
		if(idColis == -1)
			recharge(data,&drone);
		else if (idColis == -2)
			drone.poidsMaximum--;
		else
			livreColis(data,idColis,&drone);
		/*printf("id colis = %d\n",idColis);*/
		/**/
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	}

	green("Fin thread drone\n");
	pthread_exit(NULL);
}

void initDrone(Drone* drone)
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
	sprintf(chaine, "\t\tpoidMax = %d\n",drone->poidsMaximum);
	cyan(chaine);
}

int rechercheColis(Data* data,Drone* drone)
{
	/*printf("rechercheColis\n");*/
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
		stop = NBCOLIS;
	}

	pthread_mutex_lock(&data->mutex_collis);

	while(i < stop && impossible )
	{
		/*printf("%d\n",i);*/
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

	if (plusCollis)
		i = -2;
	else if (i == stop)
		i = -1;


	pthread_mutex_unlock(&data->mutex_collis);

	return i;
}

int prendreColis(Data* data, Drone* drone)
{
	int idColis;
	attacheDock(data);
	/* utiliser recherche colis ici*/

	idColis = rechercheColis(data,drone);
	
	if(idColis >=0)
	{	
		pthread_mutex_lock(&data->mutex_collis);
			printf("idCOlis = %d\n",idColis);
			data->colis[idColis].livrer = -1;
		pthread_mutex_unlock(&data->mutex_collis);
	}
	/* mettre printf pour dire si colis prit ou non*/
	detacheDock(data);
	return idColis;
}

void attacheDock(Data* data)
{
	char chaine[50];
	pthread_mutex_lock(&data->mutex_docksAppro);
	/*printf("nb slot %d\tdrone = %ld\n", data->nbSlotRecharge, pthread_self());*/
		while (data->nbDocksAppro <= 0)/*expliquer pq while et pas if*/
		{
			/*printf("wait %d\t\tdrone : %ld\n", data->nbSlotRecharge, pthread_self());*/
			pthread_cond_wait(&data->cond_docksAppro, &data->mutex_docksAppro);
			/*printf("JE DEMARRE\n");*/
		}
		/*printf("\t\t\tnb slot avant -- = %d\n", data->nbSlotRecharge);*/
		data->nbDocksAppro--;
		printf("\t\t nb Dock = %d\n",data->nbDocksAppro);
		sprintf(chaine, "Le drone %ld se connecte au dock\n",pthread_self());
		yellow(chaine);
	pthread_mutex_unlock(&data->mutex_docksAppro);
}

void detacheDock(Data* data)
{
	char chaine[50];
	pthread_mutex_lock(&data->mutex_docksAppro);
		sprintf(chaine, "Le drone %ld se detache du dock\n",pthread_self());
		yellow(chaine);
		data->nbDocksAppro++;
		/*printf("\t\t\tnb slot apres ++ = %d\n", data->nbSlotRecharge);*/
		pthread_cond_signal(&data->cond_docksAppro);
	pthread_mutex_unlock(&data->mutex_docksAppro);
	/*printf("recharge fin\n------------------\n");*/
	
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
	char chaine[50];
	pthread_mutex_lock(&data->mutex_slotRecharge);
	/*printf("nb slot %d\tdrone = %ld\n", data->nbSlotRecharge, pthread_self());*/
		while (data->nbSlotRecharge <= 0)/*expliquer pq while et pas if*/
		{
			/*printf("wait %d\t\tdrone : %ld\n", data->nbSlotRecharge, pthread_self());*/
			pthread_cond_wait(&data->cond_slotRecharge, &data->mutex_slotRecharge);
			/*printf("JE DEMARRE\n");*/
		}
		/*printf("\t\t\tnb slot avant -- = %d\n", data->nbSlotRecharge);*/
		data->nbSlotRecharge--;
	
		sprintf(chaine, "Le drone %ld se recharge\n",pthread_self());
		yellow(chaine);
		
	pthread_mutex_unlock(&data->mutex_slotRecharge);
	return ((CAPACITEBATTERY - drone->capaciteActuel)/1.5);
}

void detacheSlotRecharge(Data* data,Drone* drone)
{
	char chaine[50];
	pthread_mutex_lock(&data->mutex_slotRecharge);
		drone->capaciteActuel = 90;
		sprintf(chaine, "Le drone %ld a fini de se recharge\n",pthread_self());
		yellow(chaine);
		data->nbSlotRecharge++;
		/*printf("\t\t\tnb slot apres ++ = %d\n", data->nbSlotRecharge);*/
		pthread_cond_signal(&data->cond_slotRecharge);
	pthread_mutex_unlock(&data->mutex_slotRecharge);
	/*printf("recharge fin\n------------------\n");*/
}

void livreColis(Data* data, int i,Drone* drone)
{
	/*printf("livreColis\n");*/
	/*////////////////////////////////////danger/////////////////////////////////////////*/
	Client* client = &data->clients[data->colis[i].idClient];
	int donne = donneColis(client);
	int temps;

	descente(client, drone);

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

	/*printf("livreColis fin\n------------------\n");*/
}

void descente(Client* client, Drone* drone)
{
	int temps;
	pthread_mutex_lock(&client->mutex_client);
		while(client->couloir[0] == 1)/* || client->dronePresent==1)*/
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		client->couloir[0] = 1;/* on descend*/
		temps = client->dist *  0.5;
	pthread_mutex_unlock(&client->mutex_client);
	
	sleep(temps * TIMESCALE);
	drone->capaciteActuel = drone->capaciteActuel - temps;
}

void remonte(Data* data, Client* client, Drone* drone,int temps,int i)
{
	pthread_mutex_lock(&client->mutex_client);
		client->couloir[0]=0;
		/*client->dronePresent=1;*/
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);

	pthread_mutex_lock(&client->mutex_client);
		while(client->couloir[1] == 1)
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		client->couloir[1] = 1;/* on descend*/
		/*client->dronePresent=0;*/
	pthread_mutex_unlock(&client->mutex_client);

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

int donneColis(Client *c)
{
	/*printf("donneColis\n");*/
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
			sprintf(chaine, "Colis livre pour le client %d\n", c->id);
			blue(chaine);
		}
		else
		{
			sprintf(chaine, "Le client %d pas la\n", c->id);
			red(chaine);
			pthread_mutex_unlock(&c->mutex_client);
			sleep(2 * TIMESCALE);
		}
		pthread_mutex_unlock(&c->mutex_client);
		i++;
	}
	if(i == 5)
	{
		/*printf("donneColis fin\n------------------\n");*/
		return 0;
	}
	else
	{
		/*printf("donneColis fin\n------------------\n");*/
		return 1;
	}

}
