#include "gmr.h"

namespace gmr
{
    Particle::Particle()
     : position_(0.0, 3), spin_(gmr::Spin::Up), specie_(gmr::Specie::Ion), nbh_() 
    {}

    Particle::Particle (darray position, Spin spin, Specie specie)
     : position_(position), spin_(spin), specie_(specie), nbh_()
    {}

    Particle::~Particle()
    {}

    darray Particle::getPosition () const
    {
        return position_;
    }

    Spin Particle::getSpin () const
    {
        return spin_;
    }

    Specie Particle::getSpecie () const
    {
        return specie_;
    }

    std::vector<Particle*> Particle::getNbh () const
    {
        return nbh_;
    }


    void Particle::setPosition (const darray& position)
    {
        position_ = position;
    }

    void Particle::setSpin (const Spin& spin)
    {
        spin_ = spin;
    }

    void Particle::setSpecie (const Specie& specie)
    {
        specie_ = specie;
    }

    void Particle::setNbh (const std::vector<Particle*>& nbh)
    {
        nbh_ = nbh;
    }

    int operator * (const Spin& a, const Spin& b) 
    {
        return static_cast<int> ( a == b );
    }

    Spin operator - (const Spin& spin)
    {
        if (spin == Spin::Up) return Spin::Down;
        else return Spin::Up;
    }

    void insertParticles (std::vector<Particle>& particles, Specie sp, Lattice lt, int w, int h, int l)
    {
        insertParticles(particles, sp, lt, {w, h, l});
    }

    void insertParticles (std::vector<Particle>& particles, Specie sp, Lattice lt, std::initializer_list<int> dims)
    {
        if (dims.size() != 3) throw new std::exception();
        std::vector<darray> basis;

        switch(lt)
        {
            case Lattice::sc:
                basis = { darray({ 0.0, 0.0, 0.0 }) };
                break;

            case Lattice::fcc:
                basis = {
                    darray({ 0.0, 0.0, 0.0 }),
                    darray({ 0.0, 0.5, 0.5 }),
                    darray({ 0.5, 0.0, 0.5 }),
                    darray({ 0.5, 0.5, 0.0 })
                };
                break;

            case Lattice::bcc:
                basis = {
                    darray({ 0.0, 0.0, 0.0 }),
                    darray({ 0.5, 0.5, 0.5 })
                };
                break;

            default:
                throw new std::exception();
        }

        auto dimensions = std::vector<int>(dims);

        std::mt19937_64 engine;
        std::uniform_int_distribution<int> uniform_dist(0, 1);

        for (int i = 0; i < dimensions.at(0); ++i)
        {
            for (int j = 0; j < dimensions.at(1); ++j)
            {
                for (int k = 0; k < dimensions.at(2); ++k)
                {
                    for (auto b : basis)
                        particles.push_back(Particle(
                            darray({(double)i, (double)j, (double)k}) + b,
                            uniform_dist(engine) == 0? Spin::Up : Spin::Down,
                            sp
                        ));
                }
            }
        }

    }

    void insertParticles (std::vector<Particle>& particles, Specie sp, int n, int w, int h, int l)
    {
        insertParticles(particles, sp, n, {w, h, l});
    }

    void insertParticles (std::vector<Particle>& particles, Specie sp, int n, std::initializer_list<int> dims)
    {
        if (dims.size() != 3) throw std::exception();

        auto dimensions = std::vector<int>(dims);

        std::mt19937_64 engine;
        std::uniform_int_distribution<int> spin_selector(0, 1);
        std::uniform_real_distribution<> x_selector(0.0, (double)dimensions.at(0));
        std::uniform_real_distribution<> y_selector(0.0, (double)dimensions.at(1));
        std::uniform_real_distribution<> z_selector(0.0, (double)dimensions.at(2));

        for (int i = 0; i < n; ++i)
        {
            particles.push_back(Particle(
                darray({ x_selector(engine), y_selector(engine), z_selector(engine) }),
                spin_selector(engine) == 0 ? Spin::Up : Spin::Down,
                sp
            ));
        }
    }

    void updateNeighbors (std::vector<Particle>& particles, double cutoff)
    {
        for (auto&& particle : particles)
        {
            auto nbh = std::vector<Particle*>();
            for (auto&& other : particles)
                if (&particle != &other && norm(particle.getPosition() - other.getPosition()) < cutoff)
                    nbh.push_back(&other);
            particle.setNbh(nbh);
        }
    }

    void mcStep (std::vector<Particle>& particles, 
                 std::function<double(const Particle&)> contribution,
                 std::function<double()> rng,
                 double kbT)
    {
        auto relatedEnergy = [&contribution](const Particle& particle) {
            double rEnergy = contribution(particle);
            for (auto&& nb : particle.getNbh())
                rEnergy += contribution(*nb);
            return rEnergy;
        };

        for (auto&& particle : particles)
        {
            double oldEnergy = relatedEnergy(particle);
            particle.setSpin( - particle.getSpin());
            double deltaE = relatedEnergy(particle) - oldEnergy;
            double r = rng();
            if (r > std::exp(- deltaE / kbT))
            {
                particle.setSpin( - particle.getSpin());
            }
        }
    }

    void mcDynamicStep (std::vector<Particle>& particles, 
                        Specie specie,
                        std::function<double(const Particle&)> contribution,
                        std::function<double()> rng,
                        double kbT)
    {
        auto relatedEnergy = [&contribution](const Particle& particle) {
            double rEnergy = contribution(particle);
            for (auto&& nb : particle.getNbh())
                rEnergy += contribution(*nb);
            return rEnergy;
        };

        std::vector<Particle*> targets;
        for (auto&& particle : particles)
        {
            if (particle.getSpecie() == specie)
            {
                targets.push_back(&particle);
            }
        }
        if(targets.size() == 0) return;
        
        for (auto&& particle : particles)
        {
            double oldEnergy = relatedEnergy(particle);
            darray oldPos    = particle.getPosition();
            darray delta({rng() - 0.5, rng() - 0.5, rng() - 0.5});
            particle.setPosition(particle.getPosition() + delta);
            double deltaE = relatedEnergy(particle) - oldEnergy;
            double r = rng();
            if (r > std::exp(- deltaE / kbT))
            {
                particle.setPosition(oldPos);
            }
        }
    }

    double energy (std::vector<Particle>& particles, 
                   std::function<double(const Particle&)> contribution)
    {
        double e = 0.0;
        for (auto&& particle : particles)
            e += contribution(particle);
        return e;
    }

    double magnetization (const std::vector<Particle>& particles)
    {
        // N = #Up + #Down
        // M = |#Up - #Down| / N
        // #Down = N - #Up
        // M = |2#Up - N| / N
        int numberUp = std::count_if(
            begin(particles), end(particles), [](const Particle& p){
                return p.getSpin() == Spin::Up;
            });
        int N = particles.size();
        return (double) std::abs(2.0 * numberUp - N) / N;
    }

    double norm(const darray& vec)
    {
        return std::sqrt(std::pow(vec, 2.0).sum());
    }

    double stddev(double mean, double meansq)
    {
        return std::sqrt(meansq - std::pow(mean, 2));
    }
}

int main(int argc, char const *argv[])
{
    using namespace gmr;

    auto particles = std::vector<Particle>();

    insertParticles(particles, Specie::Ion, Lattice::bcc, {5, 5, 5});
    insertParticles(particles, Specie::Electron, 125, {5, 5, 5});
    updateNeighbors(particles, 1.01);

    // Set up random devices
    std::random_device rd;
    std::mt19937_64 engine;
    std::uniform_real_distribution<> uniform(0.0,1.0);

    // Set up output
    std::cout << std::setprecision(8) << std::scientific;

    // Pick up a Hamiltonian

    auto Hamiltonian = [](const Particle& particle){
        double energy = 0.0;
        for (auto&& nb : particle.getNbh())
            energy -= particle.getSpin() * (*nb).getSpin();
        return energy;
    };

    // Perform a Monte Carlo thermalization

    for (double kbT = 40.0; kbT >= 0.0; kbT -= 1.0)
    {
        double e;
        double m;
        double eacum = 0.0;
        double eacumsq = 0.0;
        double macum = 0.0;
        double macumsq = 0.0;
        int    mcs  = 1000;
        for (int i = 0; i < mcs; ++i)
        {
            mcStep(particles, Hamiltonian, [&rd, &uniform](){
                return uniform(rd);
            }, kbT);
            e = energy(particles, Hamiltonian);
            m = magnetization(particles);
            eacum += e;
            macum += m;
            eacumsq += std::pow(e, 2);
            macumsq += std::pow(m, 2);
        }
        std::cout << std::setw(20) << kbT
                  << std::setw(20) << eacum / mcs
                  << std::setw(20) << stddev(eacum / mcs, eacumsq / mcs)
                  << std::setw(20) << macum / mcs
                  << std::setw(20) << stddev(macum / mcs, macumsq / mcs)
                  << std::endl;
    }

    return 0;
}