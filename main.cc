#include "Gmr.h"

#include <iostream>

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

    gmr::particles_t particles;
    gmr::insertParticles (particles, gmr::Specie::Ion, 1000, { 5, 5, 5 });
    gmr::updateNeighbors(particles, 10);
    for (auto&& particle : particles)
        std::cout << particle.getPosition() << " " << particle.getSpin() << "  " << particle.getNbh().size() << std::endl;
    return 0;
}
