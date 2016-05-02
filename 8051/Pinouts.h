#ifndef PINOUTS_H
#define PINOUTS_H

#include <compiler_defs.h>
#include <c8051f020_defs.h>

sbit P0_2 = P0^2;
sbit P0_4 = P0^4;
sbit P1_0 = P1^0;
sbit P1_4 = P1^4;
sbit P1_1 = P1^1;
sbit P1_5 = P1^5;
sbit P1_7 = P1^7;// Right decimal
sbit P1_6 = P1^6;// left decimal

sbit PIN_IR = P1^0;
sbit PIN_MOTION = P1^4;
sbit PIN_MOTOR_STEP = P1^1;
sbit PIN_MOTOR_DIR = P1^5;
sbit DEC_RIGHT = P1^7;
sbit DEC_LEFT = P1^6;

#endif