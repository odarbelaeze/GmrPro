#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <cstdlib>
#include <string>
#include <cmath>
#include <vector>

#include <jsoncpp/json/json.h>

#include "simplevector.h"

#define TR(i, its) for(typeof(its.begin()) i = its.begin(); i != its.end(); i++)


class Particle
{
    private:
        Vector              position_;
        Vector              spin_;
        Vector              oldPosition_;
        Vector              oldSpin_;
        std::vector<int>    neighbors_;
        int                 id_;
        std::string         type_;
        float               charge_;

        
    public:
        Particle();
        ~Particle();

        void updateSpin(float);
        void updatePosition(float);
        void rollBackPosition();
        void rollBackSpin();

        void setPosition(const Vector&);
        void setSpin(const Vector&);
        void setOldPosition(const Vector&);
        void setOldSpin(const Vector&);
        void setNeighbors(const std::vector<int>&);
        void addNeighbor(const int);
        void clearNeighbors();
        void setId(int);
        void setType(const std::string&);
        void setCharge(const float);

        const Vector            getPosition();
        const Vector            getSpin();
        const Vector            getOldPosition();
        const Vector            getOldSpin();
        const std::vector<int>  getNeighbors();
        const int               getId();
        const std::string       getType();
        const float             getCharge();

};
#endif