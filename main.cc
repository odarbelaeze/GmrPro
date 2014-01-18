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
    st.nElectrons = 0;
    st.lt = Lattice::sc;
    st.easyAxis = { 0.0, 0.0, 1.0 };

    it.ee = 0.5;
    it.ei = 0.5;
    it.ii = 1.0;
    it.pauli = 0.1;
    it.eeCutOff = 2.0;
    it.eiCutOff = 1.0;
    it.iiCutOff = 1.0;

    et.electricField = { 1.0, 0.0, 0.0 };
    et.magneticField = { 0.0, 0.0, 0.0 };
    et.temperature = 15.0;

    GmrHeisenberg gmr(st, it, &et);

    while (et.temperature > 0.0)
    {
        Accumulator energy;
        Accumulator magnetization;

        for (int i = 0; i < 1000; ++i)
        {
            gmr.thermalStep();
            energy.accum(gmr.energy());
            magnetization.accum(norm(gmr.magnetization()));
        }

        std::cout << et.temperature << " "
                  << magnetization.mean() << " "
                  << energy.mean() << " "
                  << std::endl;

        et.temperature -= 0.1;
    }


    return 0;
}
