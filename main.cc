#include <iomanip>
#include <iostream>

#include "DynamicStats.h"
#include "System.h"

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
    system.updateNeighbors(1.0, 1.0);

    std::map<std::string, Gmr::Accumulator> acumulators;
    acumulators["magnetization"] = Gmr::Accumulator();
    acumulators["energy"] = Gmr::Accumulator();

    std::cout << std::setprecision(5) << std::fixed;

    double thermalEnergy = 12.0;
    
    while (thermalEnergy > 0)
    {
        int mcs = 1000;
        Gmr::DynamicStats statistics(5, 4.0, 1.0);

        for (auto&& acumulator : acumulators)
            acumulator.second.reset();

        for (int i = 0; i < mcs; ++i)
        {
            for(int j = 0; j < 10; j++)
                system.mcThermalStep(thermalEnergy);
            for(int j = 0; j < 10; j++) 
                system.mcDynamicStep ({Gmr::Specie::Electron}, thermalEnergy, statistics);
            
            system.updateNeighbors(1.0, 1.0);

            acumulators["energy"] += system.energy();
            acumulators["magnetization"] += system.magnetization();
        }

        std::cout  << std::setw(20) << thermalEnergy 
                   << std::setw(20) << acumulators["energy"].mean()
                   << std::setw(20) << acumulators["energy"].stddev()
                   << std::setw(20) << acumulators["magnetization"].mean()
                   << std::setw(20) << acumulators["magnetization"].stddev()
                   << std::setw(20) << statistics.mean(Gmr::Stat::wall)
                   << std::setw(20) << statistics.stddev(Gmr::Stat::wall)
                   // << std::setw(20) << statistics.mean(Gmr::Stat::tau)
                   // << std::setw(20) << statistics.stddev(Gmr::Stat::tau)
                   // << std::setw(20) << statistics.mean(Gmr::Stat::nu)
                   // << std::setw(20) << statistics.stddev(Gmr::Stat::nu)
                   // << std::setw(20) << statistics.mean(Gmr::Stat::distance)
                   // << std::setw(20) << statistics.stddev(Gmr::Stat::distance)
                   // << std::setw(20) << statistics.mean(Gmr::Stat::dx)
                   // << std::setw(20) << statistics.stddev(Gmr::Stat::dx)
                   // << std::setw(20) << statistics.mean(Gmr::Stat::dy)
                   // << std::setw(20) << statistics.stddev(Gmr::Stat::dy)
                   // << std::setw(20) << statistics.mean(Gmr::Stat::dz)
                   // << std::setw(20) << statistics.stddev(Gmr::Stat::dz)
                   << std::endl;
        thermalEnergy -= 0.05;
    }

    return 0;
}
