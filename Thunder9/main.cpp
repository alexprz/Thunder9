//
//  main.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//


#include <iostream>

using namespace std;

#include <SFML/Audio.hpp>

#include "SerialPort.h"
#include "animation.hpp"
#include "Tools.hpp"

#include <thread>
#include <iostream>

bool triggerFlash = false;
bool over = false;

void severalFlash()
{
    SerialPort arduino("/dev/cu.usbmodem1411");

    while(!over)
    {
        if(triggerFlash)
        {
            flash(arduino);
            triggerFlash = false;
        }
    }
}

int main()
{
    std::thread t1(severalFlash);
    for(int j = 0; j<100; j++)
    {
        cout << j << endl;
        triggerFlash = true;
        usleep(bpmToUS(76));
    }
    over = true;
    t1.join();
    return 0;
}
