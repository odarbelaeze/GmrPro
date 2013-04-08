// Header files for the GMR program

#include <cmath>
#include <exception>
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <valarray>

// Types and structs

namespace gmr
{
    typedef std::valarray<double> darray;
    enum class Spin { Up, Down };
    enum class Lattice { sc, fcc, bcc };
    enum class Specie { Ion, Electron };

    class Particle
    {
    public:
        Particle ();
        Particle (darray, Spin, Specie);
        ~Particle ();

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

    int operator * (const Spin&, const Spin&);
    Spin operator - (const Spin&);
}

// System construction helpers

namespace gmr
{
    void insertParticles (std::vector<Particle>&, Specie, Lattice, int, int, int);
    void insertParticles (std::vector<Particle>&, Specie, Lattice, std::initializer_list<int>);
    void insertParticles (std::vector<Particle>&, Specie, int, int, int, int);
    void insertParticles (std::vector<Particle>&, Specie, int, std::initializer_list<int>);
    void updateNeighbors (std::vector<Particle>&, double);
}

// Monte carlo stuff

namespace gmr
{
    void mcStep (std::vector<Particle>&, 
                 std::function<double(const Particle&)>,
                 std::function<double()>,
                 double);
    double energy (std::vector<Particle>&, 
                   std::function<double(const Particle&)>);
    double magnetization (const std::vector<Particle>&);
}

// Mathematic helpers

namespace gmr
{
    double norm(const darray&);
    double stddev (double, double);
}