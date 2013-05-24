#ifndef HELPERS_H_
#define HELPERS_H_

#include <cmath>
#include <iostream>
#include <valarray>

namespace Gmr
{
    typedef std::valarray<double> darray;

    double distance (const darray& a, const darray& b) {
        return std::sqrt(std::pow(b - a, 2.0).sum());
    }

    template <typename T>
    darray fmod(const darray& da, std::initializer_list<T> dim_list)
    {
        if (da.size() != dim_list.size())
            throw std::exception();

        darray answer(da.size());
        for (int i = 0; i < dim_list.size(); ++i)
        {
            answer[i] = std::fmod(da[i], (double)(*(dim_list.begin() + i)));
            if (answer[i] < 0) 
                answer[i] = answer[i] + (double)(*(dim_list.begin() + i));
        }
        
        return answer;
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& os, std::valarray<T> vr)
    {
        for (auto&& i : vr)
            os << i << " ";

        return os;
    }

}

#endif