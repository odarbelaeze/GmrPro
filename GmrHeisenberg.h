#ifndef GMR_HEISENBERG_H_
#define GMR_HEISENBERG_H_

#include "helpers.h"

class GmrHeisenberg
{

public:
    GmrHeisenberg(const SampleTraits&, const InteractionTraits&, ExternalTraits*);
    ~GmrHeisenberg();

    darray randomSpin();
    darray perturbedSpin(const darray&);

    double interactionEnergy(int) const;
    double interactionEnergy(int, darray, darray) const;
    double externalEnergy(int) const;
    double externalEnergy(int, darray, darray) const;
    double energy(int) const;
    double energy(int, darray, darray) const;

    double energy() const;
    darray magnetization() const;

    void updateNbh();
    std::vector<DynamicEvent> dynamicStep();
    std::vector<ThermalEvent> thermalStep();

    friend std::ostream& operator << (std::ostream&, GmrHeisenberg&);

protected:
    SampleTraits st_;
    InteractionTraits it_;
    ExternalTraits* et_;

    std::valarray<Specie> species_;
    ddarray positions_;
    ddarray spins_;
    darray dims_;

    std::valarray<std::valarray<size_t> > ionNbs_;
    std::valarray<std::valarray<size_t> > electronNbs_;

    std::mt19937_64 engine_;
    std::uniform_real_distribution<> real_;

    ExaustDeck electrons_;
    ExaustDeck particles_;

private:
    void __init__();

    double interactionEnergyIon_(int, darray, darray) const;
    double interactionEnergyElectron_(int, darray, darray) const;

    double deltaEnergyDynamic_(int, darray) const;
    double deltaEnergyThermal_(int, darray) const;

};

#endif