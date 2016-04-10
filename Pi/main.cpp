//
// Created by mmilord on 2/25/16.
//
#include <wiringPi.h>
#include <stdlib.h>
#include "io.h"
#include "hw_worker.h"
#include "pin_outs.h"
#include <ctime>
#include <time.h>
#include <unistd.h>
#include <iostream>

prog_conf global_conf;
io _io;
hw_worker _hw_worker;
struct tm global_last_disp_time;

//Init GPIO pins with wiringPi
void initPins() {
    wiringPiSetupGpio();
    pinMode(IR_PIN, INPUT);
    pinMode(MOTION_PIN, INPUT);
    pinMode(STEP_MOTOR_PIN, OUTPUT);
    pinMode(DIR_MOTOR_PIN, OUTPUT);
}

//Dispense and attempt to log to file
void motor_dispense() {
    //Step motor to dispense one 1/6th rotation
    _hw_worker.step_motor(50);

	//Allow food to settle
    usleep(1000000 * 5);
    //Check IR for low fill level
    if (!_hw_worker.ir_detect()) {
        _io.write_log(2);
		//email alert/log
    }

    //Log file with fail checks for log failure and sys reboot if multi fails;
    int i = 0;
    bool temp = false;
    while (!temp) {
        if (i > 5)
            system("sudo reboot");
        temp = _io.write_log(0);
        i++;
    }
}

//Main runner
void master_control(int mode) {
    while (true) {
        if (_hw_worker.motion_detect()) {
			std::cout << "motion detect" << std::endl;
			_io.write_log(1);
            time_t time_current = time(0);
			long int seconds = time_current - global_last_disp_time.tm_sec;
            
			if (seconds > global_conf.freq * 60) {
				cout << "motor time" << endl;
				motor_dispense();
				global_last_disp_time = _io.read_log();
				usleep(1000000 * 4);
            }
			
			std::cout << "post time check" << std::endl;
        }
        usleep(1000000);
    }
}

int main() {
    initPins();
	
	std::cout << "pre load conf" << std::endl;
    global_conf = _io.read_conf();
    std::cout << "pre init last disp" << std::endl;
	global_last_disp_time = _io.read_log();
	std::cout << "pre master" << std::endl;
    master_control(global_conf.mode);
    std::cout << "pre terminate" << std::endl;
	
	return 0;
}
