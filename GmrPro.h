#ifndef GMR_PRO_H_
#define GMR_PRO_H_

#include <iostream>
#include <iomanip>
#include <functional>
#include <map>
#include <string>
#include <random>

#include "Gmr.h"
#include "Accumulator.h"

namespace Gmr
{
    template<typename T>
    std::ostream& operator<< (std::ostream& os, std::valarray<T> vr)
    {
        for (auto&& i : vr)
            os << i << " ";

        return os;
    }

    double distance (const darray& a, const darray& b) {
        return std::sqrt(std::pow(b - a, 2).sum());
    }

    std::map<std::string, double> parameters {
        { "Jex", 1.0 },
        { "I_0", 1.0 },
        { "K_0", 2.0 },
        { "R_0", 0.001 }
    };

    darray electricField({ 0.0, 0.0, 0.0 });

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

    double contribution (const Particle& particle){
        double contribution = 0;
        #ifndef _SPECIE
        #define _SPECIE(p, sp) (p).getSpecie() == Specie::sp
        #endif

        for (auto&& other : particle.getNbh())
        {
            contribution += exp( 
                - distance(particle.getPosition(), other -> getPosition())
                / parameters["R_0"]
            );

            if (_SPECIE(particle, Ion) && _SPECIE(*other, Ion))
                contribution -= parameters["Jex"] * 
                                (particle.getSpin() * other -> getSpin());

            else if ((_SPECIE(particle, Ion) && _SPECIE(*other, Electron))
                     || (_SPECIE(particle, Electron) && _SPECIE(*other, Ion)))
                contribution -= I(particle, *other) * 
                                (particle.getSpin() * other -> getSpin());

            else if (_SPECIE(particle, Electron) && _SPECIE(*other, Electron))
                contribution -= K(particle, *other) * 
                                (particle.getSpin() * other -> getSpin());
        }

        #undef _SPECIE
        return contribution;
    };

    double electricContribution (const Particle& particle){
        double contribution = 0;
        #ifndef _SPECIE
        #define _SPECIE(p, sp) (p).getSpecie() == Specie::sp
        #endif

        for (auto&& other : particle.getNbh())
        {
            contribution += exp( 
                - distance(particle.getPosition(), other -> getPosition())
                / parameters["R_0"]
            );

            if ((_SPECIE(particle, Ion) && _SPECIE(*other, Electron))
                     || (_SPECIE(particle, Electron) && _SPECIE(*other, Ion)))
            {
                contribution -= I(particle, *other) * (particle.getSpin() * other -> getSpin());
            }
            else if (_SPECIE(particle, Electron) && _SPECIE(*other, Electron))
            {
                contribution -= K(particle, *other) * (particle.getSpin() * other -> getSpin());
            }
        }

        contribution -= particle.getCharge() * (electricField * particle.getPosition()).sum();

        #undef _SPECIE
        return contribution;
    };

    double energy (const particles_t particles){
        double energy = 0;
        for (auto&& particle : particles)
            energy += contribution(particle);
        return energy;
    };

    double magnetization (const particles_t& particles){
        float magnetization = 0;
        for (auto&& particle : particles)
        {
            magnetization += particle.getSpin() == Spin::Up ? 1.0: -1.0;
        }
        return std::abs(magnetization / particles.size());
    };

}

#endif