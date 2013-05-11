#ifndef DECK_H_
#define DECK_H_

#include <cstdlib>
#include <exception>

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

template<typename T>
void Deck<T>::push(const T& item)
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
T Deck<T>::pop()
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
T Deck<T>::sample()
{
    if (isEmpty()) throw std::exception();
    return data_[rand() % N_];
}

template<typename T>
size_t Deck<T>::size()
{
    return N_;
}

template<typename T>
bool Deck<T>::isEmpty()
{
    return N_ == 0;
}


#endif