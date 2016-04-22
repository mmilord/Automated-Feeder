#include <compiler_defs.h>
#include <c8051f020_defs.h>

//sfr16 TMR3RL = 0x92; // Timer3 reload value
//sfr16 TMR3 = 0x94; // Timer3 counter
int count = 0x00;

unsigned int tickCount;

#define SYSCLK 2211840

void Init_Port(void);
void Init_Clock(void);

void Init_Timer0(unsigned int counts);
void Timer0_ISR(void);

void Init_Timer2(unsigned int counts);
void Timer2_ISR(void);

void main(void)
{
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	EA = 0;
	

	Init_Clock();
	Init_Port();
	Init_Timer0(SYSCLK);
	Init_Timer2(100000);

	EA = 1;
	
	while(1) {
		P2 |= 0xFF;
		P2 &= count;
	}
}



void Init_Port(void)
{
	XBR0 = 0x04;	//Ports Initialization
	XBR1 = 0x00;
	XBR2 = 0x40;
	P1 |= 0x01;		//enables crossbar & weak pull ups 
	P2 |= 0x01;


	//P0MDOUT = 0x00;
	P1MDOUT |= 0xFF;
	//P1MDOUT &= 0x7F;
	P2MDOUT = 0xFF;
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

void Init_Timer0(unsigned int counts)
{
	TMOD &= 0xFE;

	TMOD |= 0x02;

	TH0 = 50;

	TR0 = 1;

	tickCount = 0;
}

void Timer0_ISR(void) interrupt 1
{	
	P1 |= 0xFF;
	tickCount++;

	if(tickCount > 860)
	{
		tickCount = 0;
		count++;
	
		if (count > 0x59)
		{
			count = 0x00;	
		}
		else if ((0x0F & count) > 0x09)
		{
			count +=6 ;
		}
		
		P2 |= 0xFF;
		P2 &= count;
	}

	if (tickCount == 430)
	{
		
		P1 &= 0x00;
	}
	
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
	if((P5 & 0x01) == 0x00)// RESET on Button P5.0
	{
		count = 0x00;

		P2 |= 0xFF;
		P2 &= count;
	}

	if((P5 & 0x02) == 0x00) // START/STOP on Button P5.1
	{
		if (ET0 == 1)
			ET0 = 0;
		else
			ET0 = 1;
	}
}
