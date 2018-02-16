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

bool developpement = true;

void fenetre_blanche() {
    sf::window.clear(sf::Color::White);
}
void fenetre_noire() {
    sf::window.clear(sf::Color::Black);
}
void flash(SerialPort &arduino)
{
    if(dev)
    {
        fenetre_blanche();
        fenetre_noire();
        cout << "Flash !" << endl;
    }
    else
        arduino.serialPrint("a");

}
