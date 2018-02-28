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

extern sf::RenderWindow WINDOWSOUND;
extern long int currentIntensity;

Buffer::Buffer(int width1, int height1, double timescale1, int intensityscale1) {
    tab = new instant[width];
    width = width1;
    height = height1;
    timescale = timescale1;
    intensityscale = intensityscale1;
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
    int delay = timescale/width;
    tab[width/2].intensity = currentIntensity;
    usleep(delay*1000);
}


long int Buffer::getIntensity(int i) {
    return tab[i].intensity;
}

long int Buffer::getThresold() {
    return threshold;
}

Buffer::~Buffer() {
    delete[] tab;
}

