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

Buffer::Buffer(sf::RenderWindow *givenWindow, int width1, int height1, double timescale1, long int intensityscale1) {
    width = width1;
    tab = new instant[width];
    for(int i=0; i<width; i++)
    {
        tab[i].intensity = 0;
        tab[i].peak = false;
    
    }
    height = height1;
    timeScale = timescale1;
    intensityScale = intensityscale1;
    window = givenWindow;
}

void Buffer::push(long int intensityValue) {
    if(intensityValue/height > intensityScale)
        intensityScale = intensityValue/height;
    for(int i=0; i<width/2+1; i++)
    {
        tab[i].intensity = tab[i+1].intensity;
        tab[i].peak = tab[i+1].peak;
    }
    tab[(int)width/2].intensity = intensityValue;
}

void Buffer::addPeak() {
    tab[width/2].peak = true;
}

void Buffer::refresh() {
    int delay = timeScale/width;
//    sf::Vertex line[2];
    sf::RectangleShape line;
    sf::RectangleShape peakLine;

    while (window->isOpen()) {
        push(currentIntensity);
        window->clear(sf::Color::Black);
        for (int k=0;k<width;k++) {
//            line[0] = sf::Vertex(sf::Vector2f(k, height/2));
//            line[1] = sf::Vertex(sf::Vector2f(k, height/2 + 30));
            line = sf::RectangleShape(sf::Vector2f(1, -tab[k].intensity/intensityScale));
            line.setPosition(k, height/2);
            if(tab[k].peak)
            {
                peakLine = sf::RectangleShape(sf::Vector2f(1, height));
                peakLine.setPosition(k, 0);
                peakLine.setFillColor(sf::Color(255, 0, 0));
                window->draw(peakLine);
            }
            window->draw(line);

        }
        window->display();
        usleep(10000);

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

void Buffer::refreshWindow()
{
    sf::Vertex line[2];
    int k = 0;
    while(window->isOpen())
    {
        line[0] = sf::Vertex(sf::Vector2f(k, height/2));
        line[1] = sf::Vertex(sf::Vector2f(k, height/2 + 20));


        window->clear(sf::Color::Black);

        window->draw(line, 2, sf::Lines);

        window->display();

        k += 2;
        k = k % 400;
        usleep(100000);
    }
}

Buffer::~Buffer() {
    delete[] tab;
}
