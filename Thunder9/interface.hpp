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
    long int intensityScale;
    double timeScale;
    unsigned long int threshold=0;
    
public:
    
    //ACCESSORS
    void setIntensityScale(long int scale);
    long int getIntensityScale();
    
    void setTimeScale(double scale);
    double getTimeScale();
    
    void setThreshold(unsigned long int value);
    long int getThreshold();
    
    Buffer(int width1, int height1, double timescale1, long int intensityscale1);
    void push(int intensityvalue);
    void addPeak();
    long int getIntensity(int i);
    void refresh();
    ~Buffer();
};


