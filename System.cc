#include "System.h"

namespace Gmr
{
    // el sistema se debe inicializar con el tamano del mismo, sea con un
    // initializer_list o con un vector
    System::System (std::initializer_list<int> dim_list)
    {
        if (std::any_of(std::begin(dim_list), std::end(dim_list), 
            [](int i) { return i <= 0; }))
            throw std::exception();

        dimensions_ = std::vector<int>(dim_list);
        setDefaultParameters();
    }

    System::System (std::vector<int> dimensions)
    {
        if (std::any_of(std::begin(dimensions), std::end(dimensions), 
            [](int i) { return i <= 0; }))
            throw std::exception();

        dimensions_ = dimensions;
        setDefaultParameters();
    }

    System::~System () {}

    void System::setDefaultParameters ()
    {
        uniform_ = std::uniform_real_distribution<> (0.0, 1.0);
        electricField_ = darray({ 1.0, 0.0, 0.0 });
        magneticField_ = darray({ 0.0, 0.0, 0.0 });
        parameters_ = std::map<std::string, double>{
            { "Jex", 1.0 },
            { "I_0", 1.0 },
            { "K_0", 2.0 },
            { "R_0", 0.001 } };
    }

    double System::contribution_ (const Particle& particle) 
    {
        double contribution = particle.getSpin() * magneticField_[2];

        double d;
        double dot;
        double K;
        double I;

        #ifndef SPECIE_
        #define SPECIE_(p, sp) (p).getSpecie() == Specie::sp
        #endif

        if (SPECIE_(particle, Ion))
        {
            for (auto&& other : particle.getNnbh())
            {
                dot = particle.getSpin() * other -> getSpin();
                contribution -= parameters_["Jex"] * dot;
            }
        }

        for (auto&& other : particle.getNbh())
        {
            d   = distance(particle.getPosition(), other -> getPosition(), dimensions_);
            dot = particle.getSpin() * other -> getSpin();
            K   = parameters_["K_0"] * std::exp( - d);
            I   = parameters_["I_0"] * std::exp( - d);

            contribution += std::exp( - d / parameters_["R_0"]);

            if ((SPECIE_(particle, Ion) && SPECIE_(*other, Electron))
                     || (SPECIE_(particle, Electron) && SPECIE_(*other, Ion)))
                contribution += I * (1 - delta(particle.getSpin(), other -> getSpin()));

            else if (SPECIE_(particle, Electron) && SPECIE_(*other, Electron))
                contribution -= 0.0; //K * dot;
        }

        contribution -= particle.getCharge() * 
                        dot_product(electricField_, particle.getPosition());

        #undef SPECIE_

        return contribution;
    }

    double System::relatedEnergy_ (const Particle& particle) 
    {
        double energy = contribution_(particle);

        for (auto&& other : particle.getNbh())
            energy += contribution_(*other);
        
        return energy;
    }

    double System::energy ()
    {
        double energy = 0;

        for (auto&& particle : particles_)
            energy += contribution_(particle);

        return energy;
    }

    double System::magnetization (){
        float magnetization = 0;
        
        for (auto&& particle : particles_)
            magnetization += particle.getSpin() == Spin::Up ? 1.0: - 1.0;
        
        return std::abs(magnetization / particles_.size());
    }

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

        for (int i = 0; i < dimensions_[0]; ++i)
        {
            for (int j = 0; j < dimensions_[1]; ++j)
            {
                for (int k = 0; k < dimensions_[2]; ++k)
                {
                    for (auto position : basis)
                        particles_.push_back(Particle(
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

        std::uniform_real_distribution<> x(0.0, (double) dimensions_[0]);
        std::uniform_real_distribution<> y(0.0, (double) dimensions_[1]);
        std::uniform_real_distribution<> z(0.0, (double) dimensions_[2]);

        for (int i = 0; i < count; ++i)
        {
            particles_.push_back(Particle(
                darray({ x(engine_), y(engine_), z(engine_) }),
                spins.sample(),
                specie
            ));
        }
    }

    void System::updateNeighbors (double radius_nnb, double radius){
        for (auto&& p : particles_)
        {
            std::vector<Particle*> nnbh;
            std::vector<Particle*> nbh;

            for (auto&& other : particles_)
            {
                if (&other != &p)
                {
                    double d = distance(p.getPosition(), other.getPosition(), dimensions_);
                    
                    if (p.getSpecie() == Specie::Ion)
                    {
                        if (other.getSpecie() == Specie::Ion)
                        {
                            if (d <= radius_nnb)
                                nnbh.push_back(&other);
                        }
                        else
                        {
                            if (d <= radius)
                                nbh.push_back(&other);
                        }
                    }
                    else
                    {
                        if (d <= radius)
                            nbh.push_back(&other);
                    }

                }
            }
            
            p.setNbh(nbh);
            p.setNnbh(nnbh);
        }
    }

    void System::mcThermalStep (double thermalEnergy){       
        
        Deck<Particle*> targets;
        
        for (auto& particle : particles_)
            targets.push(&particle);

        while (!targets.isEmpty())
        {
            Particle* particle = targets.pop();

            double oldEnergy = relatedEnergy_(*particle);
            particle -> setSpin( - particle -> getSpin());
            double energyDelta = relatedEnergy_(*particle) - oldEnergy;
            
            if (uniform_(engine_) > std::exp( - energyDelta / thermalEnergy))
                particle -> setSpin( - particle -> getSpin());
        }
    }

    void System::mcThermalStep (std::initializer_list<Specie> targetSp,
                     double thermalEnergy)
    {
        Deck<Particle*> targets;

        for (auto&& particle : particles_)
            if (std::any_of(begin(targetSp), end(targetSp), 
                [&particle](Specie sp){ return particle.getSpecie() == sp; })) 

                targets.push(&particle);

        while (!targets.isEmpty())
        {
            Particle* particle = targets.pop();

            double oldEnergy = relatedEnergy_(*particle);
            particle -> setSpin( - particle -> getSpin());
            double energyDelta = relatedEnergy_(*particle) - oldEnergy;

            if (uniform_(engine_) > std::exp( - energyDelta / thermalEnergy))
                particle -> setSpin( - particle -> getSpin());
        }
    }

    void System::mcDynamicStep (std::initializer_list<Specie> targetSp,
                                double thermalEnergy)
    {
        Deck<Particle*> targets;

        for (auto&& particle : particles_)
        {
            if (std::any_of(begin(targetSp), end(targetSp), 
                [&particle](Specie sp){ return particle.getSpecie() == sp; })) 
                targets.push(&particle);
        }

        while (!targets.isEmpty())
        {
            Particle* particle = targets.pop();

            double oldEnergy = relatedEnergy_(*particle);
            darray oldPosition = particle -> getPosition();
            particle -> setPosition(oldPosition + rand3d(engine_, uniform_));
            double energyDelta = relatedEnergy_(*particle) - oldEnergy;
            
            if (uniform_(engine_) > std::exp( - energyDelta / thermalEnergy))
                particle -> setPosition(oldPosition);
            else
                particle -> setPosition(
                    fmod(particle -> getPosition(), dimensions_));
        }
    }

    void System::mcDynamicStep (std::initializer_list<Specie> targetSp,
                                double thermalEnergy,
                                DynamicStats& stats)
    {
        Deck<Particle*> targets;

        for (auto&& particle : particles_)
        {
            if (std::any_of(begin(targetSp), end(targetSp), 
                [&particle](Specie sp){ return particle.getSpecie() == sp; })) 
                targets.push(&particle);
        }

        while (!targets.isEmpty())
        {
            Particle* particle = targets.pop();

            double oldEnergy = relatedEnergy_(*particle);
            darray oldPosition = particle -> getPosition();
            particle -> setPosition(oldPosition + rand3d(engine_, uniform_));
            double energyDelta = relatedEnergy_(*particle) - oldEnergy;
            
            if (uniform_(engine_) > std::exp( - energyDelta / thermalEnergy))
            {
                particle -> setPosition(oldPosition);
                stats.record(particle);
            }
            else
            {
                stats.record (particle, oldPosition, particle -> getPosition());

                // Pacman effect
                particle -> setPosition(
                    fmod(particle -> getPosition(), dimensions_));
            }
        }
    }

    std::vector<int> System::getDimensions()
    {
        return dimensions_;
    }

    std::mt19937_64& System::getEngineRef()
    {
        return engine_;
    }

    std::vector<Particle> System::getParticles()
    {
        return particles_;
    }

    void System::setDimensions(std::vector<int> dimensions)
    {
        dimensions_ = dimensions;
    }

    void System::setEngine(std::mt19937_64& engine)
    {
        engine_ = engine;
    }

    void System::setParticles(std::vector<Particle> particles)
    {
        particles_ = particles;
    }

    void System::setParameter(std::string parameter, double value)
    {
        parameters_[parameter] = value;
    }

}