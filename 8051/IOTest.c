#include <C8051F020.h>


#define SYSCLK 22118400

void Init_Port(void);
void Init_Clock(void);

void Init_Timer3(unsigned int counts);
void Timer3_ISR(void);

//Special function registers
sfr16 TMR3RL = 0x92; // Timer3 reload value
sfr16 TMR3 = 0x94; // Timer3 counter

//sfr P2 = 0xFF;
sbit P1_0 = P1^0;
sbit P1_4 = P1^4;
sbit P2_0 = P2^0;
sbit P2_4 = P2^4;
//pull out these defines to a header file
char PIN_IR, PIN_MOTION, PIN_MOTOR_STEP, PIN_MOTOR_DIR;

void main(void)
{	
	Init_Port();
	Init_Clock();
	//Init_Timer3(SYSCLK/12);
	
	//P5 &= 0x0F;	
			
	//P1_1 = 1;    // Set port for input

	P5 = P5 & 0x00;
	P1_0 = 0xFF;
	P1_4 = 0xFF;
	//P2_0 = 0x01;
	//P2_4 = 0x01;

	while(1)
	{
		PIN_MOTION = P1_0;
		PIN_MOTION &= 0x01;
		PIN_IR = P1_4;
		PIN_IR &= 0x01;

		//IR Detected
		if (PIN_IR == 1) {
			P5 &= 0x30;
			Dispense();
		}
		else {
			P5 &= 0x00;
			P5 ^= 0x30;
		}

		//Motion Detected
		if (PIN_MOTION == 1) {
			P5 &= 0x00;
			P5 ^= 0xC0;
		}
		else {
			P5 &= 0xC0;
		}
	}

}


void Init_Port(void)
{
	XBR0 = 0x04;	//Ports Initialization
	XBR1 = 0x00;
	XBR2 = 0x40;
	P1 |= 0xFF;		//enables crossbar & weak pull ups 
	P2 |= 0x01;


	//P0MDOUT = 0x00;
	//P1MDOUT |= 0xFF;
	//P1MDOUT &= 0x7F;
	P2MDOUT |= 0xFF;
	//P3MDOUT = 0x00;

	P1MDIN |= 0xFF;
		
	P74OUT = 0x08;
	
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

void Init_Timer3(unsigned int counts)
{
	TMR3CN = 0x00; //-- Stop Timer3; Clear TF3;
					//-- use SYSCLK/12 as timebase

	TMR3RL = -counts; //-- Init reload values

	TMR3 = 0xFFFF;	//-- set to reload immediately

	EIE2 |= 0x01; 	//-- enable Timer3 interrupts

	TMR3CN |= 0x04; //-- start Timer3 by setting TR3
					// (TMR3CN.2) to 1

}

void Timer3_ISR(void) interrupt 14
{
	TMR3CN &= ~(0x80);	//-- clear TF3 flag

	//sbit test = P1;

	if (P1 & 1 == 1) {
		P5 = P5 & 0x00;
		P5 = P5 ^ 0x70;
	}
	else {
		P5 = P5 & 0x00;
		P5 = P5 ^ 0x10;
	}
}

void Dispense(void)
{
	//push motor out
}
