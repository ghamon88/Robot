#include <pthread.h>
#include <math.h>
#include "Bras.h"
#include "I2C.h"

extern char BiereDesiree[16];
extern int ThreadBras_Over;
float resultat[5];


/*thread qui envoi les coordonn�es du bras � atteindre pour un balayage
 points de coordonn�es possibles: sph�re de rayon 200mm � 600mm
 y=230mm, x varie de -500 � 500 mm et z de 100 � 550mm
 */
void *Balayage_Bras(char* BiereDesiree) 
		{

	float *angles[5];
	float x;
	float y;
	float z;
	y=230;
	fprintf (stdout, "thread Bras On \n");
	for (z=100;z<=550;z=z+50){
		for (x=-500;x<=500;x=x+10){
			*angles=Correspondance_Angles(x,y,z);
			Envoi_Consignes_I2C(*angles);
			sleep(1);

		}
	}

	fprintf (stdout, "thread Bras off \n");
	ThreadBras_Over=1;
	pthread_exit (0);
}

void Envoi_Consignes_I2C(float* angles)
	// générer les angles entre 0 et 180 et non -90/90 avant cette fonction
	int Theta1,Theta2,Theta3,Theta4;
	//theta1 rotation plateau (MG995), theta2 1er segment (2x GWS777), theta3 2e segement (3e GWS777), theta4 (MG995) dernier segment (pince)
	Theta1=angles[0]+90; 
	Theta2=angles[1];
	Theta3=angles[2]+90;
	Theta4=angles[3]+90;
	if(Theta1>180 || Theta1<0)
	{
		printf("Problème Theta1 = %d",(int)Theta1);
		i2cset(0x22,0x10);
		sleep(1);
	}else{
		i2cset(0x22,4*(int)(Theta1/4));
		sleep(1);
		printf("Theta1 = %d",4*(int)(Theta1/4));
	}
	if(Theta2>90 || Theta2<0)
		{
			printf("Problème Theta2 = %d",(int)Theta2);
		}else{
			i2cset(0x2a,4*(int)(Theta2/4));
			printf("Theta2 = %d",4*(int)(Theta2/4));
			sleep(1);
		}
	if(Theta3<0 || Theta3>180)
			{
				printf("Problème Theta3 = %d",(int)Theta3);
			}else{
				//i2cset(0x2a,(4*(int)(Theta3/4))+1);
				printf("Theta3 = %d",4*(int)(Theta3/4)+1);
			}
	if(Theta4<0 || Theta4>180)
				{
					printf("Problème Theta4 = %d",(int)Theta4);
				}else{
					i2cset(0x22,(4*(int)(Theta4/4))+2);
					printf("Theta4 = %d",4*(int)(Theta4/4)+2);
					sleep(1);
				}
	i2cset(0x22,0x23); // rotation de la pince
	sleep(1);
	sleep(0.25); //debug
	return;
}

// coordonn�es x,y,z en angles moteurs
float* Correspondance_Angles(float x,float y,float z)
{
	// Initialisations

	float pi=3.14;
	float theta1,theta2,theta3,theta4,ErreurPos,Px,Py,Pz;
	int d1=93; // hauteur initiale plateau
	int L2=230; // 1er segment
	int L3=215; // second segment
	int L4=185; //dernier segment + pince
	// Recherche du meilleur theta2
	int j=0;
	int i,h;
	float Erreur[90];
	for (i=90;i>=1;i--){
		Erreur[j]=abs((x*x+y*y-(L4+L2*cos((double)(i*3.14/180))+L3*cos((double)(asin((z-d1-L2*sin(i*3.14/180))/L3))))*(L4+L2*cos((double)(i*3.14/180))+L3*cos((double) (asin((z-d1-L2*sin(i*3.14/180))/L3))))));
		j=j+1;
	}
	for(i=0;i<=89;i++){
		if (i==0){
			h=0;
		}else{
			if (Erreur[i]<=Erreur[h]){
				h=i;
			}
		}
	}
	h=h+1;
	// Calculs des theta
	theta2=91-h;
	theta3=(asin((z-d1-L2*sin(theta2*3.14/180))/L3)-theta2*3.14/180)*180/3.14;
	theta4=-(theta3+theta2);

	theta1=atan(y/x)*180/pi;
	Px=cos(theta1*pi/180)*(L2*cos(theta2*pi/180)+L3*cos((theta2+theta3)*pi/180)+L4);
	Py=sin(theta1*pi/180)*(L2*cos(theta2*pi/180)+L3*cos((theta2+theta3)*pi/180)+L4);
	Pz=L3*sin((theta3+theta2)*pi/180)+L2*sin(theta2*pi/180)+d1;
	ErreurPos=sqrt((abs(Px-x)+abs(Py-y)+abs(Pz-z))*(abs(Px-x)+abs(Py-y)+abs(Pz-z)));

	if (x<=0 && y>=0){
		theta1=atan(y/x)*180/pi+90;
	}else{
		if (x>0 && y>=0){
			theta1=atan(y/x)*180/pi-90;
		}
		if (x<=0 && y<0){
			theta1=atan(y/x)*180/pi+90;
		}
		if (x>0 && y<0){
			theta1=atan(y/x)*180/pi-90;
		}
	}
	resultat[0]=theta1;
	resultat[1]=theta2;
	resultat[2]=theta3;
	resultat[3]=theta4;
	resultat[4]=ErreurPos;

	return resultat;
}
