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
            st_.w * real_(engine_),
            st_.l * real_(engine_),
            st_.h * real_(engine_)
        };
    }

    for (int i = 0; i < st_.particleCount(); ++i)
    {
        spins_[i] = randomSpin();
    }

    updateNbh();
}


double GmrHeisenberg::interactionEnergyIon_(int i, darray spin, darray pos) const
{
    ddarray ionDots = ddarray(spins_[ionNbs_[i]]) * spin;
    ddarray eleDots = ddarray(spins_[electronNbs_[i]]) * spin;
    ddarray ionDists = ddarray(positions_[ionNbs_[i]]) - pos;
    ddarray eleDists = ddarray(positions_[electronNbs_[i]]) - pos;

    double energy = 0.0;

    for (int j = 0; j < ionNbs_[i].size(); ++j)
    {
        // energy += std::exp(1.0 / (it_.pauli * norm(ionDists[j])));
        energy -= it_.ii * ionDots[j].sum();
    }

    for (int j = 0; j < electronNbs_[i].size(); ++j)
    {
        // energy += std::exp(1.0 / (it_.pauli * norm(eleDists[j])));
        energy -= it_.ei * eleDots[j].sum() * std::exp( - norm(eleDists[j]));
    }

    return energy;
}


double GmrHeisenberg::interactionEnergyElectron_(int i, darray spin, darray pos) const
{
    ddarray ionDots = ddarray(spins_[ionNbs_[i]]) * spin;
    ddarray eleDots = ddarray(spins_[electronNbs_[i]]) * spin;
    ddarray ionDists = ddarray(positions_[ionNbs_[i]]) - pos;
    ddarray eleDists = ddarray(positions_[electronNbs_[i]]) - pos;

    double energy = 0.0;

    for (int j = 0; j < ionNbs_[i].size(); ++j)
    {
        // energy += std::exp(1.0 / (it_.pauli * norm(ionDists[j])));
        energy -= it_.ei * ionDots[j].sum();
    }

    for (int j = 0; j < electronNbs_[i].size(); ++j)
    {
        // energy += std::exp(1.0 / (it_.pauli * norm(eleDists[j])));
        energy -= it_.ee * eleDots[j].sum() * std::exp( - norm(eleDists[j]));
    }

    return energy;
}


inline double GmrHeisenberg::deltaEnergyDynamic_(int i, darray newPosition) const
{
    return energy(i, spins_[i], newPosition) - energy(i);
}


inline double GmrHeisenberg::deltaEnergyThermal_(int i, darray newSpin) const
{
    return energy(i, newSpin, positions_[i]) - energy(i);
}


GmrHeisenberg::GmrHeisenberg(
    const SampleTraits& st, 
    const InteractionTraits& it, 
    ExternalTraits* et)

 : st_(st), it_(it), et_(et), 
   electrons_(st.electronCount()),
   particles_(st.particleCount())
{
    __init__();
}


GmrHeisenberg::~GmrHeisenberg()
{

}


darray GmrHeisenberg::randomSpin()
{
    double theta = 2.0 * M_PI * real_(engine_);
    double phi   = 1.0 * M_PI * real_(engine_);
    return darray {
        std::cos(theta) * std::sin(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(phi)
    };
}


darray GmrHeisenberg::perturbedSpin(const darray& us)
{
    darray d = us + randomSpin();
    d = d / norm(d);
    return d;
}


double GmrHeisenberg::interactionEnergy(int i) const
{
    if (species_[i] == Specie::Electron) 
    {
        return interactionEnergyElectron_(i, spins_[i], positions_[i]);
    }

    return interactionEnergyIon_(i, spins_[i], positions_[i]);
}


double GmrHeisenberg::interactionEnergy(int i, darray spin, darray pos) const
{
    if (species_[i] == Specie::Electron) 
    {
        return interactionEnergyElectron_(i, spin, pos);
    }

    return interactionEnergyIon_(i, spin, pos);
}


double GmrHeisenberg::externalEnergy(int i) const
{
    return externalEnergy(i, spins_[i], positions_[i]);
}


double GmrHeisenberg::externalEnergy(int i, darray spin, darray pos) const
{
    double energy = 0;
    energy -= (et_ -> electricField * pos).sum();
    energy -= (et_ -> magneticField * spin).sum();
    energy -= std::pow((st_.easyAxis * spin).sum(), 2);
    return energy;
}


double GmrHeisenberg::energy(int i) const
{
    return interactionEnergy(i) + externalEnergy(i);
}


double GmrHeisenberg::energy(int i, darray spin, darray pos) const
{
    return interactionEnergy(i, spin, pos) + externalEnergy(i, spin, pos);
}


double GmrHeisenberg::energy() const
{
    double e = 0.0;
    for (int i = 0; i < st_.particleCount(); ++i)
    {
        e += energy(i);
    }
    return e;
}


darray GmrHeisenberg::magnetization() const
{
    darray mag(3);
    for (int i = 0; i < spins_.size(); ++i)
    {
        mag += spins_[i];
    }
    return mag;
}


void GmrHeisenberg::updateNbh()
{
    std::vector<int> inbs;
    std::vector<int> enbs;
    double d;

    for (int i = 0; i < st_.particleCount(); ++i)
    {
        inbs.clear();
        enbs.clear();
        for (int j = 0; j < st_.particleCount(); ++j)
        {
            if (i != j)
            {
                d = distance(positions_[i], positions_[j]);
                if (electronElectron(species_[i], species_[j]) && d <= it_.eeCutOff)
                {
                    enbs.push_back(j);
                }
                if (electronIon(species_[i], species_[j]) && d <= it_.eiCutOff)
                {
                    if (species_[j] == Specie::Electron) enbs.push_back(j);
                    if (species_[j] == Specie::Ion) inbs.push_back(j);
                }
                if (ionIon(species_[i], species_[j]) && d <= it_.iiCutOff)
                {
                    inbs.push_back(j);
                }
            }
        }

        ionNbs_[i].resize(inbs.size());
        std::copy(begin(inbs), end(inbs), begin(ionNbs_[i]));
        
        electronNbs_[i].resize(enbs.size());
        std::copy(begin(enbs), end(enbs), begin(electronNbs_[i]));
    }
}


std::vector<DynamicEvent> GmrHeisenberg::dynamicStep()
{
    int id;

    darray newPosition(3);
    double delta;
    
    std::vector<DynamicEvent> events;
    DynamicEvent event;

    for (int i = 0; i < st_.electronCount(); ++i)
    {
        id = st_.ionCount() + electrons_.pop();
        newPosition = positions_[id] + 0.5 * randomSpin();
        delta = deltaEnergyDynamic_(id, newPosition);

        if (real_(engine_) < std::exp( - delta / et_ -> temperature))
        {
            event.particleId = id;
            event.oldPosition = positions_[id];
            event.newPosition = newPosition;
            event.deltaEnergy = delta;
            events.push_back(event);
            positions_[id] = modclap(
                newPosition, 
                darray{(double) st_.w, (double) st_.l, (double) st_.h}
                );
        }
    }

    return events;
}


std::vector<ThermalEvent> GmrHeisenberg::thermalStep()
{
    int id;
    darray newSpin(3);
    double delta;

    std::vector<ThermalEvent> events;
    ThermalEvent event;

    for (int i = 0; i < st_.particleCount(); ++i)
    {
        id = particles_.pop();
        newSpin = perturbedSpin(spins_[id]);
        delta = deltaEnergyThermal_(id, newSpin);

        if (real_(engine_) < std::exp( - delta / et_ -> temperature))
        {
            event.particleId = id;
            event.oldSpin = spins_[id];
            event.newSpin = newSpin;
            event.deltaEnergy = delta;
            events.push_back(event);
            spins_[id] = newSpin;
        }
    }

    return events;
}


std::ostream& operator << (std::ostream& os, GmrHeisenberg& gmr)
{
    for (int i = 0; i < gmr.st_.particleCount(); ++i)
    {
        os << gmr.positions_[i] << " " << gmr.spins_[i] << " "
           << (gmr.species_[i] == Specie::Ion ? "Ion" : "Electron") << " "
           << gmr.ionNbs_[i] << " " << gmr.electronNbs_[i]
           << std::endl;
    }
    return os;
}

