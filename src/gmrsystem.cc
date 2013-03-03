#include "gmrsystem.h"

GmrSystem::GmrSystem()
 : System(), electricField_(), displacementAcumulator_(),
   magnetizationAcumulator_(0.0), magnetizationCounter_(0)
{}



GmrSystem::GmrSystem(const Json::Value& root)
 : System(root), electricField_(), displacementAcumulator_(),
   magnetizationAcumulator_(0.0), magnetizationCounter_(0)
{
    interactionTraits_.J = interactionInformation_["J"].asDouble();
    interactionTraits_.I_0 = interactionInformation_["I_0"].asDouble();
    interactionTraits_.K_0 = interactionInformation_["K_0"].asDouble();
}



GmrSystem::GmrSystem(const std::string fileName)
 : System(fileName), electricField_(), displacementAcumulator_(),
   magnetizationAcumulator_(0.0), magnetizationCounter_(0)
{
    interactionTraits_.J = interactionInformation_["J"].asDouble();
    interactionTraits_.I_0 = interactionInformation_["I_0"].asDouble();
    interactionTraits_.K_0 = interactionInformation_["K_0"].asDouble();
}



GmrSystem::GmrSystem(const char* fileName)
 : System(fileName), electricField_(), displacementAcumulator_(),
   magnetizationAcumulator_(0.0), magnetizationCounter_(0)
{
    interactionTraits_.J = interactionInformation_["J"].asDouble();
    interactionTraits_.I_0 = interactionInformation_["I_0"].asDouble();
    interactionTraits_.K_0 = interactionInformation_["K_0"].asDouble();
}



GmrSystem::GmrSystem(std::istream& istream)
 : System(istream), electricField_(), displacementAcumulator_(),
   magnetizationAcumulator_(0.0), magnetizationCounter_(0)
{
    interactionTraits_.J = interactionInformation_["J"].asDouble();
    interactionTraits_.I_0 = interactionInformation_["I_0"].asDouble();
    interactionTraits_.K_0 = interactionInformation_["K_0"].asDouble();
}



GmrSystem::~GmrSystem()
{}



void    GmrSystem::setElectricField(const Vector& electricField)
{
    electricField_ = electricField;
}



Vector& GmrSystem::getElectricFiedlRef()
{
    return electricField_;
}



Vector  GmrSystem::getMagnetization()
{
    return magnetization_;
}



Vector  GmrSystem::recalculateMagnetization()
{
    magnetization_ = Vector(0.0);
    for (int i = 0; i < particles_.size(); ++i)
    {
        magnetization_ = magnetization_ + particles_[i].getSpin();
    }
}



double   GmrSystem::computeFieldContribution_(int i)
{
    return - particles_[i].getCharge() * dot(particles_[i].getPosition(), electricField_);
}



double   GmrSystem::computeInteractionContribution_(int i)
{
    double sum = System::computeInteractionContribution_(i);
    std::vector<int> neighbors = particles_[i].getNeighbors();

    double dis;
    double dott;
    double j;

    for (int iii = 0; iii < neighbors.size(); iii++)
    {
        j    = neighbors[iii];
        dis  = distancePbc(particles_[i].getPosition(), particles_[j].getPosition(), 
                          dimensions_, boundaryConditions_);
        dott = dot(particles_[j].getSpin(), particles_[i].getSpin());

        if (particles_[j].getType() == "ion" && particles_[i].getType() == "ion")
        {
            sum = - interactionTraits_.J * dott;
        }
        else if (particles_[j].getType() == "electron" && particles_[i].getType() == "electron")
        {
            sum = - interactionTraits_.K_0 * exp( - dis) * dott;
        }
        else if ((particles_[j].getType() == "electron" && particles_[i].getType() == "ion") ||
                 (particles_[j].getType() == "ion" && particles_[i].getType() == "electron"))
        {
            sum = - interactionTraits_.I_0 * exp( - dis) * dott;
        }
        else
        {
            throw std::exception();
        }
    }

    return sum;
}



void    GmrSystem::onThermalEventCb_(Particle& particle, double energyDelta)
{
    Vector deltaM(particle.getSpin() - particle.getOldSpin());
    magnetization_ = magnetization_ + deltaM;
    magnetizationCounter_++;
    magnetizationAcumulator_ = magnetizationAcumulator_ + norm(magnetization_);
}



void    GmrSystem::onDynamicEventCb_(Particle& particle, double energyDelta)
{
    Vector deltaR(particle.getPosition() - particle.getOldPosition());
    double deltaPosition = systemInformation_["updatePolicy"]["deltaPosition"].asDouble(); 
    if (abs(deltaR[0]) < deltaPosition && abs(deltaR[1]) < deltaPosition && abs(deltaR[2]) < deltaPosition)
        displacementAcumulator_ = displacementAcumulator_ + deltaR;
}



Vector  GmrSystem::collectDisplacementData()
{
    Vector data = displacementAcumulator_;
    displacementAcumulator_ = Vector(0.0);
    return data;
}



double   GmrSystem::collectMagnetizationData()
{
    double data = magnetizationAcumulator_ / magnetizationCounter_;
    magnetizationAcumulator_ = 0.0;
    magnetizationCounter_ = 0;
    return data;
}
