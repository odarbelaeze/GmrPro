#include "DynamicStats.h"

namespace Gmr
{
    DynamicStats::DynamicStats() {}
    DynamicStats::~DynamicStats() {}

    void DynamicStats::record ()
    {
        tau_.accumulate();
        nu_.stage(1.0);
        distance_.accumulate();
        dx_.accumulate();
        dy_.accumulate();
        dz_.accumulate();
    }

    void DynamicStats::record (const darray& a, const darray& b)
    {
        tau_.stage(1.0);
        nu_.accumulate();
        distance_.stage(distance(a, b));
        dx_.stage(b[0] - a[0]);
        dy_.stage(b[1] - a[1]);
        dz_.stage(b[2] - a[2]);
    }


    double DynamicStats::mean (Stat stat)
    {
        switch (stat)
        {
            case Stat::tau : 
                return tau_.mean();

            case Stat::nu : 
                return nu_.mean();

            case Stat::distance : 
                return distance_.mean();

            case Stat::dx : 
                return dx_.mean();

            case Stat::dy : 
                return dy_.mean();

            case Stat::dz : 
                return dz_.mean();

        }
    }

    double DynamicStats::stddev (Stat stat)
    {
        switch (stat)
        {
            case Stat::tau : 
                return tau_.stddev();

            case Stat::nu : 
                return nu_.stddev();

            case Stat::distance : 
                return distance_.stddev();

            case Stat::dx : 
                return dx_.stddev();

            case Stat::dy : 
                return dy_.stddev();

            case Stat::dz : 
                return dz_.stddev();

        }
    }

}
