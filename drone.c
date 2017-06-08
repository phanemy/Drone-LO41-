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

void rechercheColis(Data* data,Drone* drone)
{
	if(drone->capaciteActuel == 0)
	{
		rechercheColisLeger(data);
	}
/*	else if()
	else()*/
}

void rechercheColisLeger(Data* data)
{	
	int i = 0;
	for(i = 0; i < NBCOLIS; i++)
	{
		data.leger[i]
	}
}

void rechercheColisMoyen(Data* data)
{
	/*verif colis de poids voulu dispo et doc de livraison libre
	
	*/
}

void rechercheColisLourd(Data* data)
{
	/*verif colis de poids voulu dispo et doc de livraison libre
	
	*/
}

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



