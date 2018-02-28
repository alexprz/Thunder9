//
//  main.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//


#include <iostream>
#include <thread>
#include <math.h>

using namespace std;

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
// #include <SFML/Graphics.hpp>



#include "SerialPort.h"
#include "animation.hpp"
#include "Tools.hpp"

#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

bool triggerFlash = false;
bool over = false;
bool stopRecording = false;
bool stopPeakDetect = false;
std::string ARDUINO_PATH = "/dev/cu.usbmodem1411";

//RECORD VARS
int currentIntensity = 0;
double recordTime = 0;

//DEV VARS
bool dev = false;
bool endFlashSimulator = false;

sf::SoundBufferRecorder RECORDER; //Sert pour l'enregistrement
sf::RenderWindow WINDOW(sf::VideoMode(800, 600), "My window");
sf::RenderWindow WINDOWSOUND(sf::VideoMode(800, 600), "Sound");

SerialPort *ARDUINO;


void flashController()
{
    //SerialPort arduino("/dev/cu.usbmodem1411");

    while(!over)
    {
        if(triggerFlash)
        {
            flash();
            triggerFlash = false;
        }
        usleep(10000);
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
    int seuil = 0;
//    cout << "Ecoute du vide" << endl;
//    while (recordTime < 5.) {
//        int current = currentIntensity;
//        if(abs(current) > seuil)
//        {
//            cout << " up" << endl;
//            seuil = abs(current);
//            triggerFlash = true;
//        }
//    }
//    cout << "Fin" << endl;
    
    //seuil =159404908;
    seuil =159404908;
    
    cout << "Seuil : " << seuil << endl;
    
    while(!over)
    {
        if(abs(currentIntensity) > seuil)
        {
            triggerFlash = true;
        }
        
        usleep(5000);
    }
}

void facticePeakDetector()
{
    if(dev)
    {
        while(!endFlashSimulator)
        {
            triggerFlash = true;
            usleep(bpmToUS(94));
        }
    }
    else
    {
        for(int j = 0; j<100; j++)
        {
            cout << j << endl;
            triggerFlash = true;
            usleep(bpmToUS(76));
        }
    }
    
}

bool mainInit()
{
    if(!dev)
    {
        //Arduino qu'en production
        ARDUINO = new SerialPort("/dev/cu.usbmodem1411");

        if(!ARDUINO->isAvailable()){
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



int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *userData )
{
    if ( status )
        std::cout << "Stream overflow detected!" << std::endl;
    // Do something with the data in the "inputBuffer" buffer.
    //    int *buf = inputBuffer;
//    int max = 0;
    for (int i=0; i<nBufferFrames; i++) {
//        if ( abs(((int*)inputBuffer)[i]) > max)
//            max = abs(((int*)inputBuffer)[i]);
        currentIntensity = ((int*)inputBuffer)[i];
    }
//    currentIntensity = max;
    recordTime = streamTime;
    return 0;
}

int listening()
{
    RtAudio adc;
    if ( adc.getDeviceCount() < 1 ) {
        std::cout << "\nNo audio devices found!\n";
        exit( 0 );
    }
    RtAudio::StreamParameters parameters;
    parameters.deviceId = adc.getDefaultInputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256; // 256 sample frames
    try {
        adc.openStream( NULL, &parameters, RTAUDIO_SINT16,
                       sampleRate, &bufferFrames, &record );
        adc.startStream();
    }
    catch ( RtAudioError& e ) {
        e.printMessage();
        exit( 0 );
    }
    
    char input;
    std::cout << "\nRecording ... press <enter> to quit.\n";
    while(!over)
    {
        sleep(1);
    }
    try {
        // Stop the stream
        adc.stopStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
    }
    if ( adc.isStreamOpen() ) adc.closeStream();
    return 0;
}

int main()
{
    if(!mainInit()) {
        //L'initialisation a échouée
        return -1;
    }

    std::thread t1(flashController);
    //std::thread t2(facticePeakDetector);
    std::thread listen(listening);
    std::thread t3(peakDetector);

    flash();
    // run the program as long as the window is open
    while (WINDOW.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (WINDOW.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                WINDOW.close();
                endFlashSimulator = true;
            }
        }
        usleep(10000);
    }
    





    over = true;
    t1.join();
    //t2.join();
    t3.join();
    listen.join();
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



