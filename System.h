#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "Particle.h"
#include "Deck.h"

#include <random>
#include <algorithm>

namespace Gmr
{
    enum class Lattice { sc, fcc, bcc };

    class System
    {
    public:
        System(std::initializer_list<int> dim_list);
        System(std::vector<int> dimensions);
        ~System();

        void insertParticles (Specie, Lattice);
        void insertParticles (Specie, int);
        void updateNeighbors (double);

        void mcThermalStep (std::function<double(const Particle&)>,
                     double);

        void mcThermalStep (std::initializer_list<Specie>,
                     std::function<double(const Particle&)>,
                     double);

        void mcDynamicStep (std::initializer_list<Specie>,
                     std::function<double(const Particle&)>,
                     double);


        // getter y setter
        std::vector<int> getDimensions();
        std::mt19937_64& getEngineRef();
        std::vector<Particle> getParticles();

        void setDimensions(std::vector<int>);
        void setEngine(std::mt19937_64);
        void setParticles(std::vector<Particle>);
        
    private:
        std::vector<int> dimensions;
        std::mt19937_64 engine;

        std::vector<Particle> particles;
    };
}


#endif