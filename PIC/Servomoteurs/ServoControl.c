// Programme de control de servo

#include <htc.h>
#define _XTAL_FREQ 4000000
#define PULSE_ON	1
#define PULSE_OFF	0

__CONFIG(FOSC_HS & WDTE_OFF & LVP_OFF & WRT_OFF & CP_OFF);

//variables globales
int cpt=0;
int cpt2=0;
int cons=9000/5;
int cons2=9000/5;
int cons3=9000/5;
int cons4=9000/5;

//I2C variables
const int Addy = 0x54;                    // I2C adresse = 0x21
unsigned short j;                      // 
unsigned short rxbuffer;               //
unsigned short rxbuffer2;
unsigned short rxbuffer3;
unsigned short rxbuffer4;
unsigned short tx_data=0x1C;                //
unsigned short tx_data2;    
unsigned short tx_data3;    
unsigned short tx_data4;    
float calcul;
float calcul2;
float calcul3;
float calcul4;

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
void Init_Timer(){
	// TIMER1
	//prescaler timer1 1:4
	T1CKPS1=0;
	T1CKPS0=0;
	//timer1 oscillator off
	T1OSCEN=0;
	// timer1 sur internal clock (Fosc/4)=5Mhz = 200ns
	TMR1CS=0;
	TMR1IE=1;
	// enable bit timer1, on le démarre
	TMR1ON=1;
	GIE=1;	// enable all global interrupts
	PEIE=1;	// enable all peripheral interrupts
}
void main(void){
	//pin de signal de control du servo
	TRISB1=0;
	RB1=0;
	TRISB0=0;
	RB0=0;
	TRISB4=0;
	RB4=0;
	TRISB5=0;
	RB5=0;
	// Heartbeat
	TRISB2=0;
	RB2=0;
	Init_Timer();
	Init_i2c();
	while(1){
		calcul=(3500+(rxbuffer/160.0)*7700)/5;
		calcul2=(3500+(rxbuffer/160.0)*7700)/5;
		calcul3=(3500+(rxbuffer3/160.0)*7700)/5;
		calcul4=(3500+(rxbuffer4/160.0)*7700)/5;
		cons=(int)calcul;
		cons2=(int)calcul2;
		cons3=(int)calcul3;
		cons4=(int)calcul4;
	//cons=11200/5;
		if (cons>=(11200/5))
		{
			cons=11200/5;
		}
		if (cons<=(3500/5))
		{
			cons=3500/5;
		}
		if (cons2>=(11200/5))
		{
			cons2=11200/5;
		}
		if (cons2<=(3500/5))
		{
			cons2=3500/5;
		}
		if (cons3>=(11200/5))
		{
			cons3=11200/5;
		}
		if (cons3<=(3500/5))
		{
			cons3=3500/5;
		}
		if (cons4>=(11200/5))
		{
			cons4=11200/5;
		}
		if (cons4<=(3500/5))
		{
			cons4=3500/5;
		}
		//calcul2=(cons-3500)*(180.0/9000.0);
		tx_data=rxbuffer;
		tx_data2=rxbuffer2;
		tx_data3=rxbuffer3;
		tx_data4=rxbuffer4;
		//tx_data=(unsigned short)(cons/100);
	//	tx_data=(unsigned short)calcul2;
		__delay_ms(250);
	}
}

void interrupt GestionPulseTimer_I2C(void)
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
			if ((SSPBUF&0x03)==0x00)
			{
				rxbuffer = SSPBUF;				// get data
			}else{
				if((SSPBUF&0x03)==0x01)
				{
					rxbuffer2 = SSPBUF-1;
				}else{
					if((SSPBUF&0x03)==0x02)
					{
						rxbuffer3 = SSPBUF-2;
					}else{
						if((SSPBUF&0x03)==0x03)
						{
							rxbuffer4 = SSPBUF-3;
						}
					}
				}
			}
			j = SSPBUF;                      // read buffer to clear flag [address]
			return;
		}
		j = SSPBUF;                       // read buffer to clear flag [address]
    }
  
	if (TMR1IF)
	{
		// on réarme le timer
		TMR1IF = 0;
		if (RB5==PULSE_OFF && RB4==PULSE_OFF && RB0==PULSE_OFF && RB1==PULSE_OFF)
		{
			cpt=cpt+1;
			if (cpt>=(71))
			{
				if (RB2==0)
				{
					RB2=1;
					cpt=0;
				}else{
					RB2=0;
					cpt=0;
				}
			}
			RB0=PULSE_OFF;
			RB4=PULSE_OFF;
			RB5=PULSE_OFF;
			RB1=PULSE_ON;
			TMR1=65535-cons; //pulse de la durée de la consigne
		}else{
			if (RB1==PULSE_ON)
			{
				RB1=PULSE_OFF;
				RB0=PULSE_ON;
				RB4=PULSE_OFF;
				RB5=PULSE_OFF;
				TMR1=65535-cons2; //pulse consigne 2
			}else{
				if(RB0==PULSE_ON)
				{
					RB1=PULSE_OFF;
					RB0=PULSE_OFF;
					RB4=PULSE_ON;
					RB5=PULSE_OFF;
					TMR1=65535-cons3; //pulse consigne 2
				}else{
					if (RB4==PULSE_ON)
					{
						RB1=PULSE_OFF;
						RB0=PULSE_OFF;
						RB4=PULSE_OFF;
						RB5=PULSE_ON;
						TMR1=65535-cons4; //pulse consigne 2
					}else{
						RB1=PULSE_OFF;
						RB0=PULSE_OFF;
						RB4=PULSE_OFF;
						RB5=PULSE_OFF;
						TMR1=52428+(cons+cons2+cons3+cons4);
					}
				}
			}
		}
	}
	return;
}