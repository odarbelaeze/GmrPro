#include "Accumulator.h"

Accumulator::Accumulator()
{
    data_ = 0.0;
    datasq_ = 0.0;
    counter_ = 0UL;
}


Accumulator::~Accumulator()
{
    /*
        Nothing to do here.
    */
}


void Accumulator::accum(double x)
{
    data_ += x;
    datasq_ += x * x;
    counter_ ++;
}


double Accumulator::mean() const
{
    /*
        If counter_ is equal to zero, it means there is no data added to the
        accumulator and then it is an error to show something here.
    */

    if (counter_ == 0UL) throw NoDataException();

    return data_ / counter_;
}


double Accumulator::meansq() const
{
    /*
        If counter_ is equal to zero, it means there is no data added to the
        accumulator and then it is an error to show something here.
    */

    if (counter_ == 0UL) throw NoDataException();

    return datasq_ / counter_;
}


double Accumulator::var() const
{
    return meansq() - mean() * mean();
}


double Accumulator::stddev() const
{
    return std::sqrt(mean());
}
