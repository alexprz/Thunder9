//
//  animation.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include "animation.hpp"
#include <iostream>

extern bool dev;
extern sf::RenderWindow WINDOW;
extern SerialPort ARDUINO;


void fenetre_blanche() {
    WINDOW.clear(sf::Color::White);
    WINDOW.display();
}
void fenetre_noire() {
    WINDOW.clear(sf::Color::Black);
    WINDOW.display();
}
void flash()
{
    if(dev)
    {
        fenetre_blanche();
        usleep(100000);
        fenetre_noire();
        cout << "Flash !" << endl;
    }
    else
        ARDUINO.serialPrint("a");

}
