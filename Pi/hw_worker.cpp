#include <wiringPi.h>
#include <iostream>
#include "pin_outs.h"
#include "hw_worker.h"
#include <time.h>
#include <unistd.h>

using namespace std;


/**
 * This method checks the state of the IR sensor for obstructions
 * @return : True if object detected in path of IR sensor
 */
bool hw_worker::ir_detect()
{
	return digitalRead(IR_PIN);//state;
}

/**
 * This method checks the state of the PIR motion sensing detector
 * @return : True if motion detected in sensor's path
 */
bool hw_worker::motion_detect()
{
	return digitalRead(MOTION_PIN);
}

/**
 * This method is used to test the function of the motor
 * @param : duration : Specifies the amount of increments to make
 */
void hw_worker::step_motor (int duration)
{
	digitalWrite(DIR_MOTOR_PIN, HIGH);

	int i, state;
	for (i = 0; i < duration; i++)
	{
		digitalWrite(STEP_MOTOR_PIN, HIGH);
		usleep(1000);
		digitalWrite(STEP_MOTOR_PIN, LOW);
		usleep(1000);
	}
}
