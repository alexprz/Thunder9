//
//  animation.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#include "animation.hpp"
#include <iostream>

extern bool dev;

void flash(SerialPort &arduino)
{
    if(dev)
    {
        cout << "Flash !" << endl;
    }
    else
        arduino.serialPrint("a");

}
