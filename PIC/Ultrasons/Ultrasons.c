//LED Blink
#include <htc.h>
#define _XTAL_FREQ 20000000

void init();
void DemandeDistance ();

__CONFIG(FOSC_HS & WDTE_OFF & LVP_OFF & WRT_OFF & CP_OFF);

int newValue;

void main()
{	
	TRISB1=0;
	RB1=1;
	TRISB2=0;
	RB2=0;

	DemandeDistance();	
	int i=1;
	int distance;
	for(;;)
	{
		if (newValue){
			distance=TMR1/73;
			newValue=0;
			if (distance<450){
				for (i=1;i<distance;i++){
					RB1=0;
					__delay_ms(500);
					RB1=1;
					__delay_ms(500);
				}
				__delay_ms(4000);
			}
			DemandeDistance();
		}
	}
}

void init()
{
	// TIMER0
	TRISB0=1;
	INTEDG=1;// INTEDG => on rising edge
	// enable all global interrupts
	GIE=1;
	// enable all peripheral interrupts
	PEIE=1;
	// enable external interrupt on RB0
	INTE=1;

	// TIMER1
	//prescaler timer1 1:4
	T1CKPS1=1;
	T1CKPS0=0;
	//timer1 oscillator off
	T1OSCEN=0;
	// timer1 sur internal clock (Fosc/4)=5Mhz
	TMR1CS=0;
	// enable bit timer1, on le stop au départ
	TMR1ON=0;
}

void DemandeDistance ()
{
	RB2=1;
	__delay_us(10);
	RB2=0;
}

void interrupt ReceptionSonar(void)
{
	if (INTF) {
		INTF=0; // on clear le flag d'interruption
	
		if (INTEDG){ // front montant détecté
			TMR1=0x0000;//mise à zéro du timer1
			TMR1ON=1;// démarrage du timer
			INTEDG=0; //attente d'un front descendant
		}
		else{  // front descendant détecté
			TMR1ON=0;// on stop le timer
			INTEDG=1; //attente d'un front montant
			newValue=1;
		}	
	}
	return;
}

