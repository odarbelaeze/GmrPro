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

    public:
        Particle();
        Particle(const std::string&);
        ~Particle();

        void updateSpin(double);
        void updatePosition(double);
        void rollBackPosition();
        void rollBackSpin();

        void setPosition(const Vector&);
        void setSpin(const Vector&);
        void setOldPosition(const Vector&);
        void setOldSpin(const Vector&);
        void setNeighbors(const std::vector<int>&);
        void addNeighbor(int);
        void clearNeighbors();
        void setId(int);
        void setType(const std::string&);
        void setCharge(const double);
        void setMovable(bool);

        void pacmanEffect(const Vector&);

        Vector            getPosition();
        Vector            getSpin();
        Vector            getOldPosition();
        Vector            getOldSpin();
        std::vector<int>  getNeighbors();
        int               getId();
        std::string       getType();
        double             getCharge();
        bool              getMovable();

    private:
        Vector              position_;
        Vector              spin_;
        Vector              oldPosition_;
        Vector              oldSpin_;
        std::vector<int>    neighbors_;
        int                 id_;
        bool                isMovable_;
        std::string         type_;
        double               charge_;
};

#endif
