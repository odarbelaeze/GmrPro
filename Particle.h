#ifndef PARTICLE_H_
#define PARTICLE_H_ 

#include <valarray>
#include <vector>

namespace gmr
{
    typedef std::valarray<double> darray;
    enum class Spin { Up, Down };
    enum class Specie { Ion, Electron };

    class Particle
    {
    public:
        Particle();
        Particle (darray, Spin, Specie);
        ~Particle();

        darray getPosition () const;
        Spin getSpin () const;
        Specie getSpecie () const;
        std::vector<Particle*> getNbh () const;

        void setPosition (const darray&);
        void setSpin (const Spin&);
        void setSpecie (const Specie&);
        void setNbh (const std::vector<Particle*>&);

    private:
        darray position_;
        Spin spin_;
        Specie specie_;
        std::vector<Particle*> nbh_;
    };

    int operator* (const Spin&, const Spin&);
    Spin operator- (const Spin&);

}

#endif