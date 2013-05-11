#ifndef ACCUMULATOR_H_
#define ACCUMULATOR_H_

namespace Gmr
{
    struct Accumulator
    {
        double acum;
        double acumsq;
        long   count;

        Accumulator() {
            acum = 0.0;
            acumsq = 0.0;
            count = 0L;
        }

        void operator+= (double x) {
            acum += x;
            acumsq += x * x;
            count++;
        }

        double mean () {
            if (!count) throw std::exception();
            return acum / (double) count;
        }

        double stddev ()
        {
            if (!count) throw std::exception();
            return std::sqrt(
                acumsq / (double) count -
                mean() * mean()
            );
        }

        void reset () {
            acum = 0.0;
            acumsq = 0.0;
            count = 0L;
        }

    };
}


#endif