//
//  Tools.hpp
//  Thunder9
//
//  Created by Alexandre PEREZ on 15/02/2018.
//  Copyright © 2018 Alexandre PEREZ. All rights reserved.
//

#ifndef Tools_hpp
#define Tools_hpp

#include <stdio.h>

#endif /* Tools_hpp */

int bpmToUS(float bpm);

class smoothedZScore
{
private:
    long int *data;
    int length;
    int lag;
    float threshold;
    float influence;
    double avg;
    double std;
    
    
    
public:
    smoothedZScore(int givenLag, float givenThreshold, float givenInfluence);
    ~smoothedZScore();
    void avgUpdate();
    void stdUpdate();
    long int getData(int i);
    long int getLastData();
    void pushData(long int newData);
    bool isPeak(long int newData);
    
    
};
