/*
 * Douchette.h
 *
 *  Created on: Dec 10, 2012
 *      Author: chouf
 */

#ifndef DOUCHETTE_H_
#define DOUCHETTE_H_

#include <pthread.h>
#include <math.h>
#include "I2C.h"
#include "GPIO.h"
#include "Bras.h"

char BiereDesiree[16];

void InitDouchette(void);
void Activation(void);
void *Scan(char*);

#endif /* DOUCHETTE_H_ */
