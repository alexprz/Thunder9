//
//  Tools.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#include "Tools.hpp"

int bpmToUS(float bpm)
{
    return int(1000000*60/bpm);
}
