#include "Accumulator.h"
#include "GmrHeisenberg.h"

int main(int argc, char const *argv[])
{
    SampleTraits st;
    InteractionTraits it;
    ExternalTraits et;

    st.w = 10;
    st.l = 10;
    st.h = 5;
    st.nElectrons = 500;
    st.lt = Lattice::sc;
    st.easyAxis = { 0.0, 0.0, 1.0 };

    it.ee = 0.5;
    it.ei = 0.5;
    it.ii = 1.0;
    it.eePauli = 0.5;
    it.eeCutOff = 2.0;
    it.eiCutOff = 1.0;
    it.iiCutOff = 1.0;

    et.electricField = { 1.0, 0.0, 0.0 };
    et.magneticField = { 0.0, 0.0, 0.0 };
    et.temperature = 3.0;

    GmrHeisenberg gmr(st, it, &et);

    while (et.temperature > 0.0)
    {
        Accumulator energy;
        Accumulator magnetization;

        for (int i = 0; i < 50; ++i)
        {
            for (int j = 0; j < 10; ++j) gmr.thermalStep();
            for (int j = 0; j < 10; ++j) gmr.dynamicStep();
            gmr.updateNbh();
        }

        for (int i = 0; i < 50; ++i)
        {
            for (int j = 0; j < 10; ++j)
            {
                gmr.thermalStep();
                energy.accum(gmr.energy());
                magnetization.accum(norm(gmr.magnetization()));
            }
            for (int j = 0; j < 10; ++j) gmr.dynamicStep();
            gmr.updateNbh();
        }

        std::cout << et.temperature << " "
                  << magnetization.mean() << " "
                  << energy.mean() << " "
                  << std::endl;

        et.temperature -= 0.03;
    }

    return 0;
}
