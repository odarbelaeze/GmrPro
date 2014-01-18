#ifndef HELPERS_H_
#define HELPERS_H_

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <valarray>

typedef std::valarray<double> darray;
typedef std::valarray<darray> ddarray;

inline double norm(darray da)
{
    return std::sqrt(std::pow(da, 2.0).sum());
}

inline double distance(darray da, darray db)
{
    return norm(db - da);
}

template<typename T>
std::ostream& operator << (std::ostream& os, std::valarray<T> da)
{
    for (auto&& i : da)
    {
        os << i << " ";
    }
    return os;
}

enum class Lattice { sc, bcc, fcc };
enum class Specie { Ion, Electron };

struct SampleTraits
{
    int w;
    int h;
    int l;
    int nElectrons;
    Lattice lt;
    darray easyAxis;

    int ionCount() const
    {
        int n = w * h * l;
        switch (lt) 
        {
            case Lattice::sc :
                return n;
                break;

            case Lattice::fcc :
                return 4 * n;
                break;

            case Lattice::bcc :
                return 2 * n;
                break;
        
            default:
                return 0;
                break;
        }
    }

    int electronCount() const
    {
        return nElectrons;
    }

    int particleCount() const
    {
        return ionCount() + electronCount();
    }

    ddarray getBasis() const
    {
        ddarray basis;

        switch (lt)
        {
            case Lattice::sc :
                basis = { { 0.0, 0.0, 0.0 } };
                break;

            case Lattice::bcc :
                basis = { { 0.0, 0.0, 0.0 }, 
                          { 0.5, 0.5, 0.5 } };
                break;

            case Lattice::fcc :
                basis =  { { 0.0, 0.0, 0.0 }, 
                           { 0.5, 0.5, 0.0 }, 
                           { 0.5, 0.0, 0.5 }, 
                           { 0.0, 0.5, 0.5 } };
                break;
        }

        return basis;
    }

    int getBasisSize() const
    {
        int basis;

        switch (lt)
        {
            case Lattice::sc :
                basis = 1;
                break;

            case Lattice::bcc :
                basis = 2;
                break;

            case Lattice::fcc :
                basis = 4;
                break;
        }

        return basis;

    }

};

struct InteractionTraits
{
    double ee;
    double ei;
    double ii;
    double pauli;
    double eeCutOff;
    double eiCutOff;
    double iiCutOff;
};

struct ExternalTraits
{
    darray electricField;
    darray magneticField;
    double temperature;
};

class ExaustDeck
{
public:
    ExaustDeck(int n)
    {
        data_ = new int[n];
        
        for (int i = 0; i < n; ++i)
        {
            data_[i] = i;
        }

        count_ = n;
        top_ = n;
    }

    ~ExaustDeck() {}

    int pop()
    {
        if (top_ == 0) top_ = count_;
        int item = std::rand() % top_;
        int temp = data_[--top_];
        data_[top_] = data_[item];
        data_[item] = temp;
        return data_[top_];
    }

private:
    int* data_;
    int count_;
    int top_;

};


inline bool electronIon(Specie& a, Specie& b)
{
    return (a == Specie::Ion && b == Specie::Electron) || 
           (a == Specie::Electron && b == Specie::Ion);
}


inline bool electronElectron(Specie& a, Specie& b)
{
    return a == Specie::Electron && b == Specie::Electron;
}


inline bool ionIon(Specie& a, Specie& b)
{
    return a == Specie::Ion && b == Specie::Ion;
}

inline double modclap(double a, double m)
{
    if (a < 0) return modclap(a + m, m);
    return std::fmod(a, m);
}


template<typename T>
std::valarray<T> modclap(std::valarray<T> a, std::valarray<T> m)
{
    std::valarray<T> mod;
    mod.resize(a.size());
    for (int i = 0; i < a.size(); ++i)
    {
        mod[i] = modclap(a[i], m[i]);
    }
    return mod;
}


struct DynamicEvent
{
    int particleId;
    darray oldPosition;
    darray newPosition;
    double deltaEnergy;
};

struct ThermalEvent
{
    int particleId;
    darray oldSpin;
    darray newSpin;
    double deltaEnergy;
};

#endif