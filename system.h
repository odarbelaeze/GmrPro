#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <exception>

#include <jsoncpp/json/json.h>

#include "particle.h"

#define TR(i, its) for(typeof(its.begin()) i = its.begin(); i != its.end(); i++)

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
        System(const std::istream&);
        ~System();

        const std::vector<Particle>& getParticles();
        const Json::Value& getSystemInformation();
        const Json::Value& getInteractionInformation();
        const std::map<std::string, Vector*>& getFields();
        const Vector& getDymensions();
        const float& getThermalEnergy();

        void setFields(const std::map<std::string, Vector*>&);
        void setThermalEnergy(const float&);

        void monteCarloThermalStep(bool);
        void monteCarloDynamicStep(bool);

    protected:
        std::vector<Particle> particles_;
        Json::Value systemInformation_;
        Json::Value interactionInformation_;
        std::map<std::string, Vector*> fields_;
        Vector dymensions_;

        virtual float computeFieldContribution_(int);
        virtual float computeInteractionContribution_(int);
        virtual void  onEventCb_(const Particle&, float, float);

    private:
        float thermalEnergy_;
        float time_;

        void initSystem_(const Json::Value&);
        void findNeighbors_();
        float computeEnergyContribution_(int);
        float computeTotalEnergyContribution_(int);

};


#endif