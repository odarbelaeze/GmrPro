#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <algorithm>
#include <functional>
#include <map>
#include <random>
#include <string>

#include "Accumulator.h"
#include "Deck.h"
#include "DynamicStats.h"
#include "Helpers.h"
#include "Particle.h"

namespace Gmr
{

    enum class Lattice { sc, fcc, bcc };

    class System
    {

    public:
        System(std::initializer_list<int> dim_list);
        System(std::vector<int> dimensions);
        ~System();

        void setDefaultParameters();

        void insertParticles (Specie, Lattice);
        void insertParticles (Specie, int);
        void updateNeighbors (double);
        void mcThermalStep (double);
        void mcThermalStep (std::initializer_list<Specie>, double);
        void mcDynamicStep (std::initializer_list<Specie>, double);
        void mcDynamicStep (std::initializer_list<Specie>, double, DynamicStats&);

        std::vector<int> getDimensions ();
        std::mt19937_64& getEngineRef ();
        std::vector<Particle> getParticles ();

        void setParameter (std::string, double);
        void setDimensions (std::vector<int>);
        void setEngine (std::mt19937_64&);
        void setParticles (std::vector<Particle>);
        
        double energy ();
        double magnetization ();

    protected:
        virtual double contribution_ (const Particle& particle);
        virtual double relatedEnergy_ (const Particle& particle);

        std::vector<int> dimensions_;
        std::vector<Particle> particles_;
        darray electricField_;
        darray magneticField_;
        std::map<std::string, double> parameters_;

        std::mt19937_64 engine_;
        std::uniform_real_distribution<> uniform_;

    };
}


#endif