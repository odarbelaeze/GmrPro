#include "system.h"

System::System(){}

System::~System(){}


void System::create_system(const Json::Value& root)
{
    Vector vectorTemplate;
    systemInformation_ = root["system"];


    int width = systemInformation_["dimensions"]["width"].asInt();
    int lenght = systemInformation_["dimensions"]["lenght"].asInt();
    int height = systemInformation_["dimensions"]["height"].asInt();
    int scale = systemInformation_["scale"].asInt();
    std::string structure = systemInformation_["structure"].asString();

    particles_.clear();


    Particle particleTemplate("Ión");
    if (structure == "sc")
    {
        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < lenght; ++j)
            {
                for (int k = 0; k < height; ++k)
                {
                    vectorTemplate.setX((i + 0.0f) * scale);
                    vectorTemplate.setY((j + 0.0f) * scale);
                    vectorTemplate.setZ((k + 0.0f) * scale);
                    particleTemplate.position_ = vectorTemplate;
                    particleTemplate.spin_ = randomVector();
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
                    particleTemplate.position_ = vectorTemplate;
                    particleTemplate.spin_ = randomVector();
                    particles_.push_back(particleTemplate);

                    vectorTemplate.setX((i + 0.5f) * scale);
                    vectorTemplate.setY((j + 0.5f) * scale);
                    vectorTemplate.setZ((k + 0.5f) * scale);
                    particleTemplate.position_ = vectorTemplate;
                    particleTemplate.spin_ = randomVector();
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
                    particleTemplate.position_ = vectorTemplate;
                    particleTemplate.spin_ = randomVector();
                    particles_.push_back(particleTemplate);


                    vectorTemplate.setX((i + 0.5f) * scale);
                    vectorTemplate.setY((j + 0.5f) * scale);
                    vectorTemplate.setZ((k + 0.0f) * scale);
                    particleTemplate.position_ = vectorTemplate;
                    particleTemplate.spin_ = randomVector();
                    particles_.push_back(particleTemplate);
                    

                    vectorTemplate.setX((i + 0.0f) * scale);
                    vectorTemplate.setY((j + 0.5f) * scale);
                    vectorTemplate.setZ((k + 0.5f) * scale);
                    particleTemplate.position_ = vectorTemplate;
                    particleTemplate.spin_ = randomVector();
                    particles_.push_back(particleTemplate);
                    

                    vectorTemplate.setX((i + 0.5f) * scale);
                    vectorTemplate.setY((j + 0.0f) * scale);
                    vectorTemplate.setZ((k + 0.5f) * scale);
                    particleTemplate.position_ = vectorTemplate;
                    particleTemplate.spin_ = randomVector();
                    particles_.push_back(particleTemplate);
                }
            }
        }
    }


    particleTemplate = Particle("Electrón");
    electronCount = this -> systemInformation_["number_of_free_electrons"].asInt();
    for (int i = 0; i < electronCount; ++i)
    {
        particleTemplate.position_ = randomVector();
        particles_.push_back(particleTemplate);
    }

    findNeighbors();
}



void System::findNeighbors()
{
    float cutOff = systemInformation_["cut_off_radius"].asFloat();
    
    int scale = systemInformation_["scale"].asInt();
    Vector vectorTemplate;
    systemDimensions.setX(systemInformation_["dimensions"]["width"].asFloat() * scale);
    systemDimensions.setY(systemInformation_["dimensions"]["height"].asFloat() * scale);
    systemDimensions.setZ(systemInformation_["dimensions"]["lenght"].asFloat() * scale);

    Vector periodicBoundaryConditions;
    periodicBoundaryConditions.setX(systemInformation_["periodic_boundary_conditions"]["x"].asInt());
    periodicBoundaryConditions.setY(systemInformation_["periodic_boundary_conditions"]["y"].asInt());
    periodicBoundaryConditions.setZ(systemInformation_["periodic_boundary_conditions"]["z"].asInt());

    TR(p, particles_)
    {
        (*p).neighbors_.clear();
        TR(other, particles_)
            if (p != other)
                if (distancePbc((*p).position_, (*other).position_, 
                    systemDimensions, periodicBoundaryConditions))
                    (*p).neighbors_.push_back((*other).id_);
    }


}
