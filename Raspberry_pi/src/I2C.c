
// Commencer par charger le module i2c-dev dans script d'init rC.c

#include "I2C.h"

void i2cget(int addr, int octet)
{
	char commandeI2C[20]="i2cget -y 1 ";
	char adresseSlave[5];
	sprintf(adresseSlave,"%d",addr);
	strcat(commandeI2C,adresseSlave);
	system(commandeI2C);
}

void i2cset(int addr, int octet) //0x1A motoréducteurs, 0x22 servomoteurs
{
	char commandeI2C[20]="i2cset -y 1 ";
	char adresseSlave[4];
	char DataToSend[3];
	sprintf(adresseSlave,"%d ",addr);
	sprintf(DataToSend,"%d",octet);
	strcat(commandeI2C,adresseSlave);
	strcat(commandeI2C,DataToSend);
	system(commandeI2C);
}


