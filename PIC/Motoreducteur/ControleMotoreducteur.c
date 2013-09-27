// Programme de control du H bridge avec réception des consignes venant de la pandaboard par I2C

#include <htc.h>
#define _XTAL_FREQ 20000000

float Motor1_SetPoint=0;
float Motor2_SetPoint=0;
int Direction1=0;
int Direction2=1;
int Dir1prev=0;
int Dir2prev=1;
//I2C variables globales
const int Addy = 0x34;                    // I2C adresse = 0x1A
unsigned short j;                      // 
unsigned short rxbuffer;               //
unsigned short tx_data;                //

__CONFIG(FOSC_HS & WDTE_OFF & LVP_OFF & WRT_OFF & CP_OFF);

void Init_i2c(){
	ADCON1 = 0;                          // All ports set to digital
	TRISC3=1;
	TRISC4=1;
	SSPADD =  Addy;                      // Get address (7bit). Lsb is read/write flag
	SSPCON = 0x36;                       // Set to I2C slave with 7-bit address
	SSPIF = 0;                      // enable SSP interrupts
	INTCON = 0xC0;                       // enable INTCON.GIE
	SSPIE = 1;
}
void Init_Timer1(){
	// TIMER1 pour la LED
	//prescaler timer1 1:8
	T1CKPS1=1;
	T1CKPS0=1;
	//timer1 oscillator off
	T1OSCEN=0;
	// timer1 sur internal clock (Fosc/4)=5Mhz = 200ns
	TMR1CS=0;
	TMR1IE=1;
	// enable bit timer1, on le démarre
	TMR1ON=1;
	GIE=1;	// enable all global interrupts
	PEIE=1;	// enable all peripheral interrupts
	// LED de debug
	TRISC5=0;
	RC5=0;
}

void Init_Timer2(){
	//Timer 2
	PR2=0x1F; // PWM à ...
	T2CKPS1=0;
	T2CKPS0=1;// prescaler ...
	// CCP modules
	CCPR1L=0x80;//Duty cycle à 50%
	CCPR2L=0x80;//Duty cycle à 50%
	CCP1CON=0b00001111;//PWM mode, 3e et 4e bit LSB du duty cyle
	CCP2CON=0b00001111;//PWM mode, 3e et 4e bit LSB du duty cyle
	// pins de sortie PWM1 et PWM2 à connecter aux pin enable A et B du H-bridge
	TRISC1=0; //CCP2 EnA
	TRISC2=0; //CCP1 EnB
	TMR2ON=1;// Timer 2 start
}


void main(void){
	//pins de signal de polarisation moteur A
	TRISC7=0;
	RC7=1;// Motor A + (In1)
	TRISC6=0;
	RC6=0;// Motor A - (In2)
	//pins de signal de polarisation moteur B
	TRISB4=0;
	RB4=0;// Motor B + (In3)
	TRISB5=0;
	RB5=1; // Motor B - (In4)
	Init_i2c();
	Init_Timer1();
	Init_Timer2();
	rxbuffer=0x00;
	while(1){
		if ((rxbuffer&0x03)==0x00)
		{
			Motor1_SetPoint=0.0; 
		}else{ 
			if((rxbuffer&0x03)==0x01)
			{
				Direction1=1; // forward moteur 1
				Motor1_SetPoint=(rxbuffer&0x70)/16.0*14.0; 
			}else{
				if((rxbuffer&0x03)==0x02)
				{
					Direction1=0; //reverse moteur 1
					Motor1_SetPoint=(rxbuffer&0x70)/16.0*14.0; 
				}
			}
		}
		if ((rxbuffer&0x0C)==0x00)
		{
			Motor2_SetPoint=0.0; 
		}else{ 
			if((rxbuffer&0x0C)==0x04)
			{
				Direction2=0; // forward moteur 1
				Motor2_SetPoint=(rxbuffer&0x70)/16.0*14.0; 
			}else{
				if((rxbuffer&0x0C)==0x08)
				{
					Direction2=1; //reverse moteur 1
					Motor2_SetPoint=(rxbuffer&0x70)/16.0*14.0; 
				}
			}
		}
		tx_data=(unsigned short)Motor1_SetPoint;//PWM en %
		CCPR1L=(unsigned short)(Motor2_SetPoint*2.56);
		CCPR2L=(unsigned short)(Motor1_SetPoint*2.56);
		if (Dir1prev==Direction1){
			Dir1prev=Direction1;
			if (Direction1==1){
				RC7=0; //reverse Moteur 1
				RC6=1;
			}else{
				RC7=1;  // forward Moteur 1
				RC6=0;
			}
		}else{
			RC7=0; //phase d'arrêt Moteur 1
			RC6=0;
			Dir1prev=Direction1;
		}
		if (Dir2prev==Direction2){
			Dir2prev=Direction2;
			if (Direction2==1){
				RB4=0; //reverse Moteur 2
				RB5=1;
			}else{
				RB4=1; // forward Moteur 2
				RB5=0;
			}
		}else{
			RB4=0; //phase d'arrêt Moteur 2
			RB5=0;
			Dir2prev=Direction2;
		}
		__delay_ms(250);
	}
}

void interrupt Timer_LED_I2C(void)
{
	if (SSPIF == 1){                // I2C Interrupt
	SSPIF = 0;                    // reset SSP interrupt flag
    //transmit data to master
    if (R_W == 1){             // Read request from master
      SSPBUF = tx_data;                // Get data to send
      CKP = 1;                  // Release SCL line
      j = SSPBUF;                      // That's it
      return;
    }
    if (BF == 0){              // all done,
      j = SSPBUF;                      // Nothing in buffer so exit
      return;
    }

    //recieve data from master
    if (D_A == 1){             // Data [not address]
      rxbuffer = SSPBUF;               // get data
      j = SSPBUF;                      // read buffer to clear flag [address]
      return;
    }
		j = SSPBUF;
	}
       
	if (TMR1IF)
	{
		// on réarme le timer
		TMR1IF = 0;
		if (RC5==0)
		{
			RC5=1;
		}else{
			RC5=0;
		}
		TMR1=0; //on va attendre 13.1ms *8
	}
	return;
}
