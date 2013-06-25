#include "Helpers.h"

namespace Gmr
{
    // distancia entre dos darrays
    double distance (const darray& a, const darray& b) {
        return std::sqrt(std::pow(b - a, 2.0).sum());
    }

    // producto interno, producto punto o producto escalar entre dos darrays
    // A·B= A[a1, a2, ..., an]·B[b1, b2, ..., bn] = a1*b1 + a2*b2 + ... + an*bn
    double dot_product (const darray& a, const darray& b) {
        return (a * b).sum();
    }

    // funcion para devolver la norma de un darray en un espacio euclídeo de n
    // dimensiones
    double norm (const darray& a)
    {
        return std::sqrt(std::pow(a, 2.0).sum());
    }

    // funcion que devuelve la norma-p de un darray. Si p=2 se tiene la norma
    // euclidea
    double normp (const darray& a, double p)
    {
        return std::pow(std::pow(a, p).sum(), 1.0 / p);
    }

    // funcion para devolver un std::valarray<double> aleatorio, en si es un
    // vector de norma 1 con direccion aleatoria
    darray rand3d (std::mt19937_64& engine, 
                   std::uniform_real_distribution<>& distribution) {

        // uniform_real_distribution = Produces random floating-point values
        // i, uniformly distributed on the interval [a, b), that is,
        // distributed according to the probability function: 
        // P(i|a,b) = 1 / (b - a)

        // pregunto si se puede crear std::uniform_real_distribution<>& distribution(0, M_PI)
        // y luego no mas decir Cte * distribution(engine)
        // devolveria un numero aleatorio entre [0, Cte * M_PI)
        double t = 1.0 * M_PI * distribution(engine);
        double p = 2.0 * M_PI * distribution(engine);

        // un darray se inicializa como darray({a1, a2, ..., an})
        // se devuelve un vector con componentes:
        // [sin(t) * cos(p), sin(t) * sin(p), cos(t)]
        // la norma de esto es 1
        return darray({
            std::sin(t) * std::cos(p),
            std::sin(t) * std::sin(p),
            std::cos(t)
        });

    }

    double distance (const darray& a, const darray& b, 
                     const darray& dims,
                     const std::initializer_list<double> pbc)
    {
        return norm(min(std::abs(b - a), std::abs(darray(pbc) * dims - (b - a))));
    }

    double distance (const darray& a, const darray& b, 
                     const std::vector<int>& dims, 
                     const std::initializer_list<double> pbc)
    {
        std::valarray<int> dims_int(dims.data(), dims.size());
        std::valarray<double> dims_double(dims.size());
        for (int i = 0; i < dims.size(); ++i)
            dims_double[i] = dims_int[i];
        return distance(a, b, dims_double, pbc);
    }

    double distance (const darray& a, const darray& b, 
                     const std::initializer_list<int>& dims, 
                     const std::initializer_list<double> pbc)
    {
        std::valarray<int> dims_int(dims);
        std::valarray<double> dims_double(dims.size());
        for (int i = 0; i < dims.size(); ++i)
            dims_double[i] = dims_int[i];
        return distance(a, b, dims_double, pbc);
    }

    darray min(const darray& A, const darray& B)
    {
        if (A.size() != B.size()) throw std::exception();

        darray min(A.size());
        for (int i = 0; i < A.size(); i++)
            min[i] = (A[i] < B[i])? A[i] : B[i];

        return min;
    }

    darray max(const darray& A, const darray& B)
    {
        if (A.size() != B.size()) throw std::exception();

        darray max(A.size());
        for (int i = 0; i < A.size(); i++)
            max[i] = (A[i] > B[i])? A[i] : B[i];

        return max;
    }

}