#ifndef GMR_H_
#define GMR_H_

#include "Particle.h"
#include "Deck.h"

#include <random>
#include <algorithm>

namespace gmr
{
    typedef std::vector<Particle> particles_t;
    enum class Lattice { sc, fcc, bcc };

    void insertParticles (particles_t&, Specie, Lattice, std::initializer_list<int>);
    void insertParticles (particles_t&, Specie, int, std::initializer_list<int>);
    void updateNeighbors (particles_t&, double);

    void mcThermalStep (std::vector<Particle>&, 
                 std::function<double(const Particle&)>,
                 std::mt19937_64&,
                 double);

    void mcThermalStep (std::vector<Particle>&, 
                 std::initializer_list<Specie>,
                 std::function<double(const Particle&)>,
                 std::mt19937_64&,
                 double);

    void mcDynamicStep (std::vector<Particle>&,
                 std::initializer_list<int> dim_list,
                 std::initializer_list<Specie>,
                 std::function<double(const Particle&)>,
                 std::mt19937_64&,
                 double);

}


#endif