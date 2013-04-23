#include "Gmr.h"
#include <cmath>
#include <iostream>
namespace gmr
{
    void insertParticles (particles_t& particles, Specie specie, 
                          Lattice lattice, std::initializer_list<int> dim_list)
    {
        if (std::any_of(std::begin(dim_list), std::end(dim_list), [](int i) { return i <= 0; }))
            throw std::exception();

        std::vector<int> dimensions(dim_list);

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

    void insertParticles (particles_t& particles, Specie specie, 
                          int count, std::initializer_list<int> dim_list)
    {
        if (std::any_of(begin(dim_list), end(dim_list), [](int i) { return i <= 0; }))
            throw std::exception();

        std::vector<int> dimensions(dim_list);

        Deck<Spin> spins;
        spins.push(Spin::Up);
        spins.push(Spin::Down);

        std::mt19937_64 engine;
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

    void updateNeighbors (particles_t& particles, double radius)
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

    void mcThermalStep (std::vector<Particle>& particles, 
                        std::function<double(const Particle&)> contribution,
                        std::mt19937_64& engine,
                        double thermalEnergy)
    {

        std::function<double(const Particle&)> relatedEnergy = 
            [&contribution](const Particle& particle) {
                double energy = contribution(particle);
                for (auto&& other : particle.getNbh())
                    energy += contribution(*other);
                return energy;
            };
        
        Deck<Particle*> targets;
        for (auto& particle : particles)
            targets.push(&particle);

        std::uniform_real_distribution<> distribution(0.0, 1.0);

        while (!targets.isEmpty())
        {
            Particle* particle = targets.pop();
            double oldEnergy = relatedEnergy(*particle);
            particle -> setSpin( - particle -> getSpin());
            double energyDelta = relatedEnergy(*particle) - oldEnergy;
            if (distribution(engine) > std::exp( - energyDelta / thermalEnergy))
                particle -> setSpin( - particle -> getSpin());
        }
    }

    void mcThermalStep (std::vector<Particle>& particles, 
                 std::initializer_list<Specie> targetSp,
                 std::function<double(const Particle&)> contribution,
                 std::mt19937_64& engine,
                 double thermalEnergy)
    {
        std::function<double(const Particle&)> relatedEnergy = 
            [&contribution](const Particle& particle) {
                double energy = contribution(particle);
                for (auto&& other : particle.getNbh())
                    energy += contribution(*other);
                return energy;
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
            double oldEnergy = relatedEnergy(*particle);
            particle -> setSpin( - particle -> getSpin());
            double energyDelta = relatedEnergy(*particle) - oldEnergy;
            if (distribution(engine) > std::exp( - energyDelta / thermalEnergy))
                particle -> setSpin( - particle -> getSpin());
        }
    }



     void mcDynamicStep (std::vector<Particle>& particles,
                 std::initializer_list<int> dim_list,
                 std::initializer_list<Specie> targetSp,
                 std::function<double(const Particle&)> contribution,
                 std::mt19937_64& engine,
                 double thermalEnergy)
     {
        std::vector<int> dimensions(dim_list);


        std::function<double(const Particle&)> relatedEnergy = 
            [&contribution](const Particle& particle) {
                double energy = contribution(particle);
                for (auto&& other : particle.getNbh())
                    energy += contribution(*other);
                return energy;
            };

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
            double oldEnergy = relatedEnergy(*particle);
            darray oldPosition = particle -> getPosition();
            particle -> setPosition(oldPosition + randVec3D());
            double energyDelta = relatedEnergy(*particle) - oldEnergy;
            if (distribution(engine) > std::exp( - energyDelta / thermalEnergy))
                particle -> setPosition(oldPosition);
            else
                particle -> setPosition(fmod(particle -> getPosition(), dim_list));
        }
     }
}
