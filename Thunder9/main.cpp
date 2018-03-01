//
//  main.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//


#include <iostream>
#include <ctime>
#include <thread>
#include <math.h>

#include <cstdlib>
#include <cstring>

using namespace std;

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "RtAudio.h"



#include "SerialPort.h"
#include "animation.hpp"
#include "Tools.hpp"
#include "interface.hpp"

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
long int currentIntensity = 0;
double recordTime = 0;
unsigned long int peakThreshold = 79404908;//259404908;  //Default threshold value
int minThreshold = 10000000;
float maxPercent = 0.4;

//RESOLUTION VARS
int peakResolution = 5000; //in us
int thresholdUpdaterResolution = 10000;

//DEV VARS
bool dev = true;
bool endFlashSimulator = false;

//WINDOW VAR
int width = 800;
int height = 600;



sf::RenderWindow WINDOW(sf::VideoMode(800, 600), "My window");
sf::RenderWindow WINDOWSOUND(sf::VideoMode(width, height), "Sound");
sf::View view(sf::FloatRect(0, 0, 800, 600));
//window.setView(view);

SerialPort *ARDUINO;
RtAudio ADC;
Buffer BUFF(&WINDOWSOUND, width, height, 6, peakThreshold);



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

void maxDetector(double duration, unsigned long int &max, bool &analyse)
{
    //Determines the maximum intensity during the specified time
    //and modifies the given max


    while(!over)
    {
        if(analyse)
        {
            cout << "Analysing..." << endl;

            max = 0;

            double initTime = time(nullptr);
            long int current;

            while(time(nullptr) - initTime < duration)
            {
                current = currentIntensity;
                if(abs(current) > max)
                {
                    max = abs(current);
                }
                usleep(10000);
            }

            peakThreshold = maxPercent*max;
            if(peakThreshold < minThreshold)
            {
                peakThreshold = minThreshold;
                cout << "Inferior bound !" << endl;
            }
            cout << "New threshold : " << peakThreshold << endl;
            //redFlash();
            analyse = false;
        }
        usleep(500000);
    }
}


void thresholdUpdater()
{
    unsigned long int max = 0;
    bool analyse = false;

    thread tMaxDetector(maxDetector, 5., ref(max), ref(analyse));

    //First analysis at the beginning
    analyse = true;

    double t0 = time(nullptr);

    while(!over)
    {
        if(time(nullptr) - t0 > 10.)
        {
            t0 = time(nullptr);
            analyse = true;
        }

        usleep(thresholdUpdaterResolution);
    }

    tMaxDetector.join();
}

void peakDetector()
{
    while(!over)
    {
        if(abs(currentIntensity) > peakThreshold)
        {
            triggerFlash = true;
            BUFF.addPeak();

        }

        usleep(peakResolution);
    }
}

void peakDetector2()
{
    //Using Smoothed Z Score algorithm
    smoothedZScore algo(50, 2.5, 1);
    while(!over)
    {
        if(algo.isPeak(currentIntensity))
        {
            triggerFlash = true;
            BUFF.addPeak();
        }
//        usleep(1000);
        usleep(1);
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

//    if (!sf::SoundBufferRecorder::isAvailable()){
//        //Si le micro n'est pas disponible
//        cout << " Micro non disponible..." << endl;
//        return false;
//    }

    //Vérification du micro
    if ( ADC.getDeviceCount() < 1 ) {
        std::cout << "\nNo audio devices found!\n";
        exit( 0 );
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
    RtAudio::StreamParameters parameters;
    parameters.deviceId = ADC.getDefaultInputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256; // 256 sample frames
    try {
        ADC.openStream( NULL, &parameters, RTAUDIO_SINT16,
                       sampleRate, &bufferFrames, &record );
        ADC.startStream();
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
        ADC.stopStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
    }
    if ( ADC.isStreamOpen() ) ADC.closeStream();
    return 0;
}

//void refreshWindow(sf::RenderWindow *myWindow)
//{
//    sf::Vertex line[2];
//    int k = 0;
//    while(myWindow->isOpen())
//    {
//        line[0] = sf::Vertex(sf::Vector2f(k, height/2));
//        line[1] = sf::Vertex(sf::Vector2f(k, height/2 + 20));
//
//
//        myWindow->clear(sf::Color::Black);
//
//        myWindow->draw(line, 2, sf::Lines);
//
//        myWindow->display();
//
//        k += 2;
//        k = k % 400;
//        usleep(10000);
//    }
//}

int main()
{
    WINDOWSOUND.setActive(false);


    sf::Thread thread(&Buffer::refresh, &BUFF);
    thread.launch();
//
//    sf::Vertex line[2];
//    int k = 0;
//    while (WINDOWTEST.isOpen())
//    {
//        // check all the window's events that were triggered since the last iteration of the loop
//        sf::Event event;
//        while (WINDOWTEST.pollEvent(event))
//        {
//            // "close requested" event: we close the window
//            if (event.type == sf::Event::Closed)
//            {
//                WINDOWTEST.close();
//            }
//        }
//
////        line[0] = sf::Vertex(sf::Vector2f(k, height/2));
////        line[1] = sf::Vertex(sf::Vector2f(k, height/2 + 20));
////
////        WINDOWTEST.clear(sf::Color::Black);
////
////        WINDOWTEST.draw(line, 2, sf::Lines);
////
////        WINDOWTEST.display();
////
////        k += 2;
////        k = k % 400;
//
//        usleep(100000);
//    }
//    sleep(3);

    if(!mainInit()) {
        //L'initialisation a échouée
        return -1;
    }

    std::thread t1(flashController);    //Trigger flash when told to
    std::thread listen(listening);      //Sound acquisition
    std::thread t2(thresholdUpdater);   //Real-time peakThreshold updater
    std::thread t3(peakDetector2);       //Real-time peak detection

    flash();
    sleep(1);

    //BUFFER.thread();


    //flash();
    // run the program as long as the window is open
    while (WINDOW.isOpen() || WINDOWSOUND.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        sf::Event soundEvent;
        while (WINDOW.pollEvent(event) || WINDOWSOUND.pollEvent(soundEvent))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                WINDOW.close();
                endFlashSimulator = true;
            }

            // "close requested" event: we close the window
            if (soundEvent.type == sf::Event::Closed)
            {
                WINDOWSOUND.close();
            }
        }
        usleep(10000);
    }

    over = true;
    t1.join();
    t2.join();
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
