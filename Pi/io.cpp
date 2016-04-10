//
// Created by mmilord on 2/24/16.
//

#include "io.h"
#include <fstream>
#include <ctime>
#include <iostream>

using namespace std;

/**
 * This method is used to write the log file
 * @param : mode : Specifies the version of log to write
 * @return : True if write succeeded
 */
bool io::write_log(int mode) {
    string filename;
    string log_type;
    if (!mode) {
        filename = "dispense.log";
        log_type = "DISPENSED";
    }
    else if (mode == 1) {
        filename = "sensor.log";
        log_type = "MOTION_DETECTED";
    }
    else if (mode == 2) {
        filename = "sensor.log";
        log_type = "FILL_LOW";
    }

    ofstream os (filename, std::ofstream::app);
    time_t time = std::time(nullptr);
    if (os.is_open()) {
        os << log_type << " " << time << endl;
        os.close();
        return true;
    }
    else {
		//log??
        return false;
	}
}

/**
 * This method is used to write to the conf file
 * @param : args : Specifies the conf state to write to the conf file
 * @return : True if write succeeded
 */
bool io::write_conf(prog_conf args) {
	//Establish defaults if corrupted config
    if (&args == NULL) {
        args.mode = 0;
        args.freq = 5;
        args.isEmpty = 0;
    }

    ofstream os ("fido.conf", std::ofstream::out);
    if (os.is_open()) {
        os /*<< "MODE:"*/ << args.mode << endl;
        os /*<< "FREQ:"*/ << args.freq << endl;
        os /*<< "IS_EMPTY:"*/ << args.isEmpty << endl;
        os.close();
        return true;
    }
    else {
        //log?
        return false;
    }
}

/**
 * This method is used to read the dispense log file
 * @return : Returns a tm struct containing the last dispensed time in seconds since epoch
 */ 
struct tm io::read_log() {
    string line, _line;
    time_t time_last;
	struct tm _tm = {0};
    ifstream is("dispense.log");
    if (is.is_open()) {
		while (!is.eof()) {
			getline(is, line, ' ');
			getline(is, line, '\n');
			if (!line.empty())
				_line = line;
		}
		is.close();
    }
    else {
        return _tm;
    }

    _tm.tm_sec = std::stoi(_line);

    return _tm;
}

/**
 * This method is used to read the conf file
 * @return : Returns a prog_conf struct containing the configuration values
 */
prog_conf io::read_conf() {
    string line;
    prog_conf input;
    ifstream is("fido.conf");
    if (is.is_open()) {
        getline(is, line);
		cout << line << endl;
        input.mode = std::stoi(line);
        getline(is,line);
        input.freq = std::stoi(line);
        getline(is, line);
        input.isEmpty = std::stoi(line);

        is.close();
    }

    else {
        //do log error on conf file
        input.mode = 0;
        input.freq = 480;
        input.isEmpty = 0;
        write_conf(input);
    }

    return input;
}