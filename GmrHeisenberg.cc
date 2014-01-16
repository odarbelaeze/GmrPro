#include "GmrHeisenberg.h"

void GmrHeisenberg::__init__()
{
    species_.resize(st_.particleCount());
    positions_.resize(st_.particleCount());
    spins_.resize(st_.particleCount());

    ionNbs_.resize(st_.particleCount());
    electronNbs_.resize(st_.particleCount());

    species_ = Specie::Electron;
    species_[std::slice(0, st_.ionCount(), 1)] = Specie::Ion;

    ddarray basis = st_.getBasis();

    int id = 0;

    for (int i = 0; i < st_.w; ++i)
    {
        for (int j = 0; j < st_.l; ++j)
        {
            for (int k = 0; k < st_.h; ++k)
            {
                for (int atom = 0; atom < basis.size(); ++atom)
                {
                    positions_[id++] = darray{ 
                                           (double) i, 
                                           (double) j, 
                                           (double) k 
                                       } + basis[atom];
                }
            }
        }
    }

    for (int i = 0; i < st_.electronCount(); ++i)
    {
        positions_[id++] = darray{
            st_.w * real(engine),
            st_.l * real(engine),
            st_.h * real(engine)
        };
    }

    for (int i = 0; i < st_.particleCount(); ++i)
    {
        spins_[i] = randomSpin();
    }

    // Update neigbors    
}


double GmrHeisenberg::interactionEnergyIon_(int i, darray spin, darray pos)
{
    ddarray ionDots = std::valarray<darray>(spins_[ionNbs_[i]]) * spin;
    ddarray eleDots = std::valarray<darray>(spins_[electronNbs_[i]]) * spin;
    ddarray ionDists = std::valarray<darray>(positions_[ionNbs_[i]]) - pos;
    ddarray eleDists = std::valarray<darray>(positions_[electronNbs_[i]]) - pos;

    double energy = 0.0;

    for (int j = 0; j < ionNbs_.size(); ++j)
    {
        energy += std::exp( - it_.pauli * norm(ionDists[i]));
        energy -= it_.ii * norm(ionDots[j]);
    }

    for (int j = 0; j < electronNbs_.size(); ++j)
    {
        energy += std::exp(1.0 / (it_.pauli * norm(eleDists[i])));
        energy -= it_.ei * norm(eleDots[j]) * std::exp( - norm(eleDists[i]));
    }

    return energy;
}


double GmrHeisenberg::interactionEnergyElectron_(int i, darray spin, darray pos)
{
    ddarray ionDots = std::valarray<darray>(spins_[ionNbs_[i]]) * spin;
    ddarray eleDots = std::valarray<darray>(spins_[electronNbs_[i]]) * spin;
    ddarray ionDists = std::valarray<darray>(positions_[ionNbs_[i]]) - pos;
    ddarray eleDists = std::valarray<darray>(positions_[electronNbs_[i]]) - pos;

    double energy = 0.0;

    for (int j = 0; j < ionNbs_.size(); ++j)
    {
        energy += std::exp( - it_.pauli * norm(ionDists[i]));
        energy -= it_.ei * norm(ionDots[j]);
    }

    for (int j = 0; j < electronNbs_.size(); ++j)
    {
        energy += std::exp(1.0 / (it_.pauli * norm(eleDists[i])));
        energy -= it_.ee * norm(eleDots[j]) * std::exp( - norm(eleDists[i]));
    }

    return energy;
}


inline double GmrHeisenberg::deltaEnergyDynamic_(int i, darray newPos)
{
    return energy(i, spins_[i], newPos) - energy(i);
}


inline double GmrHeisenberg::deltaEnergyThermal_(int i, darray newSpin)
{
    return energy(i, newSpin, positions_[i]) - energy(i);
}


GmrHeisenberg::GmrHeisenberg(
    const SampleTraits& st, 
    const InteractionTraits& it, 
    ExternalTraits& et)

 : st_(st), it_(it), et_(&et)
{
    __init__();
}


GmrHeisenberg::~GmrHeisenberg()
{

}


darray GmrHeisenberg::randomSpin()
{
    double theta = 2.0 * M_PI * real(engine);
    double phi   = 0.0 * M_PI * real(engine);
    return darray {
        std::cos(theta) * sin(phi),
        std::sin(theta) * sin(phi),
        std::cos(phi)
    };
}


darray GmrHeisenberg::perturbedSpin(const darray& us)
{
    darray d = us + randomSpin();
    d = d / norm(d);
    return d;
}


double GmrHeisenberg::interactionEnergy(int i)
{
    if (species_[i] == Specie::Electron) 
    {
        return interactionEnergyElectron_(i, spins_[i], positions_[i]);
    }

    return interactionEnergyIon_(i, spins_[i], positions_[i]);
}


double GmrHeisenberg::interactionEnergy(int i, darray spin, darray pos)
{
    if (species_[i] == Specie::Electron) 
    {
        return interactionEnergyElectron_(i, spin, pos);
    }

    return interactionEnergyIon_(i, spin, pos);
}


double GmrHeisenberg::externalEnergy(int i)
{
    return externalEnergy(i, spins_[i], positions_[i]);
}


double GmrHeisenberg::externalEnergy(int i, darray spin, darray pos)
{
    double energy = 0;
    energy -= (et_ -> electricField * pos).sum();
    energy -= (et_ -> magneticField * spin).sum();
    energy -= std::pow((st_.easyAxis * spin).sum(), 2);
    return energy;
}


inline double GmrHeisenberg::energy(int i)
{
    return interactionEnergy(i) + externalEnergy(i);
}


inline double GmrHeisenberg::energy(int i, darray spin, darray pos)
{
    return interactionEnergy(i, spin, pos) + externalEnergy(i, spin, pos);
}


double GmrHeisenberg::energy()
{
    double e = 0.0;
    for (int i = 0; i < st_.particleCount(); ++i)
    {
        e += energy(i);
    }
    return e;
}


inline darray GmrHeisenberg::magnetization()
{
    return spins_.sum();
}


std::ostream& operator << (std::ostream& os, GmrHeisenberg& gmr)
{
    for (int i = 0; i < gmr.st_.particleCount(); ++i)
    {
        os << gmr.positions_[i] << " " << gmr.spins_[i] << " "
           << (gmr.species_[i] == Specie::Ion ? "Ion" : "Electron")
           << std::endl;
    }
    return os;
}

