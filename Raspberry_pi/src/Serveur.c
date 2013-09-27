/*
 ============================================================================
 Name        : ClignotementLED
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Serveur pandaboard
 ============================================================================
 */
#include "ScanTranslation.h"
#include "I2C.h"
#include "GPIO.h"
#include "Douchette.h"
#include <pthread.h>

int cree_socket_stream (struct in_addr ip4, short int port, const char * nom_proto);
int affiche_adresse_socket(int sock);
int serveur_tcp(void);
int quitter_le_serveur(void);
void traite_connexion(int sock);

extern FILE *scan_fd;


int main(int argc, char * argv[]) {
	pthread_t ThreadScan, HeartBeat;
	void* ret;
	pid_t fork(void);

	// heartbeat
	if (pthread_create (&HeartBeat, NULL, Heartbeat, NULL) < 0) {
				fprintf (stderr, "pthread_create error for Heartbeat\n");
				exit (1);
		}
	fprintf (stdout, "InitScanner\n");
	initScanner();
	
	while(1){
		serveur_tcp();
	}
	return 0;
}

int cree_socket_stream (struct in_addr ip4, short int port, const char * nom_proto){
	int sock;
	struct sockaddr_in adresse;
	struct hostent * hostent;
	struct servent * servent;
	struct protoent * protoent;

	if((hostent=gethostbyaddr((char*) & ip4,sizeof(struct in_addr), AF_INET))==0){
		perror("gethostbyaddr");
		return -1;
	}
	if((protoent=getprotobyname(nom_proto))==NULL){
		perror("getprotobyname");
		return -1;
	}

	if((servent=getservbyport(htons(port), protoent->p_name))==NULL){
		perror("getservbyport");
		return -1;
	}

	if((sock=socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("socket");
		return -1;
	}
	memset(& adresse, 0, sizeof(struct sockaddr_in));
	adresse.sin_family=AF_INET;
	adresse.sin_port=servent->s_port;
	adresse.sin_addr.s_addr=((struct in_addr *)(hostent->h_addr))->s_addr;
	if(bind(sock, (struct sockaddr *) & adresse, sizeof(struct sockaddr_in))<0){
		close(sock);
		//perror("bind");
		return -1;
	}
	return sock;
}


int quitter_le_serveur(void){
	return 0;
}

int affiche_adresse_socket(int sock){
	struct sockaddr_in adresse;
	socklen_t longueur;
	longueur=sizeof(struct sockaddr_in);
	if(getsockname(sock, (struct sockaddr *)& adresse, & longueur)<0){
		perror("getsockname");
		return -1;
	}
	fprintf(stdout, "IP =%s, Port = %u \n", inet_ntoa(adresse.sin_addr), ntohs(adresse.sin_port));
	return 0;
}


int serveur_tcp (void){
	pthread_t ThreadScan;
	void* ret;
	struct in_addr ip4;
	int sock_contact;
	int sock_connectee;
	struct sockaddr_in adresse;
	socklen_t longueur;
	ssize_t longueur2;
	char buffer[256];
	size_t nbytes;
	nbytes=sizeof(buffer);

	if(inet_aton("192.168.1.20",&ip4)==0){
		fprintf(stdout, "problème ip \n");
	}

	sock_contact=cree_socket_stream(ip4, 1080, "tcp");
	if (sock_contact<0){
		fprintf(stdout, "echec \n");
		return -1;
	}
	listen(sock_contact, 5);
	fprintf(stdout, "Mon adresse >>");
	affiche_adresse_socket(sock_contact);
	fprintf(stdout,"affiche adresse ok");
	while(! quitter_le_serveur()){
		longueur=sizeof(struct sockaddr_in);
		sock_connectee=accept(sock_contact, (struct sockaddr *)& adresse, & longueur);
		if(sock_connectee<0){
			perror("accept");
			return -1;
		}
		switch (fork()){
		case 0: //Fils
			close(sock_contact);
			traite_connexion(sock_connectee);
			fprintf(stdout, "Traite connexion OK \n");
			while(1){
				longueur2=read(sock_connectee,buffer,nbytes);
				printf("\nLongueur 2 : %d",longueur2);
				printf("\n action : %s",buffer);
				if(strncmp(buffer,"Fin",3)==0){
					fprintf(stdout, "Fin OK \n");
					break;
				}
				if(strncmp(buffer,"Goudale",7)==0){
					fprintf (stdout, "Thread scan on\n");
					if (pthread_create (&ThreadScan, NULL, Scan, "Goudale") < 0) {
						fprintf (stderr, "pthread_create error for threadScan\n");
						exit (1);
					}
					(void)pthread_join(ThreadScan, &ret); // on attends que thread de scan s'arrête
					fprintf (stdout, "Thread scan off\n");
				}
				if(strncmp(buffer,"Avant",5)==0){
					i2cset(0x1a,0x45);
					 // moteur 1 forward 50%, moteur 2 forward 50%
					sleep(1);
					fprintf(stdout, "Avant! \n");

				}
				if(strncmp(buffer,"Arriere",7)==0){
					i2cset(0x1a,0x4A);
					// moteur 1 reverse 50%, moteur 2 reverse 50%
					sleep(1);
					fprintf(stdout, "Arriere! \n");
				}
				if(strncmp(buffer,"ArrGche",7)==0){
					i2cset(0x1a,0x48);
					// moteur 1 (gauche) arret, moteur 2 (droit) reverse 50%
					sleep(1);
					fprintf(stdout, "ArrGche! \n");
				}
				if(strncmp(buffer,"ArrDte",6)==0){
					i2cset(0x1a,0x42);
					// moteur 1 (gauche) reverse, moteur 2 (droit) arret
					sleep(1);
					fprintf(stdout, "ArrDte! \n");
				}
				if(strncmp(buffer,"AvtGche",7)==0){
					i2cset(0x1a,0x44);
					// moteur 1 (gauche) arret, moteur 2 (droit) forward 50%
					sleep(1);
					fprintf(stdout, "AvtGche! \n");
				}
				if(strncmp(buffer,"AvtDte",6)==0){
					i2cset(0x1a,0x41);
				    // moteur 1 (gauche) forward, moteur 2 (droit) arret
					sleep(1);
					fprintf(stdout, "AvtDte! \n");
				}
				if(strncmp(buffer,"Gauche",6)==0){
					i2cset(0x1a,0x46);
					// moteur 1 (gauche) reverse 50%, moteur 2 (droit) arret
					sleep(1);
					fprintf(stdout, "Gauche! \n");
				}
				if(strncmp(buffer,"Droite",6)==0){
					i2cset(0x1a,0x49);
					// moteur 1 (gauche) forward 50%, moteur 2 (droit) arret
					sleep(1);
					fprintf(stdout, "Droite! \n");
				}
				if(strncmp(buffer,"Arret",5)==0){
					i2cset(0x1a,0x00);
					// moteur 1 (gauche) arret, moteur 2 (droit) arret
					sleep(1);
					fprintf(stdout, "Arret! \n");
				}
				//buffer[longueur2]='\0';
			}
			close(sock_connectee);
			exit(EXIT_SUCCESS);
		case -1:
		perror("fork");
		return -1;
		default: // Père
		close(sock_connectee);
		break;
		}
	}
	return 0;
}

void traite_connexion(int sock){
	struct sockaddr_in adresse;
	socklen_t longueur;
	char buffer[256];
	fprintf(stdout,"traite connexion ok");
	longueur=sizeof(struct sockaddr_in);
	if(getpeername(sock,(struct sockaddr *) & adresse, & longueur)<0){
		perror("getpeername");
		return;
	}
	sprintf(buffer, "IP= %s, Port = %u \n", inet_ntoa(adresse.sin_addr), ntohs(adresse.sin_port));
	fprintf(stdout, "Connexion : locale");
	affiche_adresse_socket(sock);
	fprintf(stdout, "   distante %s", buffer);
	//write(sock, "Votre adresse : ", 16);
	//write(sock, buffer, strlen(buffer));
	//close(sock);
}

