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
    // tipo de estructuras que puede tener el sistema
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
        void updateNeighbors (double, double);
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

        std::map<double, int> getParedes();

    protected:
        virtual double contribution_ (const Particle& particle);
        virtual double relatedEnergy_ (const Particle& particle);

        // contiene las dimensiones del sistema
        std::vector<int> dimensions_;

        // vector que contiene todas las particulas que constituyen el sistema
        std::vector<Particle> particles_;

        // es un std::valarray<double> con las componentes del campo electrico
        darray electricField_;

        // es un std::valarray<double> con las componentes del campo magnetico
        darray magneticField_;

        // mapa de string a double con los parametros propios del sistema
        // (como las constantes y esas cosas)
        std::map<std::string, double> parameters_;

        // motor de numeros aleatorios y distribucion uniforme
        std::mt19937_64 engine_;
        std::uniform_real_distribution<> uniform_;

        std::map<double, int> paredes_;

    };
}


#endif