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

using namespace std;

#include <SFML/Audio.hpp>

#include "SerialPort.h"
#include "animation.hpp"
#include "Tools.hpp"

#include <thread>
#include <iostream>

bool triggerFlash = false;
bool over = false;

void severalFlash()
{
    SerialPort arduino("/dev/cu.usbmodem1411");

    while(!over)
    {
        if(triggerFlash)
        {
            flash(arduino);
            triggerFlash = false;
        }
    }
}

int main()
{
    std::thread t1(severalFlash);
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

