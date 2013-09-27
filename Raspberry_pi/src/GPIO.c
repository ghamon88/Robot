/*
 * GPIO.c
 *
 *  Created on: Dec 10, 2012
 *      Author: Guillaume
 */
#include "GPIO.h"

int export_GPIO(int numero, FILE *nom_f){
	if ((nom_f = fopen("/sys/class/gpio/export", "w")) == NULL)
			    {
			        fprintf(stdout,"Error whilst opening /sys/class/gpio/export \n");
			        return -1;
			    }
			fprintf(nom_f,"%d",numero);
			fclose(nom_f);
			return 0;
}

int trigger_LED(FILE *nom_f){
	if ((nom_f = fopen("/sys/class/leds/led0/trigger", "w")) == NULL)
			    {
			        fprintf(stdout,"Error whilst opening /sys/class/leds/led0/trigger \n");
			        return -1;
			    }
			fprintf(nom_f,"none");
			fclose(nom_f);
			return 0;
}

int brightness_LED(FILE *nom_f, int value){
	if ((nom_f = fopen("/sys/class/leds/led0/brightness", "w")) == NULL)
			    {
			        fprintf(stdout,"Error whilst opening /sys/class/leds/led0/brightness \n");
			        return -1;
			    }
			fprintf(nom_f,"%d",value);
			fclose(nom_f);
			return 0;
}

int set_direction_GPIO(int numero, FILE *nom_f, int direction){
	// direction=0 --> in, direction=1 --> out
	char tab[256];
	sprintf(tab,"/sys/class/gpio/gpio%d/direction", numero);
	if ((nom_f = fopen(tab, "w")) == NULL)
			    {
			        fprintf(stdout,"Error whilst opening /sys/class/gpio/gpio%d/direction \n",numero);
			        return -1;
			    }
	if (direction==0){
			fputs("in",nom_f);
	}else {
				if (direction==1){
					fputs("out", nom_f);
				}
				else {
					fprintf(stdout,"error direction must be either 0 or 1 \n");
				}
			}
			fclose(nom_f);
			return 0;
}

int set_value_GPIO(int numero, FILE *nom_f, int value){
	char tab[256];
	sprintf(tab,"/sys/class/gpio/gpio%d/value", numero);
	if ((nom_f = fopen(tab, "w"))==NULL)
			    {
			        fprintf(stdout,"Error whilst opening /sys/class/gpio/gpio%d/value \n", numero);
			        return -1;
			    }
			fprintf(nom_f,"%d",value);
			fclose(nom_f);
			if (value==0){
				fprintf(stdout,"GPIO%d OFF \n", numero);
			}else{
					if (value==1){
						fprintf(stdout,"GPIO%d ON \n", numero);
					}else{
						fprintf(stdout,"value must be either 0 or 1 \n");
					}
				}
			return 0;

}

void *Heartbeat(void)
{
	FILE *fd=NULL;
	trigger_LED(fd);
	while(1)
	{
		brightness_LED(fd, 0);
		sleep(0.5);
		brightness_LED(fd, 1);
		sleep(0.5);
	}
}
