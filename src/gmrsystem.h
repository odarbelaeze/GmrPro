#ifndef GMR_SYSTEM_H_
#define GMR_SYSTEM_H_

#include "GmrPro.h"

struct InteractionTraits
{
    double J;
    double I_0;
    double K_0;
};

class GmrSystem : public System
{
public:
    GmrSystem();
    GmrSystem(const Json::Value&);
    GmrSystem(const std::string);
    GmrSystem(const char*);
    GmrSystem(std::istream&);
    ~GmrSystem();

    void    setElectricField(const Vector&);
    Vector& getElectricFiedlRef();

protected:
    virtual double   computeFieldContribution_(int);
    virtual double   computeInteractionContribution_(int);
    virtual void    onThermalEventCb_(Particle&, double);
    virtual void    onDynamicEventCb_(Particle&, double);

private:
    InteractionTraits   interactionTraits_;
    Vector              electricField_;

};

#endif