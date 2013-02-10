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
    private:
        std::vector<Particle> particles_;
        float thermalEnergy_;
        Json::Value systemInformation_;

        void create_system (const Json::Value&);
    public:
        System();
        ~System();

        void findNeighbors();
};


#endif