//
//  SerialPort.h
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#pragma once

#include <stdio.h>   /* Standard input/output definitions */
#include <string>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

using namespace std;

class SerialPort
{
private:
    char* path;
    int fd;
    bool portOpened = false;

public:
    SerialPort();
    SerialPort(char* givenPath);
    ~SerialPort();

    bool isAvailable();
    void serialPrint(char* c);

};
