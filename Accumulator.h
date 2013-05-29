#ifndef ACCUMULATOR_H_
#define ACCUMULATOR_H_

#include <cmath>
#include <exception>

namespace Gmr
{
    class Accumulator
    {
    
    public:

        Accumulator ();
        ~Accumulator ();

        void accumulate ();
        void accumulate (double);
        void stage (double);
        void operator+= (double);
        void reset();

        double mean ();
        double stddev ();

    private:

        double acum;
        double acumsq;
        long   count;
        double stg;


    };
}


#endif