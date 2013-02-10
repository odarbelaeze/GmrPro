#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include <cstdlib>
#include <string>
#include <cmath>
#include <vector>

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
        Particle(const std::string&);
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

        Vector            getPosition();
        Vector            getSpin();
        Vector            getOldPosition();
        Vector            getOldSpin();
        std::vector<int>  getNeighbors();
        int               getId();
        std::string       getType();
        float             getCharge();

};

#endif
