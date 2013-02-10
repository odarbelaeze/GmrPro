#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <jsoncpp/json/json.h>

#include "particle.h"

#define TR(i, its) for(typeof(its.begin()) i = its.begin(); i != its.end(); i++)

typedef Vector Field;
typedef float (*PFI)(Particle &, Field &, Json::Value&);
typedef float (*PPI)(Particle &, Particle &, Json::Value&);

class System
{
    public:
        System();
        System(const Json::Value&);
        System(const std::string);
        System(const istream&);
        ~System();

        void findNeighbors();

    protected:
        std::vector<Particle> particles_;
        Json::Value systemInformation_;
        Json::Value interactionInformation_;
        
        virtual float computeEnergyContribution(int);
        virtual float computeTotalEnergyContribution(int);

    private:
        float thermalEnergy_;

        void create_system (const Json::Value&);

};


#endif