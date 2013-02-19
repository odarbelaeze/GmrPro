#include "gmrsystem.h"

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
    }

    std::cout << "# system zise: " << gmrSystem.getParticles().size() << std::endl;
    std::cout << "# initial energy: " << gmrSystem.getEnergy()  << std::endl;

    for (int i = 0; i < 10; ++i)
    {
        gmrSystem.monteCarloThermalStep(false, true);
    }

    return 0;
}