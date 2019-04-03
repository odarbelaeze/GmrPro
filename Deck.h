#ifndef DECK_H_
#define DECK_H_

#include <cstdlib>
#include <exception>

// se crea una clase Deck que puede ser de cualquier tipo, con el fin de obtener un objeto aleatorio
template<typename T>
class Deck
{
public:
    Deck();
    ~Deck();

    void push(const T&);
    T pop();
    T sample();

    size_t size();
    bool isEmpty();

private:
    size_t allocated_;
    size_t N_;
    T* data_;

};

// se inicializa el Deck con cero elementos (N_ = 0) 
// allocated_ sera una potencia de dos que indica el tamano del Deck
// data_ es un vector donde se almacena lo que se quiere (T)
template<typename T>
Deck<T>::Deck()
{
    allocated_ = 1;
    N_ = 0;
    data_ = new T[allocated_];
}

template<typename T>
Deck<T>::~Deck()
{
    delete[] data_;
}

// la funcion push es para agregar un elemento al Deck
template<typename T>
void Deck<T>::push(const T& item)
{
    // si la cantidad de elementos es igual a allocated_, duplicamos el
    // allocated_ y creamos un nuevo data con este nuevo allocated_ y copiamos
    // alli lo que habia antes de duplicarse el allocated_
    if (N_ == allocated_)
    {
        allocated_ *= 2;
        T* copy = data_;
        data_ = new T[allocated_];
        for (int i = 0; i < N_; ++i)
            data_[i] = copy[i];
    }

    // si la cantidad de elementos N_ es diferente a allocated_, en la
    // posicion N_ almacenamos la direccion del elemento que agregamos y
    // aumentamos a N_ en uno
    data_[N_++] = item;
}

// funcion que devuelve un elemento aleatorio del Deck
template<typename T>
T Deck<T>::pop()
{
    // si el Deck no tiene elementos (N_ = 0), devuelve una excepcion
    if (isEmpty()) throw std::exception();

    // si la cantidad de elementos es allocated_ / 4, allocated disminuye a la
    // mitad y se vuelve a copiar todo a data_
    if (N_ == allocated_ / 4)
    {
        allocated_ /= 2;
        T* copy = data_;
        data_ = new T[allocated_];
        for (int i = 0; i < N_; ++i)
            data_[i] = copy[i];
    }

    // se obtiene un elemento aleatorio y dicho elemento se remplaza por el
    // utlimo, asi se quita dicho elemento. Ademas, N_ diminuye en 1
    int id = rand() % N_;
    T item = data_[id];
    data_[id] = data_[--N_];
    return item;
}

// esta funcion es para devolver un elemento aleatorio pero no eliminarlo como
// lo hace pop()
template<typename T>
T Deck<T>::sample()
{
    if (isEmpty()) throw std::exception();
    return data_[rand() % N_];
}

// devuelve la cantidad de elementos que contiene el Deck
template<typename T>
size_t Deck<T>::size()
{
    return N_;
}

// si el numero N_ es cero quiere decir que el Deck esta vacio
template<typename T>
bool Deck<T>::isEmpty()
{
    return N_ == 0;
}


#endif