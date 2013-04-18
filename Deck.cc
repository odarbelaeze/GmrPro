#include "Deck.h"

template<typename T>
Deck::Deck()
{
    allocated_ = 1;
    N_ = 0;
    data_ = new T[allocated_];
}

template<typename T>
Deck::~Deck()
{
    delete[] data_;
}

template<typename T>
Deck::push(const T& item)
{
    if (N_ == allocated_)
    {
        allocated_ *= 2;
        T* copy = data_;
        data_ = new T[allocated_];
        for (int i = 0; i < N_; ++i)
            data_[i] = copy[i];
    }
    data_[N_++] = item;
}

template<typename T>
T Deck::pop()
{
    if (isEmpty()) throw std::exception();
    if (N_ == allocated_ / 4)
    {
        allocated_ /= 2;
        T* copy = data_;
        data_ = new T[allocated_];
        for (int i = 0; i < N_; ++i)
            data_[i] = copy[i];
    }

    int id = rand() % N_;
    T item = data_[id];
    data_[id] = data_[--N_];
    return item;
}

template<typename T>
T Deck::sample()
{
    if (isEmpty()) throw std::exception();
    return data_[rand() % N_];
}

template<typename T>
size_t size()
{
    return N_;
}

template<typename T>
bool isEmpty()
{
    return N_ == 0;
}
