#ifndef HELPERS_H_
#define HELPERS_H_

#include <valarray>
#include <cmath>
#include <iostream>

typedef std::valarray<double> darray;
typedef std::valarray<darray> ddarray;

inline double norm(darray da)
{
    return std::pow(da, 2.0).sum();
}

inline double distance(darray da, darray db)
{
    return norm(db - da);
}

std::ostream& operator << (std::ostream& os, darray da)
{
    for (auto&& i : da)
    {
        os << i << " ";
    }
    return os;
}

#endif