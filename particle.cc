#include "particle.h"

Particle::Particle()
{

}



Particle::~Particle(){}


void Particle::updateSpin(float radius)
{
    float spinNorm = norm(spin_);
    oldSpin_ = spin_; 
    spin_ = spin_ + radius * randomVector();
    spin_ = spin_ / norm(spin_);
    spin_ = spin_ * spinNorm;
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

void Particle::addNeighbor(const int id)
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

