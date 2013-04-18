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

#endif