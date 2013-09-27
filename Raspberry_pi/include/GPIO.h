/*
 * GPIO.h
 *
 *  Created on: Dec 10, 2012
 *      Author: chouf
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "I2C.h"

int trigger_LED(FILE *nom_f);
int brightness_LED(FILE *nom_f, int value);
int export_GPIO(int, FILE *);
int set_direction_GPIO(int, FILE *, int);
int set_value_GPIO(int, FILE *, int);
void *Heartbeat(void);

#endif /* GPIO_H_ */
