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
        double                           getThermalEnergy();
        double                           getEnergy();
        unsigned long int               getTime();

        void setThermalEnergy(const double&);
        void resetSystem();

        void monteCarloThermalStep(bool needNeighborUpdate, bool callback = true);
        void monteCarloDynamicStep(bool needNeighborUpdate, bool callback = true);

    protected:
        Vector                  boundaryConditions_;
        Vector                  dimensions_;
        Json::Value             interactionInformation_;
        double                   muffinTinRadi_;
        double                   neighborCutOff_;
        std::vector<Particle>   particles_;
        double                   scale_;
        std::string             structure_;
        Json::Value             systemInformation_;
        double                   thermalEnergy_;

        virtual double computeFieldContribution_(int);
        virtual double computeInteractionContribution_(int);
        virtual void  onThermalEventCb_(Particle&, double);
        virtual void  onDynamicEventCb_(Particle&, double);

    private:
        double                   energy_;
        unsigned long int       time_;

        void    initSystem_(const Json::Value&);
        void    findNeighbors_();
        void    checkCloseNeighbors_();
        double   computeEnergyContribution_(int);
        double   computeRelatedEnergy_(int);
        double   computeEnergy_();

};


#endif