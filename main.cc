#include <iomanip>
#include <iostream>

#include "System.h"

template<typename T>
std::ostream& operator<< (std::ostream& os, std::valarray<T> vr)
{
    for (auto&& i : vr)
        os << i << " ";

    return os;
};

std::ostream& operator<< (std::ostream& os, Gmr::Spin spin)
{
    os << (spin == Gmr::Spin::Up? "Up" : "Down");
    return os;
};


int main(int argc, char const *argv[])
{
    Gmr::System system({ 5, 5, 5 });

    system.insertParticles (Gmr::Specie::Ion, Gmr::Lattice::sc);
    system.insertParticles (Gmr::Specie::Electron, 125);
    system.updateNeighbors(1.0);

    std::map<std::string, Gmr::Accumulator> acumulators;
    acumulators["magnetization"] = Gmr::Accumulator();
    acumulators["energy"] = Gmr::Accumulator();

    std::cout << std::setprecision(5) << std::fixed;

    double thermalEnergy = 12.0;
    while (thermalEnergy > 0)
    {
        int mcs = 1000;
        for (auto&& acumulator : acumulators)
            acumulator.second.reset();

        if (mcs % 10 == 0)
            system.updateNeighbors(1.0);

        for (int i = 0; i < mcs; ++i)
        {
            system.mcThermalStep(thermalEnergy);
            system.mcDynamicStep ({Gmr::Specie::Electron}, thermalEnergy);

            acumulators["energy"] += system.energy();
            acumulators["magnetization"] += system.magnetization();
        }
        std::cout  << std::setw(20) << thermalEnergy 
                   << std::setw(20) << acumulators["energy"].mean()
                   << std::setw(20) << acumulators["energy"].stddev()
                   << std::setw(20) << acumulators["magnetization"].mean()
                   << std::setw(20) << acumulators["magnetization"].stddev()
                   << std::endl;
        thermalEnergy -= 0.2;
    }

    return 0;
}
