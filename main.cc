#include "GmrPro.h"

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
    std::initializer_list<int> dim_list({ 5, 5, 5 });
    std::mt19937_64 engine;

    Gmr::particles_t particles;
    Gmr::insertParticles (particles, Gmr::Specie::Ion, Gmr::Lattice::sc, dim_list);
    Gmr::insertParticles (particles, Gmr::Specie::Electron, 125, dim_list);
    Gmr::updateNeighbors(particles, 1.0);

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
            Gmr::updateNeighbors(particles, 1.0);

        for (int i = 0; i < mcs; ++i)
        {
            mcThermalStep(particles, Gmr::contribution, engine, thermalEnergy);
            mcDynamicStep (particles, dim_list, {Gmr::Specie::Electron}, 
                           Gmr::electricContribution, engine, thermalEnergy);

            acumulators["energy"] += energy(particles);
            acumulators["magnetization"] += magnetization(particles);
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
