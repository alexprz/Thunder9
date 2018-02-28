//
//  interface.hpp
//  Thunder9
//
//  Created by Antoine Eudes on 28/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#ifndef interface_hpp
#define interface_hpp

#include <stdio.h>

#endif /* interface_hpp */

struct instant
{
    bool peak=false;
    int intensity=0;
};

class Buffer
{
    instant* tab;
    int width;
    int height;
    int intensityscale;
    int timescale;
public:
    Buffer(int width, int height, int timescale, int intensityscale);
    void push(int intensityvalue);
    void addPeak(bool peakpresence);
    void refresh();
    ~Buffer();
};


