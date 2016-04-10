//
// Created by mmilord on 2/24/16.
//

#ifndef IO_H
#define IO_H

#include <bits/stringfwd.h>
#include <ctime>

struct prog_conf {
    int mode;
    int freq;	//Frequency is in minutes
    bool isEmpty;
};

class io
{
public:
    bool write_conf(prog_conf args);
    prog_conf read_conf();
    struct tm read_log();
    bool write_log(int mode);
};

#endif //IO_H
