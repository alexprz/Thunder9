//
//  animation.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#include "animation.hpp"

void flash(SerialPort &arduino)
{
    arduino.serialPrint("a");
}
