//
//  animation.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#include "animation.hpp"

bool developpement = true;

void fenetre_blanche() {
    window.clear(sf::Color::White);
}
void fenetre_noire() {
    window.clear(sf::Color::Black);
}
void flash(SerialPort &arduino)
{
    if (developpement) {
        fenetre_blanche();
        fenetre_noire();
    }
    arduino.serialPrint("a");
}
