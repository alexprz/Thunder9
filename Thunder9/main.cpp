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
long int currentIntensity = 0;
double recordTime = 0;
unsigned long int peakThreshold = 79404908;//259404908;  //Default threshold value
float maxPercent = 0.2;

//RESOLUTION VARS
int peakResolution = 5000; //in us
int thresholdUpdaterResolution = 10000;

//DEV VARS
bool dev = true;
bool endFlashSimulator = false;

//sf::SoundBufferRecorder RECORDER; //Sert pour l'enregistrement
sf::RenderWindow WINDOW(sf::VideoMode(800, 600), "My window");
sf::RenderWindow WINDOWSOUND(sf::VideoMode(800, 600), "Sound");
SerialPort *ARDUINO;
RtAudio ADC;
//Buffer BUF;


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

//void mainRecorder()
//{
//    RECORDER.start();
//    while(!stopRecording){
//
//    }
//    RECORDER.stop();
//}


void maxDetector(double duration, unsigned long int &max, bool &analyse)
{
    //Determines the maximum intensity during the specified time
    //and modifies the given max
    
    
    while(!over)
    {
        if(analyse)
        {
            cout << "Listening max..." << endl;
            
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
            }
            
            cout << "Default max : " << max << endl;
            
            peakThreshold = maxPercent*max;
            cout << "New threshold : " << max << endl;
            
            analyse = false;
        }
        usleep(500000);
    }
}


void thresholdUpdater()
{
    unsigned long int max = 0;
    bool analyse = false;
    long int current;
    
    thread tMaxDetector(maxDetector, 3., ref(max), ref(analyse));
    
    analyse = true;
    
    cout << "Updating threshold..." << endl;
    
    while(!over)
    {
        current = currentIntensity;
        
        //Max & Threshold refresh
        if(abs(current) > max)
        {
            max = abs(current);
            peakThreshold = maxPercent*max;
            cout << "New threshold : " << peakThreshold << endl;
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
        }
        
        usleep(peakResolution);
    }
}

//void facticePeakDetector()
//{
//    if(dev)
//    {
//        while(!endFlashSimulator)
//        {
//            triggerFlash = true;
//            usleep(bpmToUS(94));
//        }
//    }
//    else
//    {
//        for(int j = 0; j<100; j++)
//        {
//            cout << j << endl;
//            triggerFlash = true;
//            usleep(bpmToUS(76));
//        }
//    }
//
//}

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

int main()
{
    if(!mainInit()) {
        //L'initialisation a échouée
        return -1;
    }

    std::thread t1(flashController);
    //std::thread t2(facticePeakDetector);
    std::thread listen(listening);
    std::thread t2(thresholdUpdater);
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



