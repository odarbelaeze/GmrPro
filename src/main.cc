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

    std::cout << "# system zise:    " << gmrSystem.getParticles().size() << std::endl;

    for (int i = 0; i < 50; ++i)
    {
        gmrSystem.monteCarloThermalStep(false, false);
    }

    gmrSystem.setElectricField(Vector(1.0, 0.0, 0.0));
    gmrSystem.setThermalEnergy(0.5);

    for (int i = 0; i < 100; ++i)
    {
        gmrSystem.monteCarloDynamicStep(false, false);
    }

    return 0;
}