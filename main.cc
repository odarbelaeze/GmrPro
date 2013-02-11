#include "GmrPro.h"
#include <cassert>

int main(int argc, char const *argv[])
{
    Vector electricField;
    System system;

    try
    {    
        if (argc == 1)
        {
            system = System("docs/descriptor_example.json");
        }
        else
        {
            system = System(argv[1]);
        }
    }
    catch (BadDescriptorException& e)
    {
        std::cout << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    system.addField("Electric", electricField);

    for (int i = 0; i < 5; ++i)
    {
        system.monteCarloThermalStep(false);
        std::cout << system.computeEnergy() << std::endl;
    }

    return 0;
}