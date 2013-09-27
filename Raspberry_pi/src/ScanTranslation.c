#include "ScanTranslation.h"

extern char BiereTrouvee[16];
FILE *scan_fd=NULL;
int maj=0;

int initScanner(){
	scan_fd = fopen("/dev/input/event0","r+");
	return 1;
}

int closeScanner(){
	fclose(scan_fd);
	scan_fd = NULL;
	return 1;
}


char keycodelist(int scancode){
	char ret = '-';
	//return (unsigned char)scancode;
	if (maj==0){
		switch(scancode){
		case 0x02: ret ='&';break;
		//case 0x03: ret ='é';break;
		case 0x04: ret ='"';break;
		//case 0x05: ret ='4';break;
		case 0x06: ret ='(';break;
		case 0x07: ret ='-';break;
		//case 0x08: ret ='è';break;
		case 0x09: ret ='_';break;
		//case 0x0a: ret ='ç';break;
		//case 0x0b: ret ='à';break;
		case 0x0c: ret =')';break;

		case 0x10: ret ='a';break;
		case 0x11: ret ='z';break;
		case 0x12: ret ='e';break;
		case 0x13: ret ='r';break;
		case 0x14: ret ='t';break;
		case 0x15: ret ='y';break;
		case 0x16: ret ='u';break;
		case 0x17: ret ='i';break;
		case 0x18: ret ='o';break;
		case 0x19: ret ='p';break;

		case 0x1e: ret ='q';break;
		case 0x1f: ret ='s';break;
		case 0x20: ret ='d';break;
		case 0x21: ret ='f';break;
		case 0x22: ret ='g';break;
		case 0x23: ret ='h';break;
		case 0x24: ret ='j';break;
		case 0x25: ret ='k';break;
		case 0x26: ret ='l';break;
		case 0x27: ret ='m';break;

		case 0x2c: ret ='w';break;
		case 0x2d: ret ='x';break;
		case 0x2e: ret ='c';break;
		case 0x2f: ret ='v';break;
		case 0x30: ret ='b';break;
		case 0x31: ret ='n';break;
		case 0x32: ret =',';break;
		case 0x39: ret =' ';break;

		default: break;
		}
	}else{
		maj=0;
		switch(scancode){
		case 0x02: ret ='1';break;
		case 0x03: ret ='2';break;
		case 0x04: ret ='3';break;
		case 0x05: ret ='4';break;
		case 0x06: ret ='5';break;
		case 0x07: ret ='6';break;
		case 0x08: ret ='7';break;
		case 0x09: ret ='8';break;
		case 0x0a: ret ='9';break;
		case 0x0b: ret ='0';break;
		// case 0x0c: ret ='°';break;

		case 0x10: ret ='A';break;
		case 0x11: ret ='Z';break;
		case 0x12: ret ='E';break;
		case 0x13: ret ='R';break;
		case 0x14: ret ='T';break;
		case 0x15: ret ='Y';break;
		case 0x16: ret ='U';break;
		case 0x17: ret ='I';break;
		case 0x18: ret ='O';break;
		case 0x19: ret ='P';break;

		case 0x1e: ret ='Q';break;
		case 0x1f: ret ='S';break;
		case 0x20: ret ='D';break;
		case 0x21: ret ='F';break;
		case 0x22: ret ='G';break;
		case 0x23: ret ='H';break;
		case 0x24: ret ='J';break;
		case 0x25: ret ='K';break;
		case 0x26: ret ='L';break;
		case 0x27: ret ='M';break;

		case 0x2c: ret ='W';break;
		case 0x2d: ret ='X';break;
		case 0x2e: ret ='C';break;
		case 0x2f: ret ='V';break;
		case 0x30: ret ='B';break;
		case 0x31: ret ='N';break;
		case 0x32: ret ='?';break;
		}
	}
	/*if (ret=='-'){
		printf("ret code %d\n",scancode);
	}*/
	return ret;
}


//lit un code barre
//lit tant que *loopcond!=0 
//lit tous les caractères d'un code barre jusqu'au code 0x28 (retour charriot)
char* readScanner(int *loopcond){
	char barcode[16];
	char code[16];
	int i=0;
	struct input_event ev;

	while( loopcond==NULL?1:*loopcond ){
		fread(&ev,sizeof(struct input_event),1,scan_fd);
		if( ev.type==1 && ev.value==1 ){
			if( ev.code==0x60 ){ //retour charriot
				code[i] = 0;
				strcpy(barcode,code);
				strcpy(BiereTrouvee,code);
				printf(" %s\n",code);
				return barcode;
			}
			else{
				if( ev.code!=0 ){
					if (ev.code!=0x2a){
						code[i++] = keycodelist(ev.code);
						if( i==SCN_BCD_SZ-1 ){ printf("Buffer lecteur de code barres plein %s\n",code); return NULL;}
					}else{
						maj=1;
					}
				}
			}
		}
	}
	return NULL;
}
