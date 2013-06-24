#include "Particle.h"

namespace Gmr
{
    // se inicializa una partícula con valores por defecto
    Particle::Particle()
     : position_(0.0, 3), spin_(Gmr::Spin::Up), specie_(Gmr::Specie::Ion), nbh_() 
    {}

    // se inicializa la particula con valores particulares
    Particle::Particle (darray position, Spin spin, Specie specie)
     : position_(position), spin_(spin), specie_(specie), nbh_()
    {
        // la carga depende del tipo de particula
        if (specie == Specie::Ion) charge_ = 1.0;
        if (specie == Specie::Electron) charge_ = - 1.0;
    }

    Particle::~Particle()
    {}

    // devuelve la posicion de la particula
    darray Particle::getPosition () const
    {
        return position_;
    }

    // devuelve el Spin de la particula (UP o Down)
    Spin Particle::getSpin () const
    {
        return spin_;
    }

    // retorna la carga de la particula
    double Particle::getCharge() const
    {
        return charge_;
    }

    // retorna la Specie de la particula (Ion o Electron)
    Specie Particle::getSpecie () const
    {
        return specie_;
    }

    // retorna un vector de direcciones de particulas
    std::vector<Particle*> Particle::getNbh () const
    {
        return nbh_;
    }

    // asignar un valor a postion_
    void Particle::setPosition (const darray& position)
    {
        position_ = position;
    }

    // se asigna un valor a spin_
    void Particle::setSpin (const Spin& spin)
    {
        spin_ = spin;
    }

    // se asigna un valor a charge_
    void Particle::setCharge(double charge)
    {
        charge_ = charge;
    }

    // se asigna un valor a specie_
    void Particle::setSpecie (const Specie& specie)
    {
        specie_ = specie;
    }

    // se asigna el vector de vecinos a la particula
    void Particle::setNbh (const std::vector<Particle*>& nbh)
    {
        nbh_ = nbh;
    }

    // se sobrecarga el operador * con el fin de que de 1 en el caso que los
    // spines tengan la misma orientacion, de lo contrario -1
    int operator* (const Spin& a, const Spin& b)
    {
        return a == b? 1 : -1;
    }

    // se sobrecarga el operador - con el fin de invertir el spin
    Spin operator- (const Spin& spin)
    {
        return spin == Spin::Up ? Spin::Down : Spin::Up;
    }

}
