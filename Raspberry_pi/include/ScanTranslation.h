/*
 * ScanTranslation.h
 *
 *  Created on: Dec 30, 2012
 *      Author: chouf
 */

#ifndef SCANTRANSLATION_H_
#define SCANTRANSLATION_H_

#include <stdio.h>
#include <string.h>
//#include <dirent.h>
//#include <fcntl.h>
//#include <unistd.h>
#include <linux/input.h>

#define VENDORID  0x5e0 // à mettre à jour
#define PRODUCTID 0x200 //idem
#define SCN_BCD_SZ 30

//int scan_fd;

int initScanner(void);
int closeScanner(void);
char keycodelist(int);
char* readScanner(int*);




#endif /* SCANTRANSLATION_H_ */
