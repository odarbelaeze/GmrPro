#include "gmrsystem.h"

#define REP(k,a) for(int k=0; k < (a); ++k)

int main(int argc, char const *argv[])
{
    GmrSystem gmrSystem;

    try
    {
        if(argc >= 2) gmrSystem = GmrSystem(argv[1]);
        else gmrSystem = GmrSystem("docs/descriptor_example.json");
    }
    catch (BadDescriptorException& bde)
    {
        std::cout << bde.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "# Finding a ground state to start..." << std::endl;

    REP(i, 20)
    {
        REP(j, 10) gmrSystem.monteCarloThermalStep(false, false);
        REP(j, 10) gmrSystem.monteCarloDynamicStep((j == 9), false);
    }

    std::cout << "# OK" << std::endl;
    std::cout << "# Setting up conditions..." << std::endl;

    gmrSystem.setElectricField(Vector(250.0, 0.0, 0.0));

    std::cout << "# OK" << std::endl;
    std::cout << "# Simulating..." << std::endl;

    REP(i, 500)
    {
        REP(j, 10) gmrSystem.monteCarloThermalStep(false, false);
        REP(j, 10) 
        {
            gmrSystem.monteCarloDynamicStep((j == 9), true);
            Vector totalDisplacement = gmrSystem.collectDisplacementData();
            std::cout << gmrSystem.getThermalEnergy() << "    "
                      << totalDisplacement[0]         << "    "
                      << totalDisplacement[1]         << "    "
                      << totalDisplacement[2]         << std::endl;
        }
        gmrSystem.setThermalEnergy(gmrSystem.getThermalEnergy() + 1.0);
    }

    std::cout << "# OK" << std::endl;

    return 0;
}
