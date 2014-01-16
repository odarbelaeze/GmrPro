#ifndef GMR_HEISENBERG_H_
#define GMR_HEISENBERG_H_

#include "helpers.h"

class GmrHeisenberg
{

public:
    GmrHeisenberg(const SampleTraits&, const InteractionTraits&, ExternalTraits&);
    ~GmrHeisenberg();

    darray randomSpin();
    darray perturbedSpin(const darray&);

    double interactionEnergy(int);
    double interactionEnergy(int, darray, darray);
    double externalEnergy(int);
    double externalEnergy(int, darray, darray);
    double energy(int);
    double energy(int, darray, darray);

    double energy();
    darray magnetization();

    void updateNbh();
    void dynamicStep();
    void thermalStep();

    friend std::ostream& operator << (std::ostream&, GmrHeisenberg&);

protected:
    SampleTraits st_;
    InteractionTraits it_;
    ExternalTraits* et_;

    std::valarray<Specie> species_;
    ddarray positions_;
    ddarray spins_;

    std::valarray<std::valarray<size_t> > ionNbs_;
    std::valarray<std::valarray<size_t> > electronNbs_;

    std::mt19937_64 engine;
    std::uniform_real_distribution<> real;

private:
    void __init__();

    double interactionEnergyIon_(int, darray, darray);
    double interactionEnergyElectron_(int, darray, darray);

    double deltaEnergyDynamic_(int, darray);
    double deltaEnergyThermal_(int, darray);

};

#endif