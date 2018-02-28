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

Buffer::Buffer(int width1, int height1, int timescale1, int intensityscale1) {
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

void Buffer::addPeak(bool peakpresence) {
    if (peakpresence) {
        tab[width/2].peak = true;
    }
}

void Buffer::refresh() {
    int delay = timescale/width;
    
    usleep(delay*1000);
}
