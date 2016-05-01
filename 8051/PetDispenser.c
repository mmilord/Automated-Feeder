#include <compiler_defs.h>
#include <c8051f020_defs.h>
//#include <time.h>

int a = 0;

#define SYSCLK 2211840

void Init_Port(void);
void Init_Clock(void);

void Init_Timer3(unsigned int counts);
void Timer3_ISR(void);

void MotorCN(void);

void Init_Timer0(unsigned int counts);
void Timer0_ISR(void);

void Init_Timer2(unsigned int counts);
void Timer2_ISR(void);

sbit P0_2 = P0^2;
sbit P0_4 = P0^4;
sbit P1_0 = P1^0;
sbit P1_4 = P1^4;
sbit P1_1 = P1^1;
sbit P1_5 = P1^5;
sbit P1_7 = P1^7;// Right decimal
sbit P1_6 = P1^6;// left decimal

char PIN_IR, PIN_MOTION, PIN_MOTOR_STEP, PIN_MOTOR_DIR;

int  i, tickCounter, irTickCount = 0, dspCount = 0;


void main(void)
{
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	EA = 0;
	
	Init_Clock();
	Init_Port();

	//PIN_MOTION = P1_4;
	//PIN_MOTION &= 0x01;
	PIN_IR = P1_0;
	PIN_IR &= 0x01;
	PIN_MOTOR_DIR = P1_5;
	PIN_MOTOR_STEP = P1_1;

	
	Init_Timer0(SYSCLK);
	Init_Timer2(1000);
	Init_Timer3(SYSCLK);

	EA = 1;

	//PIN_MOTION = 0xFF;
	PIN_MOTOR_DIR = 0xFF;	//set motor direction to clockwise;
	
	P1_7 = 1;

	while(1) {
	}
}

void Init_Clock(void)
{
	OSCXCN = 0x67;				//Oscillator Frequency
   	while(!(OSCXCN & 0x80));	//Wait till XTLVLD pin is set
	
	OSCICN = 0x94;
}

void Init_Port(void)
{
	XBR0 = 0x04;	//Ports Initialization
	XBR1 = 0x00;
	XBR2 = 0x40;
	P1 |= 0x01;		//enables crossbar & weak pull ups 
//	P2 |= 0x01;

	//P1MDOUT &= 0x00;
	//P1MDIN &= 0x00;
	P1MDOUT |= 0xFF;
	//P1MDIN |= 0x09;
	//P1MDOUT &= 0x7F;
	P2MDOUT = 0xFF;
		
	P74OUT = 0x08;
	P5 &= 0x0F;
}

void MotorCN(void)
{
	
	a = 0;
	while (a < 20) {
		P1_1 = 0xFF;
		for (i = 0; i < 500; i++) {
			;
		}
		P1_1 = 0x00;
		for (i = 0; i < 500; i++) {
			;
		}
		a++;
	}
	dspCount++;

	if (dspCount > 0x59)
	{
		dspCount = 0x00;	
	}
	else if ((0x0F & dspCount) > 0x09)
	{
		dspCount +=6 ;
	}

	P2 |= 0xFF;
	P2 &= dspCount;
}

void Init_Timer0(unsigned int counts)
{
	//60 second countdown to motion

	TMOD &= 0xFE;

	TMOD |= 0x02;

	TH0 = 50;

	TR0 = 1;

	ET0 = 1;

	tickCounter = 0;
}
void Timer0_ISR(void) interrupt 1
{
	tickCounter++;

	if(tickCounter > 5000)
	{
		ET0 = 0; //disable timer0 isr
		EIE2 |= 0x01; //disable timer3 isr
//		ET3 = 1; //enable timer3 isr
		tickCounter = 0;
	}
}

void Init_Timer2(unsigned int counts)
{
	//IR detection, decimal flash on low & button press check

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
		MotorCN();
	}

	if((P1_0 & 0x01) == 1)
	{
		if (irTickCount > 10) {
			irTickCount = 0;
		}
		else if (irTickCount > 5) {
			P1 &= ~0x80;
			P1 &= 0x40; 
		}
		else if (irTickCount > 0) {
			P1 &= 0x80;
			P1 &= ~0x40;
		}

		irTickCount++;
	}
}


void Init_Timer3(unsigned int counts)
{	
	//Motion Detector

	TMR3CN = 0x00; //-- Stop Timer3; Clear TF3;
					//-- use SYSCLK/12 as timebase

	TMR3RL = -counts; //-- Init reload values

	TMR3 = 0xFFFF;	//-- set to reload immediately

	//EIE2 |= 0x01; 	//-- enable Timer3 interrupts

	TMR3CN |= 0x04; //-- start Timer3 by setting TR3
					// (TMR3CN.2) to 1

}
void Timer3_ISR(void) interrupt 14
{	
//	PIN_MOTION &= 0x01;
	//Motion Detected
	if ((P1_4 & 0x01) == 1) {
		//P5 &= 0x00;
		//P5 ^= 0xC0;
		MotorCN();
		EIE2 &= ~(0x01); //disable timer3 isr
		ET0 = 1; //enable timer0 isr
		//TCON &= ~(0x80); //reset timer0 isr
	}
}



