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
        const std::vector<Vector*>& getFields();
        const Vector& getDymensions();
        const float& getThermalEnergy();

        void setFields(const std::vector<Vector*>&);
        void setThermalEnergy(const float&);

        void monteCarloThermalStep(bool);
        void monteCarloDynamicStep(bool);

    protected:
        std::vector<Particle> particles_;
        Json::Value systemInformation_;
        Json::Value interactionInformation_;
        std::vector<Vector*> fields_;
        Vector dymensions_;

        virtual float computeFieldContribution(int);
        virtual float computeInteractionContribution(int);
        virtual void  onEventCb(const Particle&, float, float);

    private:
        float thermalEnergy_;
        float time;

        void initSystem(const Json::Value&);
        void findNeighbors();
        float computeEnergyContribution(int);
        float computeTotalEnergyContribution(int);

};


#endif