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

    double norm(const darray& vec)
    {
        return std::sqrt(std::pow(vec, 2.0).sum());
    }

}

int main(int argc, char const *argv[])
{
    using namespace gmr;

    auto particles = std::vector<Particle>();

    insertParticles(particles, Specie::Ion, Lattice::bcc, {5, 5, 5});
    updateNeighbors(particles, 1.01);

    return 0;
}