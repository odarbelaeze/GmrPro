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
        double contribution(const Particle& particle);

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


        // getter y setter
        std::vector<int> getDimensions();
        std::mt19937_64& getEngineRef();
        std::vector<Particle> getParticles();

        void setDimensions(std::vector<int>);
        void setEngine(std::mt19937_64);
        void setParticles(std::vector<Particle>);
        

        double distance (const darray& a, const darray& b) {
        return std::sqrt(std::pow(b - a, 2).sum());
        };

        double K (const Particle& particle, const Particle& other) {
        return parameters["K_0"] * std::exp(
            - distance(particle.getPosition(), other.getPosition())
            );
        };

        double I (const Particle& particle, const Particle& other) {
            return parameters["I_0"] * std::exp( 
                - distance(particle.getPosition(), other.getPosition())
            );
        };

        double relatedEnergy(const Particle& particle);

        double energy (const std::vector<Particle> particles){
            double energy = 0;
            for (auto&& particle : particles)
                energy += contribution(particle);
            return energy;
        };


        double magnetization (const std::vector<Particle>& particles){
            float magnetization = 0;
            for (auto&& particle : particles)
            {
                magnetization += particle.getSpin() == Spin::Up ? 1.0: -1.0;
            }
            return std::abs(magnetization / particles.size());
        };

    private:
        std::vector<int> dimensions;
        std::mt19937_64 engine;

        std::vector<Particle> particles;
        darray electricField;
        
        std::map<std::string, double> parameters;

    };
}


#endif