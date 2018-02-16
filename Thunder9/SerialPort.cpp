//
//  SerialPort.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#include "SerialPort.h"

#include <stdio.h>   /* Standard input/output definitions */
#include <string>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

using namespace std;


SerialPort::SerialPort()
{

}

SerialPort::SerialPort(char* givenPath)
{
    path = givenPath;

    fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1)
    {
        perror("open_port: Unable to open port - ");
        portOpened = false;
    }
    else
    {
        fcntl(fd, F_SETFL, 0);
        portOpened = true;

        //Giving time to arduino to initialize
        sleep(1);
    }

}

SerialPort::~SerialPort()
{
    close(fd);
}

bool SerialPort::isAvailable()
{
    return portOpened;
}

void SerialPort::serialPrint(char* c)
{
    int n = write(fd, c, 1);
    if (n < 0)
        fputs("writing failed!\n", stderr);
}
