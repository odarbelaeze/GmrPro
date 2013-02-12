#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <exception>
#include <fstream>

#include <jsoncpp/json/json.h>

#include "particle.h"

// #define TR(i, its) for(typeof(its.begin()) i = its.begin(); i != its.end(); i++)

typedef Vector Field;
typedef float (*PFI)(Particle &, Field &, Json::Value&);
typedef float (*PPI)(Particle &, Particle &, Json::Value&);


class BadDescriptorException
{
    public:
        BadDescriptorException();
        BadDescriptorException(const std::string);
        virtual const char* what() const throw();

    protected:
        std::string error_;

};


class System
{
    public:
        System();
        System(const Json::Value&);
        System(const std::string);
        System(const char*);
        System(std::istream&);
        ~System();

        std::vector<Particle> getParticles();
        Json::Value getSystemInformation();
        Json::Value getInteractionInformation();
        std::map<std::string, Vector*> getFields();
        Vector getDymensions();
        float getThermalEnergy();
        float getEnergy();

        void setFields(const std::map<std::string, Vector*>&);
        void setThermalEnergy(const float&);

        void addField(const std::string, Vector&);
        Vector& getField(const std::string);

        float computeEnergy();
        void monteCarloThermalStep(bool);
        void monteCarloDynamicStep(bool);

    protected:
        std::vector<Particle> particles_;
        Json::Value systemInformation_;
        Json::Value interactionInformation_;
        std::map<std::string, Vector*> fields_;
        Vector dymensions_;
        Vector pbc_;

        virtual float computeFieldContribution_(int);
        virtual float computeInteractionContribution_(int);
        virtual void  onThermalEventCb_(Particle&, float);
        virtual void  onDynamicEventCb_(Particle&, float);

    private:
        float thermalEnergy_;
        float time_;
        float energy_;

        void initSystem_(const Json::Value&);
        void findNeighbors_();
        float computeEnergyContribution_(int);
        float computeTotalEnergyContribution_(int);

};


#endif