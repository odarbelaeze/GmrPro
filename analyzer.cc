#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include "simplevector.h"

int main(int argc, char const *argv[])
{
    Vector totalDelta;
    Vector startPoint;
    Vector endPoint;

    float lastThermalEnergy = -1.0;
    float actualThermalEnergy;
    unsigned long int actualTime;
    unsigned long int lastTime;
    float actualEnergy;

    while(!std::cin.eof())
    {
        scanf("%lu   %f    %f    (%f, %f, %f)    (%f, %f, %f)\n",
              &actualTime, &actualEnergy, &actualThermalEnergy,
              &startPoint[0], &startPoint[1], &startPoint[2],
              &endPoint[0], &endPoint[1], &endPoint[2]);
        if (lastTime == actualTime)
        {
            std::cout << totalDelta << std::endl;
            break;  
        }
        
        if (lastThermalEnergy == -1.0) lastThermalEnergy = actualThermalEnergy;
        
        if (lastThermalEnergy == actualThermalEnergy)
        {
            totalDelta = totalDelta + endPoint - startPoint;
        }
        else
        {
            std::cout << totalDelta << std::endl;
            totalDelta = Vector(0,0,0);
        }

        lastTime = actualTime;
    }

    return 0;
}