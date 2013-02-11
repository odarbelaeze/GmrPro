GmrPro
======

A C++ package for [Giant magneto-resistance][1] (GMR) simulations, featuring 
custom hamiltonian contributions and data reporting throug a full [JsonCpp][2] 
integration.

Introduction:
-------------

[Giant magneto-resistance][1], i.e., the influence of an external magnetic field
in the resistivity of a material, was first discovered by  **Albert Fert** and 
**Peter Grünberg**. This phenomena is of great interest in electronics and 
storage device development.

The purpose of this package is to simulate this phenomena using the Monte Carlo
schema, at first using the well known Metropolis algorithm, and then using the
quite a bit more powerful Kinetic Monte Carlo schema (KMC).

Hamiltonian Contributions:
--------------------------

This package features a dynamic Hamiltonian contribution system, in wich the
contributions can be added dynamically without any major change in the 
application's core functionality.

The two main contributions used here are the **Heisenberg Hamiltonian 
contribution** which takes into account the spin-spin interaction and the 
**Electric potential Hamiltonian** wich takes into acount the electron-field 
interaction due to charge.

Data reporting and analysis:
----------------------------

This package also features custom data reporting, trough a callback function
triggered when an event occurs in the system.

In terms of analysis, full correlation analysis is strongly recomended.

Building the package:
---------------------

This package requires the [JsonCpp][2] library (`lib-jsoncpp-dev` if you are using 
ubuntu or similar) and the [uBLAS][3] boost library (you can get it installig 
`lib-boost-all-dev` in ubuntu or similar.

Use, `make` or `make debug` to build the main excecutable and libraries with
full debug integration using both **gprof** and **gdb**.
`make release` to build the main excecutable and libraries with optimization
flags.


  [1]: http://en.wikipedia.org/wiki/Giant_magnetoresistance
  [2]: http://jsoncpp.sourceforge.net/
  [3]: http://www.boost.org/doc/libs/1_53_0/libs/numeric/ublas/doc/index.htm