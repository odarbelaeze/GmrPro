#ifndef GMR_PRO_H_
#define GMR_PRO_H_



namespace Gmr
{
    

    

    // std::map<std::string, double> parameters {
    //     { "Jex", 1.0 },
    //     { "I_0", 1.0 },
    //     { "K_0", 2.0 },
    //     { "R_0", 0.001 }
    // };

    

    // double contribution (const Particle& particle){
    //     double contribution = 0;
    //     #ifndef _SPECIE
    //     #define _SPECIE(p, sp) (p).getSpecie() == Specie::sp
    //     #endif

    //     for (auto&& other : particle.getNbh())
    //     {
    //         contribution += exp( 
    //             - distance(particle.getPosition(), other -> getPosition())
    //             / parameters["R_0"]
    //         );

    //         if (_SPECIE(particle, Ion) && _SPECIE(*other, Ion))
    //             contribution -= parameters["Jex"] * 
    //                             (particle.getSpin() * other -> getSpin());

    //         else if ((_SPECIE(particle, Ion) && _SPECIE(*other, Electron))
    //                  || (_SPECIE(particle, Electron) && _SPECIE(*other, Ion)))
    //             contribution -= I(particle, *other) * (particle.getSpin() * other -> getSpin());

    //         else if (_SPECIE(particle, Electron) && _SPECIE(*other, Electron))
    //             contribution -= K(particle, *other) * (particle.getSpin() * other -> getSpin());
    //     }

    //     contribution -= particle.getCharge() * (electricField * particle.getPosition()).sum();

    //     #undef _SPECIE
    //     return contribution;
    // };



}

#endif