#include "Accumulator.h"

namespace Gmr
{
    Accumulator::Accumulator() 
    {
        acum = 0.0;
        acumsq = 0.0;
        count = 0L;
        stg = 0.0;
    }

    Accumulator::~Accumulator() {}

    void Accumulator::accumulate ()
    {
        accumulate(stg);
        stg = 0.0;
    }

    void Accumulator::accumulate (double x) 
    {
        acum += x;
        acumsq += x * x;
        count++;
    }

    void Accumulator::stage (double x)
    {
        stg += x;
    }

    void Accumulator::operator += (double x)
    {
        accumulate(x);
    }

    double Accumulator::mean () 
    {
        if (!count) throw std::exception();
        return acum / (double) count;
    }

    double Accumulator::stddev ()
    {
        if (!count) throw std::exception();
        return std::sqrt(
            acumsq / (double) count -
            mean() * mean()
        );
    }

    void Accumulator::reset () 
    {
        acum = 0.0;
        acumsq = 0.0;
        count = 0L;
        stg = 0.0;
    }
}
