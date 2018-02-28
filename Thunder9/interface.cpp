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
#include <SFML/Graphics/VertexArray.hpp>
#include <thread>

extern sf::RenderWindow WINDOWSOUND;
extern long int currentIntensity;
extern bool over;

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
    while (not over) {
        push(currentIntensity);
        usleep(delay*1000);
        for (int i=0;i<width;i++) {
            sf::VertexArray lines(sf::LinesStrip, width);
            for (int j=0;j<width;j++) {
                lines[i].position = sf::Vector2f(i, height/2+tab[i].intensity);
            }
            WINDOWSOUND.draw(lines);
        }
    }
}

void Buffer::thread() {
    std::thread tRefresh(&Buffer::refresh, this);
    tRefresh.join();
    
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

