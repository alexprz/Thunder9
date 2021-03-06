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
#include <SFML/Graphics.hpp>

#endif /* interface_hpp */

struct instant
{
    long int intensity;
    bool peak;
    double avg;
    double std;
};

class Buffer
{
private:
    instant *tab;
    sf::RenderWindow *window;
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
    
    Buffer(sf::RenderWindow *givenWindow, int width1, int height1, double timescale1, long int intensityscale1);
    void push(long int intensityvalue);
    void addPeak();
    void addAvg(double givenAvg);
    void addStd(double givenStd);
    long int getIntensity(int i);
    void thread();
    void refresh();
    void refreshWindow();
    void startDisplay();
    ~Buffer();
};
