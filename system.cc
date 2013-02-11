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
    Vector vectorTemplate;
    systemInformation_ = root["system"];
    interactionInformation_ = root["interaction_info"];
    thermalEnergy_ = 0.0001;

    int width = 0;
    int lenght = 0;
    int height = 0;
    int scale = 0;

    try
    {
        width = systemInformation_["dimensions"]["width"].asInt();
        lenght = systemInformation_["dimensions"]["lenght"].asInt();
        height = systemInformation_["dimensions"]["height"].asInt();    
    }
    catch (std::exception &e)
    {
        throw BadDescriptorException(e.what());
    }
    
    try
    {
        scale = systemInformation_["scale"].asInt();
    }
    catch (std::exception &e)
    {
        throw BadDescriptorException(e.what());
    }

    if (width <= 0 || lenght <= 0 || height <= 0 || scale <= 0)
    {
        throw BadDescriptorException("Come on man! are you serius about your dimensions and scale?");
    }

    std::string structure = systemInformation_["structure"].asString();

    dymensions_ = Vector(width * scale, height * scale, lenght * scale);

    particles_.clear();

    Particle particleTemplate("Ión");
    if (structure == "sc")
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                for (int k = 0; k < lenght; ++k)
                {
                    vectorTemplate.setX((i + 0.0f) * scale);
                    vectorTemplate.setY((j + 0.0f) * scale);
                    vectorTemplate.setZ((k + 0.0f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setOldPosition(vectorTemplate);
                    particles_.push_back(particleTemplate);

                }
            }
        }
    }
    else if (structure == "bcc")
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                for (int k = 0; k < lenght; ++k)
                {
                    vectorTemplate.setX((i + 0.0f) * scale);
                    vectorTemplate.setY((j + 0.0f) * scale);
                    vectorTemplate.setZ((k + 0.0f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setOldPosition(vectorTemplate);
                    particles_.push_back(particleTemplate);

                    vectorTemplate.setX((i + 0.5f) * scale);
                    vectorTemplate.setY((j + 0.5f) * scale);
                    vectorTemplate.setZ((k + 0.5f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setOldPosition(vectorTemplate);
                    particles_.push_back(particleTemplate);
                    
                }
            }
        }
    }
    else if (structure == "fcc")
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                for (int k = 0; k < lenght; ++k)
                {
                    vectorTemplate.setX((i + 0.0f) * scale);
                    vectorTemplate.setY((j + 0.0f) * scale);
                    vectorTemplate.setZ((k + 0.0f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setOldPosition(vectorTemplate);
                    particles_.push_back(particleTemplate);


                    vectorTemplate.setX((i + 0.5f) * scale);
                    vectorTemplate.setY((j + 0.5f) * scale);
                    vectorTemplate.setZ((k + 0.0f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setOldPosition(vectorTemplate);
                    particles_.push_back(particleTemplate);
                    

                    vectorTemplate.setX((i + 0.0f) * scale);
                    vectorTemplate.setY((j + 0.5f) * scale);
                    vectorTemplate.setZ((k + 0.5f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setOldPosition(vectorTemplate);
                    particles_.push_back(particleTemplate);
                    

                    vectorTemplate.setX((i + 0.5f) * scale);
                    vectorTemplate.setY((j + 0.0f) * scale);
                    vectorTemplate.setZ((k + 0.5f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setOldPosition(vectorTemplate);
                    particles_.push_back(particleTemplate);
                }
            }
        }
    }
    else
    {
        throw BadDescriptorException("The structure " + structure + " is not supported.");
    }

    particleTemplate = Particle("Electrón");
    int electronCount = systemInformation_["number_of_free_electrons"].asInt();

    for (int i = 0; i < electronCount; ++i)
    {
        particleTemplate.setPosition(dymensions_ * randomVectorInBox());
        particleTemplate.setOldPosition(particleTemplate.getPosition());
        particles_.push_back(particleTemplate);
    }

    for (int i = 0; i < particles_.size(); ++i){
        particles_[i].setId(i);
        particles_[i].setSpin(randomVector());
        particles_[i].setOldSpin(particles_[i].getSpin());
    }

    findNeighbors_();
}



void System::findNeighbors_()
{
    float cutOff = systemInformation_["cut_off_radius"].asFloat();

    int scale = systemInformation_["scale"].asInt();
    
    Vector pbc;
    pbc.setX(systemInformation_["periodic_boundary_conditions"]["x"].asInt());
    pbc.setY(systemInformation_["periodic_boundary_conditions"]["y"].asInt());
    pbc.setZ(systemInformation_["periodic_boundary_conditions"]["z"].asInt());

    for (int i = 0; i < particles_.size(); ++i)
    {
        particles_[i].clearNeighbors();
        for (int j = 0; j < particles_.size(); ++j)
        {
            if ((particles_[i].getId() != particles_[j].getId()) &&
                (distancePbc(particles_[i].getPosition(), 
                             particles_[j].getPosition(), 
                             dymensions_, pbc            ) <= cutOff))
                particles_[i].addNeighbor(j);
        }
    }
}



std::vector<Particle> System::getParticles()
{
    return particles_;
}



Json::Value System::getSystemInformation()
{
    return systemInformation_;
}



Json::Value System::getInteractionInformation()
{
    return interactionInformation_;
}



std::map<std::string, Vector*> System::getFields()
{
    return fields_;
}



Vector System::getDymensions()
{
    return dymensions_;
}



float System::getThermalEnergy()
{
    return thermalEnergy_;
}



void System::setFields(const std::map<std::string, Vector*>& fields)
{
    fields_ = fields;
}



void System::setThermalEnergy(const float& thermalEnergy)
{
    thermalEnergy_ = thermalEnergy;

}



void System::addField(const std::string name, Vector& vector)
{
    fields_.insert(std::pair<std::string, Vector*>(name, &vector));
}



Vector& System::getField(const std::string name)
{
    return *(fields_[name]);
}



float System::computeEnergy()
{
    double energy = 0.0;
    for (int id = 0; id < particles_.size(); id++)
        energy += computeEnergyContribution_(id);
    return energy;
}



void System::monteCarloThermalStep(bool needNeighborUpdate)
{
    if (needNeighborUpdate == true)
        findNeighbors_();

    int i = 0;
    float oldEnergy;
    float energyDelta;
    float radiusSpin = systemInformation_["update_policy"]["radius_spin"].asFloat();

    for (int iii = particles_.size() - 1; iii > 0; --iii)
    {
        i = rand() % particles_.size();        

        oldEnergy = computeEnergyContribution_(i);
        particles_[i].updateSpin(1);
        energyDelta = computeEnergyContribution_(i) - oldEnergy;

        if (energyDelta <= 0.0f)
        {
            onEventCb_(particles_[i], energyDelta);
        }
        else if (drand48() <= exp(- energyDelta / thermalEnergy_))
        {
            onEventCb_(particles_[i], energyDelta);
        }
        else
        {
            particles_[i].rollBackSpin();
        }


    }
}



void System::monteCarloDynamicStep(bool needNeighborUpdate)
{
    if (needNeighborUpdate == true)
    {
        findNeighbors_();
    }

    float oldEnergy;
    float energyDelta;
    float radiusPosition = systemInformation_["update_policy"]["radius_position"].asFloat(); 
    for (int i = 0; i < particles_.size(); ++i)
    {

        oldEnergy = computeTotalEnergyContribution_(i);
        particles_[i].updatePosition(radiusPosition);
        energyDelta = computeTotalEnergyContribution_(i) - oldEnergy;
        if (energyDelta <= 0)
        {
            onEventCb_(particles_[i], energyDelta);
        }
        else if (drand48() <= exp(-energyDelta / thermalEnergy_))
        {
            onEventCb_(particles_[i], energyDelta);
        }
        else
        {
            particles_[i].rollBackPosition();
        }
    }
}



float System::computeFieldContribution_(int id)
{
    return - particles_[id].getCharge() * dot(*(fields_["Electric"]) ,
                                              particles_[id].getPosition());
}



float System::computeInteractionContribution_(int id)
{
    float sum = 0;
    float J = interactionInformation_["all"]["J"].asFloat();
    float K_0 = interactionInformation_["all"]["K_0"].asFloat();
    float I_0 = interactionInformation_["all"]["I_0"].asFloat();
    std::vector<int> neighbors = particles_[id].getNeighbors();


    for (int i = 0; i < neighbors.size(); ++i)

    {
        if (particles_[id].getType() == "Ión" && particles_[i].getType() == "Ión")
        {
            sum = - J * dot(particles_[id].getSpin(), particles_[i].getSpin());
        }
        else if (particles_[id].getType() == "Electrón" && particles_[i].getType() == "Electrón")
        {
            sum = - K_0 * exp(- distance(particles_[id].getPosition(), particles_[i].getPosition())) * dot(particles_[id].getSpin(), particles_[i].getSpin());
        }
        else if ((particles_[id].getType() == "Electrón" && particles_[i].getType() == "Ión") || (particles_[id].getType() == "Ión" && particles_[i].getType() == "Electrón"))
        {
            sum = - I_0 * exp(- distance(particles_[id].getPosition(), particles_[i].getPosition())) * dot(particles_[id].getSpin(), particles_[i].getSpin());
        }
        else
        {
            throw std::exception();
        }
    }

    return sum;
}



void  System::onEventCb_(Particle& particle, float energyDelta)
{
    std::cout << particle.getOldSpin() << "    " << particle.getSpin() << std::endl;
}



float System::computeEnergyContribution_(int id)
{
    return computeFieldContribution_(id) + 
           computeInteractionContribution_(id);
}



float System::computeTotalEnergyContribution_(int id)
{
    return computeFieldContribution_(id) + 
           2.0 * computeInteractionContribution_(id);
}

