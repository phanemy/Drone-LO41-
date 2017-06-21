#include "drone.h"


void *droneThread(void *data)
{

	int idColis;
	Drone drone;
	Data *d = (Data *) data;
	
	/*permet d'arreter le thread de maniere asynchrone*/
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	
	/*initialisation du drone*/
	initDrone(&drone, d);
	
	/*le drone tourne tant qu'il peut livrer des colis*/	
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

/*fonction d'initialisation du drone*/
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

/* fonction qui recherche un colis a livrer retourne l'id du colis*/
int rechercheColis(Data* data,Drone* drone)
{
	int i,stop = 0;
	int impossible = 1, plusCollis = 1, idClient,tps;
	
	/*on recherche la plage sur laquelle doit s'effectuer la recherche de colis*/
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
	
	/*on parcours la liste des colis, si on en trouve un et qu'on a assez de battery pour le livrer
	onr retourne son id, si il le trouve mais qu'on a pas assez de battery on retroune -1, si on en trouve pas
	on retourn -2*/
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


/*on recupere le colis et retourne son id*/
int prendreColis(Data* data, Drone* drone)
{
	int idColis;
	/* pour le recuperer on doit s'attacher au doc de chargement*/
	attacheDock(data, drone);
	
	/*on le recherche*/
	idColis = rechercheColis(data,drone);
	
	/*si on le trouve on le prends*/
	if(idColis >=0)
	{	
		pthread_mutex_lock(&data->mutex_collis);
			data->colis[idColis].livrer = -1;
		pthread_mutex_unlock(&data->mutex_collis);
	}

	/*on se detache du dock*/
	detacheDock(data, drone);
	return idColis;
}


/*fonction qui permet de s'attacher au dock si il y en a un de libre*/
void attacheDock(Data* data, Drone* drone)
{
	pthread_mutex_lock(&data->mutex_docksAppro);
		while(data->nbDocksAppro <= 0)
		{
			pthread_cond_wait(&data->cond_docksAppro, &data->mutex_docksAppro);
		}
		data->nbDocksAppro--;
		afficherInfo(data, "Connection au dock", drone->id, data->nbDronesTot, 'y');
	pthread_mutex_unlock(&data->mutex_docksAppro);
}

/*fonction qui permet de liberer un dock utiliser*/
void detacheDock(Data* data, Drone* drone)
{
	pthread_mutex_lock(&data->mutex_docksAppro);
		afficherInfo(data, "Deconnection du dock", drone->id, data->nbDronesTot, 'y');
		data->nbDocksAppro++;
		pthread_cond_signal(&data->cond_docksAppro);
	pthread_mutex_unlock(&data->mutex_docksAppro);
}

/*fonction permettant de recharger le drone*/
void recharge(Data* data,Drone* drone)
{
	
	int tempRecharge;
	/* on s'attache et slot de recharge*/
	tempRecharge = attacheSlotRecharge(data, drone);

	/* on attend le temps de se recharger*/
	sleep(tempRecharge * TIMESCALE);

	/* on libere le slot*/
	detacheSlotRecharge(data,drone);
	
}

/*on s'attache au slot de recharge et on retourne le temps de recharge*/
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

/*on libere le slot de rechargement*/
void detacheSlotRecharge(Data* data,Drone* drone)
{
	pthread_mutex_lock(&data->mutex_slotRecharge);
		drone->capaciteActuel = 90;
		afficherInfo(data, "Fin recharge", drone->id, data->nbDronesTot, 'y');
		data->nbSlotRecharge++;
		pthread_cond_signal(&data->cond_slotRecharge);
	pthread_mutex_unlock(&data->mutex_slotRecharge);
}

/*fonction permettant de livrer un colis*/
void livreColis(Data* data, int i,Drone* drone)
{

	/*on recupere leclient pour pouvoir accéder a ces information*/
	Client* client = &data->clients[data->colis[i].idClient];
	int temps, donne;

	/*on descend jusqu'a chez lui*/
	descente(client, data, drone);

	/* on tente de donnez le colis si on y arrive donne = 1*/ 
	donne = donneColis(client, data, drone);

	if(donne)
	{	
		/*si on a réussi a le livre on passe le colis en livre (1)*/
		pthread_mutex_lock(&data->mutex_collis);
			data->colis[i].livrer = 1;
		pthread_mutex_unlock(&data->mutex_collis);

		pthread_mutex_lock(&client->mutex_client);	
			temps = client->dist;
		pthread_mutex_unlock(&client->mutex_client);

	}
	else
	{	
		/*sinon on doit remonter avec le colis donc on mets plus e temps*/
		pthread_mutex_lock(&client->mutex_client);
			temps = client->dist * 1.5; 
		pthread_mutex_unlock(&client->mutex_client);
	}
	
	/*on remonte jusqu'au zeppelin*/
	remonte(data,client,drone,temps,i);

}

/*fonction qui permet de descendre jusqu'au client*/
void descente(Client* client, Data* data, Drone* drone)
{
	int temps;
	char chaine[50];
	/*si il y a deja un drone en bas ou sur le couloir on attend*/
	pthread_mutex_lock(&client->mutex_client);
		while(client->couloir[0] == 1)
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		/*on prend le couloir*/
		client->couloir[0] = 1;
		temps = client->dist *  0.5;
	pthread_mutex_unlock(&client->mutex_client);

	sprintf(chaine, "Temps descente : %2.2f", temps * TIMESCALE);
	afficherInfo(data, chaine, drone->id, data->nbDronesTot, 'w');
	/*temps de la descente*/
	sleep(temps * TIMESCALE);
	drone->capaciteActuel = drone->capaciteActuel - temps;
}

/*oon remonte jusqu'au zeppelin si le couloir de remonter et libre*/
void remonte(Data* data, Client* client, Drone* drone,int temps,int i)
{
	char chaine[50];
	pthread_mutex_lock(&client->mutex_client);
		/*on libere le couloir de descente*/
		client->couloir[0]=0;
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);

	pthread_mutex_lock(&client->mutex_client);
		while(client->couloir[1] == 1)
		{
			pthread_cond_wait(&client->cond_client,&client->mutex_client);
		}
		/*on prend celui pour remonter*/
		client->couloir[1] = 1;
	pthread_mutex_unlock(&client->mutex_client);

	sprintf(chaine, "Temps monter : %2.2f", temps * TIMESCALE);
	afficherInfo(data, chaine, drone->id, data->nbDronesTot, 'w');
	sleep(temps * TIMESCALE);
	drone->capaciteActuel = drone->capaciteActuel - temps;

	/*on libere le couloir de remonter*/
	pthread_mutex_lock(&client->mutex_client);
		client->couloir[1]=0;
		pthread_cond_signal(&client->cond_client);
	pthread_mutex_unlock(&client->mutex_client);
	
	/*si le colis na pas était livrer on le remets en attente*/
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
	/*on test 5 fois maximum si le client est la*/
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
