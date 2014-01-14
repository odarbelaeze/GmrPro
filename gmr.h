#ifndef GMR_H_
#define GMR_H_

enum class Lattice { sc, bcc, fcc };

struct SampleTraits
{
    int w;
    int h;
    int l;
    int nElectrons;
    Lattice lt;
    darray easyAxis;
};

struct InteractionTraits
{
    double ee;
    double ei;
    double ii;
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

class Gmr
{
public:
    Gmr(SampleTraits, InteractionTraits, ExternalTraits&);
    ~Gmr();

protected:
    SampleTraits st_;
    InteractionTraits it_;
    ExternalTraits& et_;

static:
    darray randomSpin();
    darray perturbedSpin(const darray&);

};

#endif