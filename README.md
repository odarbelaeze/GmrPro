GmrPro
======

A C++ package suitable for [Giant magneto-resistance][1] (GMR) simulations, 
featuring custom Hamiltonian contributions and data reporting through a full 
[JsonCpp][2] integration.

The GmrPro package allows to perform Monte Carlo simulations in particle
systems in which the particles are given with spin, position and charge
it allows spin and position updates so virtually allows to perform any 
simulation between a simple thermalization process and Monte Carlo, molecular
dynamics.

Introduction:
-------------

[Giant magneto-resistance][1], i.e., the influence of an external magnetic field
in the resistivity of a material, was first discovered by  **Albert Fert** and 
**Peter Grünberg**. This phenomena is of great interest in electronics and 
storage device development.

The purpose of this package is to simulate this phenomena using the Monte Carlo
schema, at first using the well known Metropolis algorithm, and then using the
quite a bit more powerful Kinetic Monte Carlo schema (KMC).

The GmrPro library archives the flexibility needed to perform this kind of
simulation providing Monte Carlo Thermal and Dynamic steps, in which the spin
and the position are updated respectively, and triggers a signal connected to a
specific (implementable) callback after each event (accepted update).

Hamiltonian Contributions:
--------------------------

This package features a dynamic Hamiltonian contribution system, in which the
contributions can be added dynamically without any major change in the 
application's core functionality. Aiming this, the Hamiltonian is divided in it's
Particle-Field and Particle-Particle interaction contributions, the total energy
contribution of each particle is calculated internally adding this two terms, 
by default the Particle-Field contribution returns 0, and the particle particle
interaction returns a special potential that keeps the particles separated in
accordance to the muffin-tin radius, despiting the fact that the Particle-Field
interaction returns zero, is a good practice adding the returning value of both of
this default methods tho the custom implemented methods.

The two main contributions used in the `GmrSystem` subclass are the **Heisenberg 
Hamiltonian contribution** which takes into account the spin-spin interaction and
the **Electric potential Hamiltonian** which takes into account the electron-field 
interaction due to charge.

Customizing the System Class:
-----------------------------

The main class of the program is the system class, that features a public API, that
can be seen in the header files and a set of virtual methods. The way of use this
package for your own purposes is to make a subclass of system, a example of that is
the `GmrSystem` class, and overriding the virtual methods as necessary.

For example a simple subclass to solve the Heisenberg model could be,

```
class HeisenbergSystem : public System
{
    public:
    protected:
        float computeInteractionContribution_(int i) 
        {
            std::vector<int> neighbors;
            float sum = 0.0;
            sum += System::computeInteractionContribution(i);
            for (int ii = 0; ii < neighbors.size(); ii++)
            {
                int j = neighbors[ii];
                sum -= J * dot(particles_[i].getSpin(), particles_[j].getSpin());
            }
            return sum;
        }

        void onThermalEventCb_(Particle& particle, float energyDelta)
        {
            std::cout << time_ << "    " << energy_ << "    " << energyDelta 
                      << std::endl;
        }
    private:
        float J;
}
```

and that's it.


Building the package:
---------------------

This package requires the [JsonCpp][2] library (`lib-jsoncpp-dev` if you are using 
ubuntu or similar) and the [uBLAS][3] boost library (you can get it installig 
`lib-boost-all-dev` in ubuntu or similar.

In this package a makefile is provided, intending to make ;) your life easier,
try `make`, to build the entire library and GmrPro executable in release mode, it is
with all your favorite optimization flags, and saving the time of debugging flags, 
`make release` and `make debug` will build just the library en release or debug mode,
(debug uses `gdb` and `gprof` integration)  finally, `make maindebug` will build the 
entire library and executable in debug mode.


  [1]: http://en.wikipedia.org/wiki/Giant_magnetoresistance
  [2]: http://jsoncpp.sourceforge.net/
  [3]: http://www.boost.org/doc/libs/1_53_0/libs/numeric/ublas/doc/index.htm