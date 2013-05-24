#include <cmath>

#include "System.h"

namespace Gmr
{
    System::System(std::initializer_list<int> dim_list)
    {
        if (std::any_of(std::begin(dim_list), std::end(dim_list), [](int i) { return i <= 0; }))
            throw std::exception();

        this -> dimensions = std::vector<int>(dim_list);
        this -> setDefaultParameters();
    }

    System::System(std::vector<int> dimensions)
    {
        this -> dimensions = dimensions;
        this -> setDefaultParameters();
    }

    System::~System(){}

    void System::setDefaultParameters(){
        this -> electricField = darray({ 0.0, 0.0, 0.0 });
        this -> parameters = std::map<std::string, double>{
            { "Jex", 1.0 },
            { "I_0", 1.0 },
            { "K_0", 2.0 },
            { "R_0", 0.001 } };
    };

    double System::distance (const darray& a, const darray& b){
        return std::sqrt(std::pow(b - a, 2).sum());
    };

    double System::contribution (const Particle& particle){
        double contribution = 0;
        #ifndef _SPECIE
        #define _SPECIE(p, sp) (p).getSpecie() == Specie::sp
        #endif

        for (auto&& other : particle.getNbh())
        {
            float d = this -> distance(particle.getPosition(), other -> getPosition());
            float dot = particle.getSpin() * other -> getSpin();
            double K = this -> parameters["K_0"] * std::exp(- d);
            double I = this -> parameters["I_0"] * std::exp(- d);

            contribution += std::exp( - d / this -> parameters["R_0"]);

            if (_SPECIE(particle, Ion) && _SPECIE(*other, Ion))
                contribution -= this -> parameters["Jex"] * dot;

            else if ((_SPECIE(particle, Ion) && _SPECIE(*other, Electron))
                     || (_SPECIE(particle, Electron) && _SPECIE(*other, Ion)))
                contribution -= I * dot;

            else if (_SPECIE(particle, Electron) && _SPECIE(*other, Electron))
                contribution -= K * dot;
        }

        contribution -= particle.getCharge() * (this -> electricField * particle.getPosition()).sum();

        #undef _SPECIE
        return contribution;
    }

    double System::relatedEnergy(const Particle& particle){
        double energy = this -> contribution(particle);
        for (auto&& other : particle.getNbh())
            energy += this -> contribution(*other);
        return energy;
    }

    double System::energy (const std::vector<Particle> particles){
        double energy = 0;
        for (auto&& particle : particles)
            energy += contribution(particle);
        return energy;
    };

    double System::magnetization (const std::vector<Particle>& particles){
        float magnetization = 0;
        for (auto&& particle : particles)
        {
            magnetization += particle.getSpin() == Spin::Up ? 1.0: -1.0;
        }
        return std::abs(magnetization / particles.size());
    };

    void System::insertParticles (Specie specie, Lattice lattice){

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

        for (int i = 0; i < this -> dimensions[0]; ++i)
        {
            for (int j = 0; j < this -> dimensions[1]; ++j)
            {
                for (int k = 0; k < this -> dimensions[2]; ++k)
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

    void System::insertParticles (Specie specie, int count){
        Deck<Spin> spins;
        spins.push(Spin::Up);
        spins.push(Spin::Down);

        std::uniform_real_distribution<> x(0.0, (double) dimensions[0]);
        std::uniform_real_distribution<> y(0.0, (double) dimensions[1]);
        std::uniform_real_distribution<> z(0.0, (double) dimensions[2]);

        for (int i = 0; i < count; ++i)
        {
            particles.push_back(Particle(
                darray({ x(this -> engine), y(this -> engine), z(this -> engine) }),
                spins.sample(),
                specie
            ));
        }
    }

    void System::updateNeighbors (double radius){
        for (auto&& particle : this -> particles)
        {
            std::vector<Particle*> vecinitos;
            for (auto&& other : this -> particles)
            {
                if (&other != &particle 
                    && sqrt(pow(particle.getPosition() - other.getPosition(), 2).sum()) <= radius)
                    vecinitos.push_back(&other);
            }
            particle.setNbh(vecinitos);
        }
    }

    void System::mcThermalStep (double thermalEnergy){       
        Deck<Particle*> targets;
        for (auto& particle : this -> particles)
            targets.push(&particle);

        std::uniform_real_distribution<> distribution(0.0, 1.0);

        while (!targets.isEmpty())
        {
            Particle* particle = targets.pop();
            double oldEnergy = this -> relatedEnergy(*particle);
            particle -> setSpin( - particle -> getSpin());
            double energyDelta = this -> relatedEnergy(*particle) - oldEnergy;
            if (distribution(this -> engine) > std::exp( - energyDelta / thermalEnergy))
                particle -> setSpin( - particle -> getSpin());
        }
    }

    void System::mcThermalStep (std::initializer_list<Specie> targetSp,
                     double thermalEnergy)
    {
        Deck<Particle*> targets;

        for (auto&& particle : this -> particles)
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
            if (distribution(this -> engine) > std::exp( - energyDelta / thermalEnergy))
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

        auto randVec3D = [&theta, &phi, &this/*, &engine*/](){
            double t = theta(this -> engine)
                 , p = phi(this -> engine);
            return darray({
                std::sin(t) * std::cos(p),
                std::sin(t) * std::sin(p),
                std::cos(t)
            });
        };

        Deck<Particle*> targets;
        // use std::any_of(begin, end, function -> bool)
        for (auto&& particle : this -> particles)
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
            if (distribution(this -> engine) > std::exp( - energyDelta / thermalEnergy))
                particle -> setPosition(oldPosition);
            else
                particle -> setPosition(fmod(particle -> getPosition(), std::initializer_list<int>({dimensions[0],dimensions[1],dimensions[2]})));
        }
    }

    std::vector<int> System::getDimensions()
    {
        return this -> dimensions;
    }

    std::mt19937_64& System::getEngineRef()
    {
        return this -> engine;
    }

    std::vector<Particle> System::getParticles()
    {
        return this -> particles;
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

    void System::setParameter(std::string parameter, double value)
    {
        this -> parameters[parameter] = value;
    }

}