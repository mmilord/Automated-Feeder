#include "Pinouts.h"

int a = 0;

#define SYSCLK 2211840

void Init_Port(void);
void Init_Clock(void);

void Init_Timer3(unsigned int counts);
void Timer3_ISR(void);

void Init_Timer0(unsigned int counts);
void Timer0_ISR(void);

void Init_Timer2(unsigned int counts);
void Timer2_ISR(void);


int  i, motionTickCount = 0, irTickCount = 0, btnTickCount = 0, dspCount = 0;


void main(void)
{
	WDTCN = 0xDE;
	WDTCN = 0xAD;
	EA = 0;
	
	Init_Clock();
	Init_Port();

	Init_Timer0(SYSCLK);
	Init_Timer2(SYSCLK);
	Init_Timer3(SYSCLK);

	EA = 1;

	PIN_MOTOR_DIR = 0xFF;	//set motor direction to clockwise;

	while(1) { }
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

	P2MDOUT = 0xFF;
		
	P74OUT = 0x08;
	P5 &= 0x0F;
}

/**
 * Update 7 segment output
 */
void DisplayUpdate(void)
{
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

/**
 * Step Motor for 34 steps
 */
void MotorCN(void)
{
	EA = 0;
	a = 0;
	while (a < 34) {
		PIN_MOTOR_STEP = 1;
		for (i = 0; i < 500; i++) {
			;
		}
		PIN_MOTOR_STEP = 0;
		for (i = 0; i < 500; i++) {
			;
		}
		a++;
	}

	//Reset counters and renable timer interrupts
	btnTickCount = 0;
	motionTickCount = 0;
	EIE2 &= ~(0x01);
	ET0 = 1;

	EA = 1;
}


void Init_Timer0(unsigned int counts)
{
	//60 second countdown to motion

	TMOD &= 0xFE;

	TMOD |= 0x02;

	TH0 = 50;

	TR0 = 1;

	ET0 = 1;

	motionTickCount = 0;
}
void Timer0_ISR(void) interrupt 1
{
	motionTickCount++;

	if(motionTickCount > 25800)
	{
		ET0 = 0; //disable timer0 isr
		EIE2 |= 0x01; //disable timer3 isr
		motionTickCount = 0;
	}
}


void Init_Timer2(unsigned int counts)
{
	//IR detection & button press check

	CKCON = T2CON = 0x00;
	RCAP2 = counts;
	T2 = 0xFFFF;
	IE |= 0x20; // enables interrupt
	T2CON |= 0x04; //starts Timer2

}
void Timer2_ISR(void) interrupt 5
{

	T2CON &= ~(0x80);

	if(btnTickCount < 129)
	{
		btnTickCount++;
	}

	if((P5 & 0x01) == 0x00 && btnTickCount == 129)// Dispense on Button P5.0
	{
		MotorCN();
		DisplayUpdate();
	}

	if(PIN_IR == 1)
	{
		if (irTickCount > 20) {
			irTickCount = 0;
		}
		else if (irTickCount > 10) {
			DEC_LEFT = 1;
			DEC_RIGHT = 0; 
		}
		else if (irTickCount > 0) {
			DEC_RIGHT = 1;
			DEC_LEFT = 0;
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

	TMR3CN |= 0x04; //-- start Timer3 by setting TR3
					// (TMR3CN.2) to 1

}
void Timer3_ISR(void) interrupt 14
{	
	//If Motion Detected
	if (PIN_MOTION == 1) {
		MotorCN();
		DisplayUpdate();
		EIE2 &= ~(0x01); //disable timer3 isr
		ET0 = 1; //enable timer0 isr
	}
}



