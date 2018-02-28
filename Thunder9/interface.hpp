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
    long int intensity=0;
};

class Buffer
{
    instant* tab;
    int width;
    int height;
    int intensityscale;
    int timescale;
    int threshold=0;
public:
    Buffer(int width1, int height1, double timescale1, int intensityscale1);
    void push(int intensityvalue);
    void addPeak();
    long int getIntensity(int i);
    long int getThresold();
    void setThreshold();
    void refresh();
    ~Buffer();
};


