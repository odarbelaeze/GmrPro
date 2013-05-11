#include "Particle.h"

namespace Gmr
{
    Particle::Particle()
     : position_(0.0, 3), spin_(Gmr::Spin::Up), specie_(Gmr::Specie::Ion), nbh_() 
    {}

    Particle::Particle (darray position, Spin spin, Specie specie)
     : position_(position), spin_(spin), specie_(specie), nbh_()
    {
        if (specie == Specie::Ion) charge_ = 1.0;
        if (specie == Specie::Electron) charge_ = - 1.0;
    }

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

    double Particle::getCharge() const
    {
        return charge_;
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

    void Particle::setCharge(double charge)
    {
        charge_ = charge;
    }

    void Particle::setSpecie (const Specie& specie)
    {
        specie_ = specie;
    }

    void Particle::setNbh (const std::vector<Particle*>& nbh)
    {
        nbh_ = nbh;
    }

    int operator* (const Spin& a, const Spin& b)
    {
        return a == b? 1 : -1;
    }

    Spin operator- (const Spin& spin)
    {
        return spin == Spin::Up ? Spin::Down : Spin::Up;
    }

}
