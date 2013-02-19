#ifndef GMR_SYSTEM_H_
#define GMR_SYSTEM_H_

#include "GmrPro.h"

struct InteractionTraits
{
    float J;
    float I_0;
    float K_0;
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
    virtual float   computeFieldContribution_(int);
    virtual float   computeInteractionContribution_(int);
    virtual void    onThermalEventCb_(Particle&, float);
    virtual void    onDynamicEventCb_(Particle&, float);

private:
    InteractionTraits   interactionTraits_;
    Vector              electricField_;

};

#endif