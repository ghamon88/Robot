/*
 * Douchette.c
 *
 *  Created on: Dec 10, 2012
 *      Author: Guillaume
 */
#include "Douchette.h"
#include "ScanTranslation.h"
#include "GPIO.h"
#include <string.h>

extern char BiereDesiree[16];
int ThreadBras_Over=0;
extern void* ret;
char BiereTrouvee[16];

void InitDouchette(void) // ex gpio 30, prendre une vraie
{
	FILE *fd=NULL;
	export_GPIO(30,fd);
	set_direction_GPIO(30,fd,1);
	set_value_GPIO(30,fd,0);
}

void Activation(void) // à moins que le mode tout le tps actif soit enclenché
{
	FILE *fd=NULL;
	set_value_GPIO(30,fd,0);
	sleep(2);
	set_value_GPIO(30,fd,1);
}

int length(char* buffer){
	int i=0;
	while(buffer[i]!='/O'){
		i=i+1;
	}
	return i;
}
void *Scan(char* BiereDesiree)
{
	char commande[256]="./speech.sh Biaire trouver ";
	pthread_t ThreadBras;
	int j=1;
	int* loopcond=&j;
	memset(BiereTrouvee,'\0',16);
	//InitDouchette();
	if (pthread_create (&ThreadBras, NULL, Balayage_Bras, BiereDesiree) < 0) {
			    fprintf (stderr, "pthread_create error for threadBras\n");
			    exit (1);
			  }
	fprintf (stdout, "Avant while\n");
	while(strncmp(BiereTrouvee,BiereDesiree,7)!=0 && !ThreadBras_Over)
	{
		fprintf (stdout, "Dans while\n");
		//Activation(); //debug
		//readScanner(loopcond); //debug
		sleep(100);//debug
		strcat(commande,BiereTrouvee);
		system(commande);
		strcpy(commande,"./speech.sh Biaire trouver ");
		fprintf(stdout,"Biere trouvee %s\n",BiereTrouvee);
		fprintf(stdout,"Biere desiree %s\n",BiereDesiree);
	}
	fprintf (stdout, "Après while\n");
	if (ThreadBras_Over==1){ //on regarde si une bière a été trouvée
		fprintf(stdout,"Bière non trouvée dans le frigo");
		ThreadBras_Over=0;
	}else{
		fprintf(stdout,"Bière trouvée");
		ThreadBras_Over=0;
		pthread_cancel(ThreadBras);
		fprintf (stdout, "thread Bras off \n");
	}
	/* Pour l'instant on cherche juste à trouver la bière et stop
	(void)pthread_join (ThreadBras, &ret); // on attends que thread de balayage s'arrête
	*/
	 pthread_exit (0);
}

