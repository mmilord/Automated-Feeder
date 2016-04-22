#include <compiler_defs.h>
#include <c8051f020_defs.h>
//#include <time.h>

int a = 0;

#define SYSCLK 2211840

void Init_Port(void);
void Init_Clock(void);

void Init_Timer2(unsigned int counts);
void Timer2_ISR(void);

sbit P1_0 = P1^0;
sbit P1_4 = P1^4;
int i;

void main(void)
{
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	EA = 0;
	

	Init_Clock();
	Init_Port();
	
	Init_Timer2(10000);

	EA = 1;

	P1_4 = 0xFF;
	//P5 |= 0xFF;	

	while(1) {
		//P1 |= 0xFF;
		//P1 &= count;
	}
}

void Init_Port(void)
{
	XBR0 = 0x04;	//Ports Initialization
	XBR1 = 0x00;
	XBR2 = 0x40;
	P1 |= 0x01;		//enables crossbar & weak pull ups 
//	P2 |= 0x01;


	//P0MDOUT = 0x00;
	P1MDOUT |= 0xFF;
	//P1MDOUT &= 0x7F;
//	P2MDOUT = 0xFF;
	//P3MDOUT = 0x00;
		
	P74OUT = 0x08;
	P5 &= 0x0F;

	
}

void Init_Clock(void)
{
	OSCXCN = 0x67;				//Oscillator Frequency
   	while(!(OSCXCN & 0x80));	//Wait till XTLVLD pin is set
	
	OSCICN = 0x94;			
	//-- Bit 2 : Internal Osc. disabled (IOSCEN = 0)
	//-- Bit 3 : Uses External Oscillator as System Clock
	// (CLKSL = 1)
	//-- Bit 7 : Missing Clock Detector Enabled (MSCLKE = 1)

}


void Init_Timer2(unsigned int counts)
{
	CKCON = T2CON = 0x00;
	RCAP2 = counts;
	T2 = 0xFFFF;
	IE |= 0x20; // enables interrupt
	T2CON |= 0x04; //starts Timer2

}

void Timer2_ISR(void) interrupt 5
{

	T2CON &= ~(0x80);
	P1 |= 0xFF;

	if((P5 & 0x01) == 0x00)// RESET on Button P5.0
	{
		a = 0;
		P5 ^= 0xF0;

		while (a < 20) {
			P1_0 = 0xFF;
			for (i = 0; i < 500; i++) {
				;
			}
			P1_0 = 0x00;
			for (i = 0; i < 500; i++) {
				;
			}
			a++;
		}
	}

	/*if((P5 & 0x02) == 0x00) // START/STOP on Button P5.1
	{
		/*if (ET0 == 1)
			ET0 = 0;
		else
			ET0 = 1;
	}*/
}





