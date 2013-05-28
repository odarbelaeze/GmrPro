#include "Helpers.h"

namespace Gmr
{
    double distance (const darray& a, const darray& b) {
        return std::sqrt(std::pow(b - a, 2.0).sum());
    }

    double dot_product (const darray& a, const darray& b) {
        return (a * b).sum();
    }

    darray rand3d (std::mt19937_64& engine, 
                   std::uniform_real_distribution<>& distribution) {

        double t = 1.0 * M_PI * distribution(engine);
        double p = 2.0 * M_PI * distribution(engine);

        return darray({
            std::sin(t) * std::cos(p),
            std::sin(t) * std::sin(p),
            std::cos(t)
        });

    }

}