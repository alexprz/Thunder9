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
    
    
    
    void avgUpdate();
    void stdUpdate();
    void quickAvgStdUpdate(long int newData, long int oldData);
    long int getData(int i);
    long int getLastData();
    long int getOldData();
    void pushData(long int newData);
    
public:
    smoothedZScore(int givenLag, float givenThreshold, float givenInfluence);
    ~smoothedZScore();
    
    bool isPeak(long int newData);
    
    
};
