//
//  interface.cpp
//  Thunder9
//
//  Created by Antoine Eudes on 28/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#include "interface.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <thread>

extern sf::RenderWindow WINDOWSOUND;
extern long int currentIntensity;

//ACCESSORS
long int Buffer::getIntensity(int i) {
    return tab[i].intensity;
}

void Buffer::setIntensityScale(long int scale){
    intensityScale = scale;
}
long int Buffer::getIntensityScale(){
    return intensityScale;
}

void Buffer::setTimeScale(double scale){
    timeScale = scale;
}
double Buffer::getTimeScale(){
    return timeScale;
}

void Buffer::setThreshold(unsigned long int value){
    threshold = value;
}
long int Buffer::getThreshold() {
    return threshold;
}

Buffer::Buffer(int width1, int height1, double timescale1, long int intensityscale1) {
    tab = new instant[width];
    width = width1;
    height = height1;
    timeScale = timescale1;
    intensityScale = intensityscale1;
}

void Buffer::push(int intensityvalue) {
    tab[width/2].intensity = intensityvalue;
    for (int i=1;i<width;i++) {
        tab[i-1] = tab[i];
    }
}

void Buffer::addPeak() {
    tab[width/2].peak = true;
}

void Buffer::refresh() {
//    int delay = timeScale/width;
//    tab[width/2].intensity = currentIntensity;
//    usleep(delay*1000);
    for (int i=0; i<100; i++) {
        std::cout << i << std::endl;
    }
}

void Buffer::startDisplay()
{
    std::thread tRefresh(&Buffer::refresh, this);
    tRefresh.join();
}






Buffer::~Buffer() {
    delete[] tab;
}

