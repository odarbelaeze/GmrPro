#include "GmrHeisenberg.h"

int main(int argc, char const *argv[])
{
    SampleTraits st;
    InteractionTraits it;
    ExternalTraits et;

    st.w = 10;
    st.l = 10;
    st.h = 5;
    st.lt = Lattice::bcc;
    st.nElectrons = 500;
    st.easyAxis = { 0.0, 0.0, 1.0 };

    GmrHeisenberg gmr(st, it, et);

    darray a { 1.0, 2.0, 3.0 };
    darray b { 2.0, 2.0, 2.0 };


    std::cout << std::fmod( - 1.0, 2.0) << std::endl;

    return 0;
}
