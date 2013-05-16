#include <cmath>

#include "System.h"

namespace Gmr
{
    System::System(std::initializer_list<int> dim_list)
    {
        if (std::any_of(std::begin(dim_list), std::end(dim_list), [](int i) { return i <= 0; }))
            throw std::exception();

        // std::vector<int> dim(dim_list);
        // dimensions = dim;
        dimensions = std::vector<int>(dim_list);
        electricField = darray({ 0.0, 0.0, 0.0 });

        parameters = std::map<std::string, double>{
            { "Jex", 1.0 },
            { "I_0", 1.0 },
            { "K_0", 2.0 },
            { "R_0", 0.001 } };
    }

    System::System(std::vector<int> dimensions)
    {
        this -> dimensions = dimensions;
        electricField = darray({ 0.0, 0.0, 0.0 });

        parameters = std::map<std::string, double>{
            { "Jex", 1.0 },
            { "I_0", 1.0 },
            { "K_0", 2.0 },
            { "R_0", 0.001 } };
    }

    System::~System(){}


    double System::contribution (const Particle& particle){
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
                contribution -= I(particle, *other) * (particle.getSpin() * other -> getSpin());

            else if (_SPECIE(particle, Electron) && _SPECIE(*other, Electron))
                contribution -= K(particle, *other) * (particle.getSpin() * other -> getSpin());
        }

        contribution -= particle.getCharge() * (electricField * particle.getPosition()).sum();

        #undef _SPECIE
        return contribution;
    }

    double System::relatedEnergy(const Particle& particle){
        double energy = this -> contribution(particle);
        for (auto&& other : particle.getNbh())
            energy += this -> contribution(*other);
        return energy;
    }

    void System::insertParticles (Specie specie, Lattice lattice)
    {

        std::vector<darray> basis;

        switch (lattice)
        {
            case Lattice::sc :
                basis = { 
                    darray({ 0.0, 0.0, 0.0 })
                };
                break;
                
            case Lattice::fcc :
                basis = {
                    darray({ 0.0, 0.0, 0.0 }),
                    darray({ 0.5, 0.5, 0.0 }),
                    darray({ 0.5, 0.0, 0.5 }),
                    darray({ 0.0, 0.5, 0.5 })
                };
                break;
                
            case Lattice::bcc : 
                basis = {
                    darray({ 0.0, 0.0, 0.0 }),
                    darray({ 0.5, 0.5, 0.5 })
                };
                break;
                
            default:
                return;
        }

        Deck<Spin> spins;
        spins.push(Spin::Up);
        spins.push(Spin::Down);

        for (int i = 0; i < dimensions[0]; ++i)
        {
            for (int j = 0; j < dimensions[1]; ++j)
            {
                for (int k = 0; k < dimensions[2]; ++k)
                {
                    for (auto position : basis)
                        particles.push_back(Particle(
                            darray({ (double) i, (double) j, (double) k }) + position,
                            spins.sample(),
                            specie
                        ));
                }
            }
        }
    }

    void System::insertParticles (Specie specie, int count)
    {
        Deck<Spin> spins;
        spins.push(Spin::Up);
        spins.push(Spin::Down);

        std::uniform_real_distribution<> x(0.0, (double) dimensions[0]);
        std::uniform_real_distribution<> y(0.0, (double) dimensions[1]);
        std::uniform_real_distribution<> z(0.0, (double) dimensions[2]);

        for (int i = 0; i < count; ++i)
        {
            particles.push_back(Particle(
                darray({ x(engine), y(engine), z(engine) }),
                spins.sample(),
                specie
            ));
        }
    }

    void System::updateNeighbors (double radius)
    {
        for (auto&& particle : particles)
        {
            std::vector<Particle*> vecinitos;
            for (auto&& other : particles)
            {
                if (&other != &particle 
                    && sqrt(pow(particle.getPosition() - other.getPosition(), 2).sum()) <= radius)
                    vecinitos.push_back(&other);
            }
            particle.setNbh(vecinitos);
        }
    }

    void System::mcThermalStep (double thermalEnergy)
    {
        // std::function<double(const Particle&)> relatedEnergy = 
        //     [&contribution](const Particle& particle) {
        //         double energy = contribution(particle);
        //         for (auto&& other : particle.getNbh())
        //             energy += contribution(*other);
        //         return energy;
        //     };
        
        Deck<Particle*> targets;
        for (auto& particle : particles)
            targets.push(&particle);

        std::uniform_real_distribution<> distribution(0.0, 1.0);

        while (!targets.isEmpty())
        {
            Particle* particle = targets.pop();
            double oldEnergy = this -> relatedEnergy(*particle);
            particle -> setSpin( - particle -> getSpin());
            double energyDelta = this -> relatedEnergy(*particle) - oldEnergy;
            if (distribution(engine) > std::exp( - energyDelta / thermalEnergy))
                particle -> setSpin( - particle -> getSpin());
        }
    }

    void System::mcThermalStep (std::initializer_list<Specie> targetSp,
                     double thermalEnergy)
    {
        // std::function<double(const Particle&)> relatedEnergy = 
        //     [&contribution](const Particle& particle) {
        //         double energy = contribution(particle);
        //         for (auto&& other : particle.getNbh())
        //             energy += contribution(*other);
        //         return energy;
        //     };
        
        Deck<Particle*> targets;
        // use std::any_of(begin, end, function -> bool)
        for (auto&& particle : particles)
            if (std::any_of(begin(targetSp), end(targetSp), [&particle](Specie sp){
                return particle.getSpecie() == sp;
            })) targets.push(&particle);

        std::uniform_real_distribution<> distribution(0.0, 1.0);

        while (!targets.isEmpty())
        {
            Particle* particle = targets.pop();
            double oldEnergy = this -> relatedEnergy(*particle);
            particle -> setSpin( - particle -> getSpin());
            double energyDelta = this -> relatedEnergy(*particle) - oldEnergy;
            if (distribution(engine) > std::exp( - energyDelta / thermalEnergy))
                particle -> setSpin( - particle -> getSpin());
        }
    }

    void System::mcDynamicStep (std::initializer_list<Specie> targetSp,
                     double thermalEnergy)
    {
        // std::function<double(const Particle&)> relatedEnergy = 
        //     [&contribution](const Particle& particle) {
        //         double energy = contribution(particle);
        //         for (auto&& other : particle.getNbh())
        //             energy += contribution(*other);
        //         return energy;
        //     };

        std::uniform_real_distribution<> theta(0.0, M_PI);
        std::uniform_real_distribution<> phi(0.0, 2.0 * M_PI);

        auto randVec3D = [&theta, &phi, &engine](){
            double t = theta(engine)
                 , p = phi(engine);
            return darray({
                std::sin(t) * std::cos(p),
                std::sin(t) * std::sin(p),
                std::cos(t)
            });
        };

        Deck<Particle*> targets;
        // use std::any_of(begin, end, function -> bool)
        for (auto&& particle : particles)
            if (std::any_of(begin(targetSp), end(targetSp), [&particle](Specie sp){
                return particle.getSpecie() == sp;
            })) targets.push(&particle);

        std::uniform_real_distribution<> distribution(0.0, 1.0);

        while (!targets.isEmpty())
        {
            Particle* particle = targets.pop();
            double oldEnergy = this -> relatedEnergy(*particle);
            darray oldPosition = particle -> getPosition();
            particle -> setPosition(oldPosition + randVec3D());
            double energyDelta = this -> relatedEnergy(*particle) - oldEnergy;
            if (distribution(engine) > std::exp( - energyDelta / thermalEnergy))
                particle -> setPosition(oldPosition);
            else
                particle -> setPosition(fmod(particle -> getPosition(), std::initializer_list<int>({dimensions[0],dimensions[1],dimensions[2]})));
        }
    }

    std::vector<int> System::getDimensions()
    {
        return dimensions;
    }

    std::mt19937_64& System::getEngineRef()
    {
        return engine;
    }

    std::vector<Particle> System::getParticles()
    {
        return particles;
    }


    void System::setDimensions(std::vector<int> dimensions)
    {
        this -> dimensions = dimensions;
    }

    void System::setEngine(std::mt19937_64)
    {
        this -> engine = engine;
    }

    void System::setParticles(std::vector<Particle> particles)
    {
        this -> particles = particles;
    }

}