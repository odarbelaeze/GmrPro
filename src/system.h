#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <exception>
#include <fstream>

#include <jsoncpp/json/json.h>

#include "simplevector.h"
#include "particle.h"

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

        const std::vector<Particle>&    getParticles();
        const Json::Value&              getSystemInformation();
        const Json::Value&              getInteractionInformation();
        Vector                          getDimensions();
        float                           getThermalEnergy();
        float                           getEnergy();
        float                           computeEnergy();

        void setThermalEnergy(const float&);
        void resetSystem();

        void monteCarloThermalStep(bool needNeighborUpdate, bool callback = true);
        void monteCarloDynamicStep(bool needNeighborUpdate, bool callback = true);

    protected:
        Vector                  boundaryConditions_;
        Vector                  dimensions_;
        float                   energy_;
        Json::Value             interactionInformation_;
        float                   muffinTinRadi_;
        float                   neighborCutOff_;
        std::vector<Particle>   particles_;
        float                   scale_;
        std::string             structure_;
        Json::Value             systemInformation_;
        float                   thermalEnergy_;
        unsigned long int       time_;

        virtual float computeFieldContribution_(int);
        virtual float computeInteractionContribution_(int);
        virtual void  onThermalEventCb_(Particle&, float);
        virtual void  onDynamicEventCb_(Particle&, float);

    private:

        void    initSystem_(const Json::Value&);
        void    findNeighbors_();
        void    checkCloseNeighbors_();
        float   computeEnergyContribution_(int);

};


#endif