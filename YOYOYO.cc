//YO QUIERO HACER QUE TOME ESOS VALORES, ME SAQUE LA DIFERENCIA, LAS SUME Y LAS
//DIVIDA POR EL TOTAL DEL TIEMPO.

#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include "simplevector.h"

int main(int argc, char const *argv[])
{
	float banderaEnergia;
	float banderatiempo;


	float time_i;
	float time_f = 0;
	float deltaTime = 0;
	float deltaEnergy = 0;
	float thermalEnergy = 10;
	float energy;
	Vector initial;
	Vector last;
	float deltaX;
	float dist;//vendría a ser la suma de los deltaX

	bool empezando =true;

    float lastThermalEnergy = -1.0;
    float actualThermalEnergy;
    float actualEnergy;

    while(!std::cin.eof())
    {
    	time_i = time_f;

    	banderaEnergia = thermalEnergy;

        scanf("%f   %f    %f    (%f, %f, %f)    (%f, %f, %f)\n",
              &time_f, &energy, &thermalEnergy,
              &initial[0], &initial[1], &initial[2],
              &last[0], &last[1], &last[2]);

        if (time_i == 0)
        {
        	time_i = time_f;
        }

    	deltaTime += (time_f - time_i);
        

        if (banderaEnergia == thermalEnergy)
        {
	        deltaX += norm(last - initial);
        }
        else
    	{
    		std::cout << deltaX	/ deltaTime << "\t" << banderaEnergia << std::endl;
    		deltaTime = 0;
    		deltaX += norm(last - initial);
    		// time_i = time_f;
    	}
        
    }
	return 0;
}