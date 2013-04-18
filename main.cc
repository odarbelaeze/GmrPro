#include "Gmr.h"

#include <iostream>
#include <iomanip>

template<typename T>
std::ostream& operator<< (std::ostream& os, std::valarray<T> vr)
{
    for (auto&& i : vr)
        os << i << " ";

    return os;
}

std::ostream& operator<< (std::ostream& os, gmr::Spin spin)
{
    os << (spin == gmr::Spin::Up? "Up" : "Down");
    return os;
}


int main(int argc, char const *argv[])
{
    // srand(time(NULL));

    std::mt19937_64 engine;

    gmr::particles_t particles;
    gmr::insertParticles (particles, gmr::Specie::Ion, gmr::Lattice::sc, { 5, 5, 5 });
    gmr::insertParticles (particles, gmr::Specie::Electron, 125, { 5, 5, 5 });
    gmr::updateNeighbors(particles, 1.0);

    double Jex = 1.0;

    auto contribution = [&Jex](const gmr::Particle& particle){
        double contribution = 0;
        for (auto&& other : particle.getNbh())
            contribution -= Jex * (particle.getSpin() * other -> getSpin());
        return contribution;
    };

    auto energy = [&contribution](const gmr::particles_t particles){
        double energy = 0;
        for (auto&& particle : particles)
            energy += contribution(particle);
        return energy;
    };

    auto magnetization = [](const gmr::particles_t& particles){
        float magnetization = 0;
        for (auto&& particle : particles)
        {
            magnetization += particle.getSpin() == gmr::Spin::Up ? 1.0: -1.0;
        }
        return std::abs(magnetization / particles.size());
    };

    std::cout << std::setprecision(5) << std::fixed;

    double thermalEnergy = 30.0;
    while (thermalEnergy > 0)
    {
        // Your code here
        double e, m;
        double eacum = 0.0;
        double macum = 0.0;
        double eacumsq = 0.0;
        double macumsq = 0.0;
        int mcs = 1000;
        for (int i = 0; i < mcs; ++i)
        {
            mcThermalStep(particles, contribution, engine, thermalEnergy);
            double e = energy(particles);
            double m = magnetization(particles);
            eacum += e;
            macum += m;
            eacumsq += e * e;
            macumsq += m * m;
        }
        std::cout  << std::setw(20) << thermalEnergy 
                   << std::setw(20) << eacum / mcs
                   << std::setw(20) << std::sqrt(eacumsq / mcs - std::pow(eacum / mcs, 2))
                   << std::setw(20) << macum / mcs 
                   << std::setw(20) << std::sqrt(macumsq / mcs - std::pow(macum / mcs, 2)) 
                   << std::endl;
        thermalEnergy -= 0.2;
    }

    return 0;
}
