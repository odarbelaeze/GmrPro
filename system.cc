#include "system.h"

BadDescriptorException::BadDescriptorException() : error_("Unknown")
{

}



BadDescriptorException::BadDescriptorException(const std::string error) : error_(error)
{

}



const char* BadDescriptorException::what() const throw()
{
    return ("The descriptor is invalid: " + error_).data();
}



System::System()
{

}



System::~System()
{

}



void System::initSystem_(const Json::Value& root)
{
    Vector vectorTemplate;
    systemInformation_ = root["system"];

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
                    particleTemplate.setSpin(randomVector());
                    particleTemplate.setId(particles_.size());
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
                    particleTemplate.setSpin(randomVector());
                    particleTemplate.setId(particles_.size()); 
                    particles_.push_back(particleTemplate);

                    vectorTemplate.setX((i + 0.5f) * scale);
                    vectorTemplate.setY((j + 0.5f) * scale);
                    vectorTemplate.setZ((k + 0.5f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setSpin(randomVector());
                    particleTemplate.setId(particles_.size()); 
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
                    particleTemplate.setSpin(randomVector());
                    particleTemplate.setId(particles_.size()); 
                    particles_.push_back(particleTemplate);


                    vectorTemplate.setX((i + 0.5f) * scale);
                    vectorTemplate.setY((j + 0.5f) * scale);
                    vectorTemplate.setZ((k + 0.0f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setSpin(randomVector());
                    particleTemplate.setId(particles_.size()); 
                    particles_.push_back(particleTemplate);
                    

                    vectorTemplate.setX((i + 0.0f) * scale);
                    vectorTemplate.setY((j + 0.5f) * scale);
                    vectorTemplate.setZ((k + 0.5f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setSpin(randomVector());
                    particleTemplate.setId(particles_.size()); 
                    particles_.push_back(particleTemplate);
                    

                    vectorTemplate.setX((i + 0.5f) * scale);
                    vectorTemplate.setY((j + 0.0f) * scale);
                    vectorTemplate.setZ((k + 0.5f) * scale);
                    particleTemplate.setPosition(vectorTemplate);
                    particleTemplate.setSpin(randomVector());
                    particleTemplate.setId(particles_.size()); 
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
        particleTemplate.setPosition(dymensions_ * randomVector());
        particleTemplate.setId(particles_.size()); 
        particles_.push_back(particleTemplate);
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

    TR(p, particles_)
    {
        (*p).clearNeighbors();
        TR(other, particles_)
            if (p != other)
                if (distancePbc((*p).getPosition(), (*other).getPosition(), 
                                dymensions_, pbc) <= cutOff)
                    (*p).addNeighbor((*other).getId());
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


const std::map<std::string, Vector*>& System::getFields()
{
    return fields_;
}


const Vector& System::getDymensions()
{
    return dymensions_;
}


const float& System::getThermalEnergy()
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


void System::monteCarloThermalStep(bool needNeighborUpdate)
{
    if (needNeighborUpdate == true)
        findNeighbors_();
}


void System::monteCarloDynamicStep(bool needNeighborUpdate)
{
    if (needNeighborUpdate == true)
        findNeighbors_();
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


    for (typeof(particles_[id].getNeighbors().begin()) i = particles_[id].getNeighbors().begin(); i != particles_[id].getNeighbors().end(); ++i)
    {
        if (*i != id)
        {

            if (particles_[id].getType() == "Ión" && particles_[*i].getType() == "Ión")
            {
                sum = -J * dot(particles_[id].getSpin(), particles_[*i].getSpin());
            }
            else if (particles_[id].getType() == "Electrón" && particles_[*i].getType() == "Electrón")
            {
                sum = K_0 * exp(- distance(particles_[id].getPosition(), particles_[*i].getPosition())) * dot(particles_[id].getSpin(), particles_[*i].getSpin());
            }
            else if ((particles_[id].getType() == "Electrón" && particles_[*i].getType() == "Ión") || (particles_[id].getType() == "Ión" && particles_[*i].getType() == "Electrón"))
            {
                sum = I_0 * exp(- distance(particles_[id].getPosition(), particles_[*i].getPosition())) * dot(particles_[id].getSpin(), particles_[*i].getSpin());
            }
            
        }
    }

    return sum;
}


void  System::onEventCb_(const Particle&, float, float)
{
}
