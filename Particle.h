#ifndef PARTICLE_H_
#define PARTICLE_H_ 

#include <valarray>
#include <vector>

#include "Helpers.h"

namespace Gmr
{
    enum class Spin { Up, Down };
    enum class Specie { Ion, Electron };

    int operator* (const Spin&, const Spin&);
    double operator* (const Spin&, const double);
    double operator* (const double, const Spin&);
    Spin operator- (const Spin&);
    int delta(const Spin&, const Spin&);

    class Particle
    {
    public:
        Particle();
        Particle (darray, Spin, Specie);
        ~Particle();

        darray getPosition () const;
        Spin getSpin () const;
        double getCharge() const;
        Specie getSpecie () const;
        std::vector<Particle*> getNbh () const;
        std::vector<Particle*> getNnbh () const;

        void setPosition (const darray&);
        void setSpin (const Spin&);
        void setCharge(double);
        void setSpecie (const Specie&);
        void setNbh (const std::vector<Particle*>&);
        void setNnbh (const std::vector<Particle*>&);

    private:
        darray position_;
        Spin spin_;
        double charge_;
        Specie specie_;
        std::vector<Particle*> nnbh_;
        std::vector<Particle*> nbh_;
    };
}

#endif