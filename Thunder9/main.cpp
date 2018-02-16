//
//  main.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

//#include <iostream>
//
//int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    return 0;
//}
#include <iostream>
#include <thread>

using namespace std;

#include <SFML/Audio.hpp>
// #include <SFML/Graphics.hpp>



#include "SerialPort.h"
#include "animation.hpp"
#include "Tools.hpp"

bool triggerFlash = false;
bool over = false;
bool stopRecording = false;
bool dev = true;

sf::SoundBufferRecorder RECORDER; //Sert pour l'enregistrement
SerialPort ARDUINO;


void flashController()
{
    //SerialPort arduino("/dev/cu.usbmodem1411");

    while(!over)
    {
        if(triggerFlash)
        {
            flash(ARDUINO);
            triggerFlash = false;
        }
    }
}

void mainRecorder()
{
    RECORDER.start();
    while(!stopRecording){

    }
    RECORDER.stop();
}


void peakDetector()
{

}

bool mainInit()
{
    if(!dev)
    {
        //Arduino qu'en production

        ARDUINO = SerialPort("/dev/cu.usbmodem1411");

        if(!ARDUINO.isAvailable()){
            //La connexion à l'arduino a échouée
            cout << "La connexion à l'arduino a échouée..." << endl;
            return false;
        }
    }

    if (!sf::SoundBufferRecorder::isAvailable()){
        //Si le micro n'est pas disponible
        cout << " Micro non disponible..." << endl;
        return false;
    }


    return true;
}

int main()
{
    if(!mainInit()) {
        //L'initialisation a échouée
        return -1;
    }

    if(dev) {
        // sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    }

    std::thread t1(flashController);
    for(int j = 0; j<100; j++)
    {
        cout << j << endl;
        triggerFlash = true;
        usleep(bpmToUS(76));
    }
    over = true;
    t1.join();
    return 0;
}

//int main(){
//    if (sf::SoundBufferRecorder::isAvailable())
//    {
//        cout << "Avaiable";
//        // Record some audio data
//        sf::SoundBufferRecorder recorder;
//        recorder.start();
//        for(int i=0; i< 100000; i++)
//        {
//            cout << i << endl;
//        }
//        recorder.stop();
////         Get the buffer containing the captured audio data
//        const sf::SoundBuffer& buffer = recorder.getBuffer();
////         Save it to a file (for example...)
////        cout << buffer.getDuration();
//        const sf::Int16* samples = buffer.getSamples();
//        std::size_t count = buffer.getSampleCount();
//        cout << samples << endl << count << endl << endl;
//        for (int j=0; j<count; j++)
//        {
//            cout << samples[j] << endl;
//
//        }
//    }
//    else
//        cout << "Not avaiable" << endl;
//    cout << "Hello world !";
//    return 0;
//}
