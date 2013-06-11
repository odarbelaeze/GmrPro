#ifndef DYNAMIC_STATS_H_
#define DYNAMIC_STATS_H_

#include <map>

#include "Accumulator.h"
#include "Helpers.h"
#include "Particle.h"

namespace Gmr
{

    enum  class Stat { tau, nu, distance, dx, dy, dz };

    struct Stage
    {
        double tau_;
        double nu_;
        double distance_;
        double dx_;
        double dy_;
        double dz_;

        Stage();
        void   stage (Stat, double);
        double clear (Stat);

    };

    typedef std::map<Particle*, Stage> stage_map;

    class DynamicStats
    {

    public:

        DynamicStats ();
        ~DynamicStats ();

        void record (Particle*);
        void record (Particle*, const darray&, const darray&);

        double mean (Stat);
        double stddev (Stat);

    private:

        stage_map   stages_;
        Accumulator tau_;
        Accumulator nu_;
        Accumulator distance_;
        Accumulator dx_;
        Accumulator dy_;
        Accumulator dz_;

    };


}

#endif