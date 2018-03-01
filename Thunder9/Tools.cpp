//
//  Tools.cpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#include <iostream>
#include "math.h"

using namespace std;

#include "Tools.hpp"

int bpmToUS(float bpm)
{
    return int(1000000*60/bpm);
}


smoothedZScore::smoothedZScore(int givenLag, float givenThreshold, float givenInfluence)
{
    lag = givenLag;
    threshold = givenThreshold;
    influence = givenInfluence;
    avg = 0.;
    std = 0.;
    
    data = new long int[lag]; //+1 for the new value
    length = 0;
    for(int i=0; i<lag; i++)
        data[i] = 0;
    
}

smoothedZScore::~smoothedZScore()
{
    delete[] data;
}

void smoothedZScore::avgUpdate()
{
    if(length == lag)
    {
        //Enough data to calculate avg
        avg = 0.;
        for(int i=0; i<lag; i++)
            avg += data[i];
        avg /= lag;
    }
    else
        avg = 0.;
}

void smoothedZScore::stdUpdate()
{
    if(length == lag)
    {
        //Enough data to calculate avg
        std = 0.;
        for(int i=0; i<lag; i++)
            std += pow(data[i] - avg, 2);
        std /= lag;
        std = sqrt(std);
    }
    else
        avg = 0.;
}

long int smoothedZScore::getData(int i)
{
    if(i>=0 && i<lag)
        return data[i];
    else
        return 0;
}

long int smoothedZScore::getLastData()
{
    return data[lag-1];
}

void smoothedZScore::pushData(long int newData)
{
    for(int i=0; i<lag; i++)
        data[i] = data[i+1];
    
    data[lag-1] = newData;
    if(length < lag)
        length += 1;
    
    avgUpdate();
    stdUpdate();
}

bool smoothedZScore::isPeak(long int newData)
{
    if(length < lag)
    {
        pushData(newData);
        cout << " Not enough data..." << endl;
        return false;   //Not enough data to analyse
    }
    else
    {
        if((newData - avg) > threshold*std)
        {
            //Since the value is far from the average
            //we reduce its influence over the other values
            pushData(influence*newData + (1-influence)*getLastData());
            return true;    //Its a peak !
        }
        else
        {
            //Normal influence because its in the average
            pushData(newData);
            return false;   //Its not a peak...
        }
    }
}
