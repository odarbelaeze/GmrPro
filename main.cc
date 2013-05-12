#include "GmrPro.h"
#include <iostream>

template<typename T>
std::ostream& operator<< (std::ostream& os, std::valarray<T> vr)
{
    for (auto&& i : vr)
        os << i << " ";

    return os;
}

std::ostream& operator<< (std::ostream& os, Gmr::Spin spin)
{
    os << (spin == Gmr::Spin::Up? "Up" : "Down");
    return os;
}


int main(int argc, char const *argv[])
{
    Gmr::System NUEVO({ 5, 5, 5 });
    // std::initializer_list<int> dim_list({ 5, 5, 5 });
    // std::mt19937_64 engine;

    // Gmr::particles_t particles;
    NUEVO.insertParticles (Gmr::Specie::Ion, Gmr::Lattice::sc);
    NUEVO.insertParticles (Gmr::Specie::Electron, 125);
    NUEVO.updateNeighbors(1.0);

    std::map<std::string, Gmr::Accumulator> acumulators;
    acumulators["magnetization"] = Gmr::Accumulator();
    acumulators["energy"] = Gmr::Accumulator();

    std::cout << std::setprecision(5) << std::fixed;

    double thermalEnergy = 30.0;
    while (thermalEnergy > 0)
    {
        int mcs = 100;
        for (auto&& acumulator : acumulators)
            acumulator.second.reset();

        if (mcs % 10 == 0)
            NUEVO.updateNeighbors(1.0);

        for (int i = 0; i < mcs; ++i)
        {
            NUEVO.mcThermalStep(Gmr::contribution, thermalEnergy);
            NUEVO.mcDynamicStep ({Gmr::Specie::Electron}, Gmr::electricContribution, thermalEnergy);

            acumulators["energy"] += energy(NUEVO.getParticles());
            acumulators["magnetization"] += magnetization(NUEVO.getParticles());
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
