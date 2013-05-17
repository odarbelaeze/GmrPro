#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <iostream>
#include <iomanip>
#include <functional>
#include <map>
#include <string>
#include <random>
#include <algorithm>

#include "Particle.h"
#include "Accumulator.h"
#include "Deck.h"

namespace Gmr
{
    template<typename T>
    std::ostream& operator<< (std::ostream& os, std::valarray<T> vr)
    {
        for (auto&& i : vr)
            os << i << " ";

        return os;
    }

    enum class Lattice { sc, fcc, bcc };

    class System
    {
    protected:
        virtual double contribution(const Particle& particle);

    public:
        System(std::initializer_list<int> dim_list);
        System(std::vector<int> dimensions);
        ~System();

        void insertParticles (Specie, Lattice);
        void insertParticles (Specie, int);
        void updateNeighbors (double);
        void mcThermalStep (double);
        void mcThermalStep (std::initializer_list<Specie>, double);
        void mcDynamicStep (std::initializer_list<Specie>, double);
        double distance (const darray&, const darray&);
        double energy (const std::vector<Particle>);
        double magnetization (const std::vector<Particle>&);

        std::vector<int> getDimensions();
        std::mt19937_64& getEngineRef();
        std::vector<Particle> getParticles();

        void setDefaultParameters();
        void setParameter(std::string, double);
        void setDimensions(std::vector<int>);
        void setEngine(std::mt19937_64);
        void setParticles(std::vector<Particle>);
        

    private:
        double relatedEnergy(const Particle& particle);

        std::vector<int> dimensions;
        std::mt19937_64 engine;

        std::vector<Particle> particles;
        darray electricField;
        
        std::map<std::string, double> parameters;

    };
}


#endif