#ifndef DYNAMIC_STATS_H_
#define DYNAMIC_STATS_H_

#include <map>

#include "Accumulator.h"
#include "Helpers.h"
#include "Particle.h"

namespace Gmr
{

    enum  class Stat { tau, nu, distance, dx, dy, dz };

    class DynamicStats
    {

    public:

        DynamicStats();
        ~DynamicStats();

        void record ();
        void record (const darray&, const darray&);

        double mean (Stat);
        double stddev (Stat);

    private:

        Accumulator tau_;
        Accumulator nu_;
        Accumulator distance_;
        Accumulator dx_;
        Accumulator dy_;
        Accumulator dz_;

    };

    typedef std::map<Particle*, DynamicStats> stats_map;

}

#endif