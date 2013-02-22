#include <cassert>

#include "system.h"
BadDescriptorException::BadDescriptorException() : error_("Unknown")
{

}



BadDescriptorException::BadDescriptorException(const std::string error)
 : error_(error)
{

}



const char* BadDescriptorException::what() const throw()
{
    return ("The descriptor is invalid: " + error_).data();
}



System::System() : time_(0.0)
{

}


System::System(const Json::Value& root) : time_(0.0)
{
    initSystem_(root);
}



System::System(const std::string fileName) : time_(0.0)
{
    std::ifstream descriptorFile(fileName.data());
    Json::Reader reader;
    Json::Value  root;

    bool parsingSuccessful = reader.parse(descriptorFile, root, false);
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse configuration\n"
                   << reader.getFormattedErrorMessages();
        throw BadDescriptorException("The descriptor is not a valid json file.");
    }
    descriptorFile.close();

    initSystem_(root);
}



System::System(const char* fileName) : time_(0.0)
{
    std::ifstream descriptorFile(fileName);
    Json::Reader reader;
    Json::Value  root;
    
    bool parsingSuccessful = reader.parse(descriptorFile, root, false);
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse configuration\n"
                   << reader.getFormattedErrorMessages();
        throw BadDescriptorException("The descriptor is not a valid json file.");
    }
    descriptorFile.close();

    initSystem_(root);
}



System::System(std::istream& descriptorIstream) : time_(0.0)
{
    Json::Reader reader;
    Json::Value  root;

    bool parsingSuccessful = reader.parse(descriptorIstream, root, false);
    if (!parsingSuccessful)
    {
        std::cout  << "Failed to parse configuration\n"
                   << reader.getFormattedErrorMessages();
        throw BadDescriptorException("The descriptor is not a valid json file.");
    }

    initSystem_(root);
}



System::~System()
{

}



void System::initSystem_(const Json::Value& root)
{
    systemInformation_      = root["system"];
    interactionInformation_ = root["interactionInformation"];
    thermalEnergy_          = 0.0001;
    time_                   = 0;
    neighborCutOff_         = systemInformation_["neighborCutOff"].asDouble();
    muffinTinRadi_          = systemInformation_["muffinTinRadi"].asDouble();
    structure_              = systemInformation_["structure"].asString();

    try
    {
        dimensions_[0] = systemInformation_["dimensions"]["width"].asInt();
        dimensions_[1] = systemInformation_["dimensions"]["lenght"].asInt();
        dimensions_[2] = systemInformation_["dimensions"]["height"].asInt();    
    }
    catch (std::exception &e)
    {
        throw BadDescriptorException(e.what());
    }

    try
    {
        boundaryConditions_[0] = systemInformation_["boundaryConditions"]["x"].asInt();
        boundaryConditions_[1] = systemInformation_["boundaryConditions"]["y"].asInt();
        boundaryConditions_[2] = systemInformation_["boundaryConditions"]["z"].asInt();
    }
    catch (std::exception &e)
    {
        throw BadDescriptorException(e.what());
    }

    try
    {
        scale_ = systemInformation_["scale"].asInt();
    }
    catch (std::exception &e)
    {
        throw BadDescriptorException(e.what());
    }

    if (dimensions_[0] <= 0 || dimensions_[1] <= 0 || dimensions_[2] <= 0 || scale_ <= 0)
    {
        throw BadDescriptorException("Come on man! are you serius about your dimensions and scale?");
    }

    Vector vector;
    Particle particle("ion");

    if (structure_ == "sc")
    {
        for (int i = 0; i < dimensions_[0]; ++i)
        {
            for (int j = 0; j < dimensions_[1]; ++j)
            {
                for (int k = 0; k < dimensions_[2]; ++k)
                {
                    vector[0] = (i + 0.0f) * scale_;
                    vector[1] = (j + 0.0f) * scale_;
                    vector[2] = (k + 0.0f) * scale_;
                    particle.setPosition(vector);
                    particle.setOldPosition(vector);
                    particles_.push_back(particle);

                }
            }
        }
    }
    else if (structure_ == "bcc")
    {
        for (int i = 0; i < dimensions_[0]; ++i)
        {
            for (int j = 0; j < dimensions_[1]; ++j)
            {
                for (int k = 0; k < dimensions_[2]; ++k)
                {
                    vector[0] = (i + 0.0f) * scale_;
                    vector[1] = (j + 0.0f) * scale_;
                    vector[2] = (k + 0.0f) * scale_;
                    particle.setPosition(vector);
                    particle.setOldPosition(vector);
                    particles_.push_back(particle);

                    vector[0] = (i + 0.5f) * scale_;
                    vector[1] = (j + 0.5f) * scale_;
                    vector[2] = (k + 0.5f) * scale_;
                    particle.setPosition(vector);
                    particle.setOldPosition(vector);
                    particles_.push_back(particle);
                    
                }
            }
        }
    }
    else if (structure_ == "fcc")
    {
        for (int i = 0; i < dimensions_[0]; ++i)
        {
            for (int j = 0; j < dimensions_[1]; ++j)
            {
                for (int k = 0; k < dimensions_[2]; ++k)
                {
                    vector[0] = (i + 0.0f) * scale_;
                    vector[1] = (j + 0.0f) * scale_;
                    vector[2] = (k + 0.0f) * scale_;
                    particle.setPosition(vector);
                    particle.setOldPosition(vector);
                    particles_.push_back(particle);


                    vector[0] = (i + 0.5f) * scale_;
                    vector[1] = (j + 0.5f) * scale_;
                    vector[2] = (k + 0.0f) * scale_;
                    particle.setPosition(vector);
                    particle.setOldPosition(vector);
                    particles_.push_back(particle);
                    

                    vector[0] = (i + 0.0f) * scale_;
                    vector[1] = (j + 0.5f) * scale_;
                    vector[2] = (k + 0.5f) * scale_;
                    particle.setPosition(vector);
                    particle.setOldPosition(vector);
                    particles_.push_back(particle);
                    

                    vector[0] = (i + 0.5f) * scale_;
                    vector[1] = (j + 0.0f) * scale_;
                    vector[2] = (k + 0.5f) * scale_;
                    particle.setPosition(vector);
                    particle.setOldPosition(vector);
                    particles_.push_back(particle);
                }
            }
        }
    }
    else
    {
        throw BadDescriptorException("The structure " + structure_ + " is not supported.");
    }

    dimensions_ = scale_ * dimensions_;
    particle = Particle("electron");
    int electronCount = systemInformation_["freeElectronCount"].asInt();

    for (int i = 0; i < electronCount; ++i)
    {
        particle.setPosition(dimensions_ * randomVectorInBox());
        particle.setOldPosition(particle.getPosition());
        particles_.push_back(particle);
    }

    for (int i = 0; i < particles_.size(); ++i){
        particles_[i].setId(i);
        particles_[i].setSpin(randomVector());
        particles_[i].setOldSpin(particles_[i].getSpin());
    }

    findNeighbors_();
    checkCloseNeighbors_();
}



void System::findNeighbors_()
{
    for (int i = 0; i < particles_.size(); ++i)
    {
        particles_[i].clearNeighbors();
        for (int j = 0; j < particles_.size(); ++j)
        {
            if ((particles_[i].getId() != particles_[j].getId()) &&
                (distancePbc(particles_[i].getPosition(), 
                             particles_[j].getPosition(), 
                             dimensions_, boundaryConditions_) <= neighborCutOff_))
                particles_[i].addNeighbor(j);
        }
    }
}



void System::checkCloseNeighbors_()
{
    bool  hadCloseNeighbors = false;
    std::vector<int> neighbors;

    for (int i = 0; i < particles_.size(); ++i)
    {
        neighbors = particles_[i].getNeighbors();
        for (int j = 0; j < neighbors.size(); ++j)
        {
            if (distancePbc(particles_[i].getPosition(), 
                            particles_[neighbors[j]].getPosition(), 
                            dimensions_, boundaryConditions_) < muffinTinRadi_)
            {
                hadCloseNeighbors = true;
                particles_[i].setPosition(dimensions_ * randomVectorInBox());
            }
        }
    }

    if (hadCloseNeighbors == true)
    {
        findNeighbors_();
        checkCloseNeighbors_();
    }
}



const std::vector<Particle>& System::getParticles()
{
    return particles_;
}



const Json::Value& System::getSystemInformation()
{
    return systemInformation_;
}



const Json::Value& System::getInteractionInformation()
{
    return interactionInformation_;
}



Vector System::getDimensions()
{
    return dimensions_;
}



double System::getThermalEnergy()
{
    return thermalEnergy_;
}



double System::getEnergy()
{
    return energy_;
}



unsigned long int System::getTime()
{
    return time_;
}



double System::computeEnergy_()
{
    double energy = 0.0;
    for (int id = 0; id < particles_.size(); id++)
        energy += computeEnergyContribution_(id);
    return energy;
}



void System::setThermalEnergy(const double& thermalEnergy)
{
    thermalEnergy_ = thermalEnergy;

}



void System::monteCarloThermalStep(bool needNeighborUpdate, bool callback)
{
    if (needNeighborUpdate == true)
        findNeighbors_();
    if (time_ == 0)
        energy_ = computeEnergy_();

    int   i = 0;
    double oldEnergy;
    double energyDelta;
    double deltaSpin = systemInformation_["updatePolicy"]["deltaSpin"].asDouble();

    for (int iii = 0; iii < particles_.size(); iii++)
    {
        time_ += 1.0;
        i      = rand() % particles_.size();

        oldEnergy = computeRelatedEnergy_(i);
        particles_[i].updateSpin(deltaSpin);
        energyDelta = computeRelatedEnergy_(i) - oldEnergy;

        if (energyDelta <= 0.0f)
        {
            energy_ += energyDelta;
            if (callback == true)
                onThermalEventCb_(particles_[i], energyDelta);
        }
        else if (drand48() <= exp(- energyDelta / thermalEnergy_))
        {
            energy_ += energyDelta;
            if (callback == true)
                onThermalEventCb_(particles_[i], energyDelta);
        }
        else
        {
            particles_[i].rollBackSpin();
        }
    }
}



void System::monteCarloDynamicStep(bool needNeighborUpdate, bool callback)
{
    if (needNeighborUpdate == true)
        findNeighbors_();
    if (time_ == 0)
        energy_ = computeEnergy_();

    int i = 0;
    double oldEnergy;
    double energyDelta;
    double deltaPosition = systemInformation_["updatePolicy"]["deltaPosition"].asDouble(); 

    for (int iii = 0; iii < particles_.size(); iii++)
    {
        time_ += 1.0;
        i      = rand() % particles_.size();
        
        if (particles_[i].getMovable() == true)
        {
            oldEnergy   = computeRelatedEnergy_(i);
            particles_[i].updatePosition(deltaPosition);
            energyDelta = computeRelatedEnergy_(i) - oldEnergy;
            particles_[i].pacmanEffect(dimensions_);
            
            if (energyDelta <= 0)
            {
                energy_ += energyDelta;
                if (callback == true)
                    onDynamicEventCb_(particles_[i], energyDelta);
            }
            else if (drand48() <= exp(-energyDelta / thermalEnergy_))
            {
                energy_ += energyDelta;
                if (callback == true)
                    onDynamicEventCb_(particles_[i], energyDelta);
            }
            else
            {
                particles_[i].rollBackPosition();
            }
        }
    }
}



void System::resetSystem()
{
    time_ = 0;
    findNeighbors_();
    checkCloseNeighbors_();
    energy_ = computeEnergy_();
}



double System::computeFieldContribution_(int id)
{
    return 0.0;
}



double System::computeInteractionContribution_(int id)
{
    double sum = 0;
    double dis;
    std::vector<int> neighbors = particles_[id].getNeighbors();

    for (int i = 0; i < neighbors.size(); ++i)
    {
        dis = distancePbc(particles_[i].getPosition(), particles_[id].getPosition(), 
                          dimensions_, boundaryConditions_);
        sum += exp(1 / (dis * muffinTinRadi_));
    }

    return sum;
}



void  System::onThermalEventCb_(Particle& particle, double energyDelta)
{

}



void  System::onDynamicEventCb_(Particle& particle, double energyDelta)
{

}



double System::computeEnergyContribution_(int id)
{
    return computeFieldContribution_(id) + 
           computeInteractionContribution_(id);
}



double System::computeRelatedEnergy_(int i)
{
    int j = 0;
    double sum = 0.0;
    std::vector<int> neighbors = particles_[i].getNeighbors();

    sum += computeEnergyContribution_(i);

    for (int ii = 0; ii < neighbors.size(); ++ii)
    {
        j    = neighbors[ii];
        sum += computeEnergyContribution_(j);
    }

    return sum;
}