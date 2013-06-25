#ifndef HELPERS_H_
#define HELPERS_H_

#include <cmath>
#include <iostream>
#include <random>
#include <valarray>

namespace Gmr
{
    // de ahora en adelante todo lo que sea darray es un std::valarray<double>,
    // un valarray double
    typedef std::valarray<double> darray;

    // distancia entre dos darrays
    double distance (const darray& a, const darray& b);

    double distance (const darray& a, const darray& b, 
                     const darray& dims, 
                     const std::initializer_list<double> pbc = {1, 1, 1});

    double distance (const darray& a, const darray& b, 
                     const std::vector<int> & dims, 
                     const std::initializer_list<double> pbc = {1, 1, 1});

    double distance (const darray& a, const darray& b, 
                     const std::initializer_list<int>& dims, 
                     const std::initializer_list<double> pbc = {1, 1, 1});


    // producto interno, producto punto o producto escalar entre dos darrays
    // A·B= A[a1, a2, ..., an]·B[b1, b2, ..., bn] = a1*b1 + a2*b2 + ... + an*bn
    double dot_product (const darray& a, const darray& b);

    // funcion para devolver la norma de un darray en un espacio euclídeo de n
    // dimensiones
    double norm (const darray& a);

    // funcion que devuelve la norma-p de un darray. Si p=2 se tiene la norma
    // euclidea
    double normp (const darray& a, double p);

    // funcion para devolver un std::valarray<double> aleatorio, en si es un
    // vector de norma 1 con direccion aleatoria
    darray rand3d (std::mt19937_64& engine, 
                   std::uniform_real_distribution<>& distribution);
    
    // la función darray fmod contiene el efecto pacman que impide que el darray
    // da (posicion) este fuera del tamano del sistema
    template <typename T>
    darray fmod(const darray& da, std::initializer_list<T> dim_list)
    {
        // si las dimensiones de da y de la dim_list (que contiene el tamano del
        // sistema) son diferentes se produce una excepcion
        if (da.size() != dim_list.size())
            throw std::exception();

        // se crea un std::valarray<double> answer para almacenar los residuos
        // de las componentes del vector y las componentes del tamano del
        // sistema, haciendoles el debido proceso para volverlas aptas para el
        // sistema
        darray answer(da.size());
        for (int i = 0; i < dim_list.size(); ++i)
        {
            // la funcion std::fmod = Remainder of dividing arguments. The
            // result has the same sign as x.
            answer[i] = std::fmod(da[i], (double)(*(dim_list.begin() + i)));
            if (answer[i] < 0)
                // si dicho residuo es negativo quiere decir que esta fuera del
                // sistema entonces se le suma el tamano del sistema en dicha
                // componente
                answer[i] = answer[i] + (double)(*(dim_list.begin() + i));
        }
        
        return answer;
    }

    // este segmento tiene la misma finalidad que el anterior, solamente que en
    // vez de tener las dimensiones del sistema almacenadas en un
    // initializer_list las tenemos en un vector<>
    template <typename T>
    darray fmod(const darray& da, std::vector<T> dim_list)
    {
        if (da.size() != dim_list.size())
            throw std::exception();

        darray answer(da.size());
        for (int i = 0; i < dim_list.size(); ++i)
        {
            // con (double)(*(dim_list.begin() + i)) se le hace un casting a
            // double. Sin embargo esta linea pudo haber sido
            // (double)(dim_list[i]) y ya -.-
            answer[i] = std::fmod(da[i], (double)(*(dim_list.begin() + i)));
            if (answer[i] < 0) 
                answer[i] = answer[i] + (double)(*(dim_list.begin() + i));
        }
        
        return answer;
    }

    darray min(const darray& A, const darray& B);

    darray max(const darray& A, const darray& B);

    // este segmento es para modificar el ostream y que imprima un valarray de
    // cualquier tipo
    template<typename T>
    std::ostream& operator<< (std::ostream& os, std::valarray<T> vr)
    {
        for (auto&& i : vr)
            os << i << " ";

        return os;
    }

}

#endif