#include "drone.h"


void *droneThread(void *data)
{
	/*printf("Threadid : %ld\n", pthread_self());*/
	srand(time(NULL));
	Drone *drone = (Drone*) malloc(sizeof(Drone));
	drone->capaciteActuel = 90;
	drone->poidsMaximum = rand_min_max(0,3);
	/*µprintf("capa = %d, poid = %d, thread = %ld\n",drone->capaciteActuel,drone->poidsMaximum, pthread_self());
	si 0 => leger, 1 => moyen, 2 => lourd*/
	return NULL;
}

int rechercheColis(Data* data,Drone* drone)
{
	int id;
	int i,stop;
	if(drone.poidsMaximum == 0)
	{
		i = 0;
		stop = data.idMoyen;
	}
	else if
	{
		i = data.idMoyen;
		stop = data.idLourd;
	}
	else
	{
		i = data.idLourd;
		stop = NBCOLIS;
	}
	
	while(i < stop || data.colis[i] != 0)
	{
		i++;
	}
	if(i!=stop)
	{
		return i;
	}
	else
	{
		-1;
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

void/*int*/ prendreColis(Colis colis)
{
	/*int tempsD = CouloirX.distance * 0.5;
	int tempsRMax = CouloirX.distance * 1.5; 
	int temps = tempsD + tempsRMax;
	if(temps>CapaciteActuel)
	{
		return 0;
	}
	else
	{	
		colis.livrer = -1;
		return 1;
	}
	*/
}

void recharge()
{
	/*si(nbZoneRechargeLibre == 0)	
		zoneRechargeCond.wait
	Fsi
	
	nbZoneRechargeLibre --
	int tempRecharge = (CapaciteBattery -capaciteActuel)/1.5
	sleep(tempRecharge);
	 capaciteActuel = 90;	
	nbZoneRechargeLibre ++
	zoneRechargeCond.signal*/
	
}

void livre()
{
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



