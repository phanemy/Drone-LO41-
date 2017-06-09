#include "drone.h"


void *droneThread(void *data)
{
	/*printf("Threadid : %ld\n", pthread_self());*/
	/*srand(time(pthread_self()));*/
	Drone drone;
	drone.capaciteActuel = 90;
	drone.poidsMaximum = rand_min_max(0,100);
	char chaine[50];
	if(drone.poidsMaximum > 60)
	{
		drone.poidsMaximum = 2;
	}
	else if(drone.poidsMaximum >25 )
	{
		drone.poidsMaximum = 1;
	}
	else
	{
		drone.poidsMaximum = 0;
	}
	sprintf(chaine, "\t\tpoidMax = %d\n",drone.poidsMaximum);
	cyan(chaine);
	int idColis;
	int i =0;
	while(drone.poidsMaximum >= 0)
	{ 	
		/*printf("pls");*/
		idColis = rechercheColis(data,&drone);
		printf("idCOlis = %d\n",idColis);
		if(idColis == -1)
			recharge(data,&drone);
		else if (idColis == -2)
			drone.poidsMaximum--;
		else
			livreColis(data,idColis,&drone);
		/*printf("id colis = %d\n",idColis);*/
		/**/
	}
	green("Fin thread drone\n");
	return NULL;
}

/* fonction qui decremente le poids focus par le drone si son ancien et totalement livrer.*/

int toutFini(Data* data)
{
	/*printf("toutfini\n");*/
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
	/*printf("bool = %d",bool);
	printf("toutfini fin\n------------------\n");*/
	return bool;
}

int rechercheColis(Data* data,Drone* drone)
{
	/*printf("rechercheColis\n");*/
	int id;
	int i,stop = 0;
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
	
	int impossible = 1, plusCollis = 1, idClient,tps;

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
	else
		data->colis[i].livrer = -1;

	pthread_mutex_unlock(&data->mutex_collis);

	return i;

	/*printf(" i= %d,stop = %d,livre = %d, impossible = %d, plus collis = %d\n",i,stop,data->colis[i].livrer,impossible,plusCollis);*/
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
	/*printf("recharge\n");*/
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
		int tempRecharge = (CAPACITEBATTERY - drone->capaciteActuel)/1.5;
	pthread_mutex_unlock(&data->mutex_slotRecharge);

	sleep(1);
	/*sleep(tempRecharge);*/

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
	/*printf("donne = %d\n",donne);*/
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
		if(client->couloir[1] == 1)
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		client->couloir[1] = 1;/* on descend*/
	pthread_mutex_unlock(&client->mutex_client);
	sleep(1);
	/*sleep(temps);*/
	drone->capaciteActuel = drone->capaciteActuel - temps;

	pthread_mutex_lock(&client->mutex_client);
		client->couloir[1]=0;
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);

	/*printf("livreColis fin\n------------------\n");*/
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
			sleep(1);
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




