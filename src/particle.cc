#include "particle.h"

Particle::Particle()
{
} 

Particle::Particle(const std::string& type)
{
    if (type == "Electrón")
    {
        type_ = "Electrón";
        charge_ = -1;
        isMovable_ = true;
        spin_ = randomVector();
        oldSpin_ = spin_;
    }
    else if (type == "Ión")
    {
        type_ = "Ión";
        charge_ = +1;
        isMovable_ = false;
        spin_ = randomVector();
        oldSpin_ = spin_;
    }
    else
    {
        type_ = type_;
        spin_ = randomVector();
        oldSpin_ = spin_;
    }
}



Particle::~Particle(){}


void Particle::updateSpin(float radius)
{
    float spinNorm = norm(spin_);
    oldSpin_ = spin_; 
    spin_ = spin_ + radius * randomVector();
    spin_ = spinNorm * spin_ / norm(spin_);
    // oldSpin_ = spin_; 
    // spin_ = radius * randomVector();

}



void Particle::updatePosition(float radius)
{
    oldPosition_ = position_; 
    position_ = position_ + radius * randomVector();
}



void Particle::rollBackPosition()
{
    position_ = oldPosition_;
}



void Particle::rollBackSpin()
{
    spin_ = oldSpin_;
}



void Particle::setPosition(const Vector& position)
{
    position_ = position;
}



void Particle::setSpin(const Vector& spin)
{
    spin_ = spin;
}



void Particle::setOldPosition(const Vector& oldPosition)
{
    oldPosition_ = oldPosition;
}



void Particle::setOldSpin(const Vector& oldSpin)
{
    oldSpin_ = oldSpin;
}



void Particle::setNeighbors(const std::vector<int>& neighbors)
{
    neighbors_ = neighbors;
}

void Particle::addNeighbor(int id)
{
    neighbors_.push_back(id);
}

void Particle::clearNeighbors()
{
    neighbors_.clear();
}


void Particle::setId(const int id)
{
    id_ = id;
}



void Particle::setType(const std::string& type)
{
    type_ = type;
}



void Particle::setCharge(const float charge)
{
    charge_ = charge;
}



void Particle::setMovable(bool isMovable)
{
    isMovable_ = isMovable;
}



void Particle::pacmanEffect(const Vector& vector)
{
    position_.pacmanEffect(vector);
}



Vector Particle::getPosition()
{
    return position_;
}



Vector Particle::getSpin()
{
    return spin_;
}



Vector Particle::getOldPosition()
{
    return oldPosition_;
}



Vector Particle::getOldSpin()
{
    return oldSpin_;
}



std::vector<int> Particle::getNeighbors()
{
    return neighbors_;
}



int Particle::getId()
{  
    return id_;
}



std::string Particle::getType()
{
    return type_;
}



float Particle::getCharge()
{
    return charge_;
}



bool Particle::getMovable()
{
    return isMovable_;
}
