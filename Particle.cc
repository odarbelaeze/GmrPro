#include "Particle.h"

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

    int operator* (const Spin& a, const Spin& b)
    {
    	return a == b? 1 : -1;
    }

    Spin operator- (const Spin& spin)
    {
    	return spin == Spin::Up ? Spin::Down : Spin::Up;
    }

}
