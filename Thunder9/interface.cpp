//
//  interface.cpp
//  Thunder9
//
//  Created by Antoine Eudes on 28/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#include "interface.hpp"
#include "animation.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <SFML/Graphics/VertexArray.hpp>
#include <thread>

extern sf::RenderWindow WINDOWSOUND;
extern long int currentIntensity;
extern bool over;

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

void Buffer::push(int intensityValue) {
    for (int i=1;i<width;i++) {
        //tab[i-1] = tab[i];
    }
    //tab[(int)width/2].intensity = intensityValue;
}

void Buffer::addPeak() {
    tab[width/2].peak = true;
}

void Buffer::refresh() {
    int delay = timeScale/width;
    sf::Vertex line[2];

    while (not over) {
        push(currentIntensity);
        window->clear(sf::Color::Black);
        for (int k=0;k<width;k++) {
            line[0] = sf::Vertex(sf::Vector2f(k, height/2));
            line[1] = sf::Vertex(sf::Vector2f(k, height/2 + k%15));
            //std::cout << "Dessin" << std::endl;
            window->draw(line, 2, sf::Lines);
            window->display();
        }
        usleep(1000000);
        std::cout << "bite" << std::endl;

    }

}
// (getIntensity(k)*height/(2*intensityScale)
//        sf::VertexArray lines(sf::LinesStrip, width);
//        for (int i=0;i<width;i++) {
//
//            for (int j=0;j<width;j++) {
//                lines[i].position = sf::Vector2f(i, height/2+tab[i].intensity);
//            }
//            WINDOWSOUND.draw(lines);
//        }
//    }



void Buffer::thread() {
    std::thread tRefresh(&Buffer::refresh, this);
    tRefresh.join();

}

Buffer::~Buffer() {
    delete[] tab;
}
