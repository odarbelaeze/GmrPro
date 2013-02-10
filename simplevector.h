#ifndef SIMPLEVECTOR_H_
#define SIMPLEVECTOR_H_

#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>

class OutOfBoundsException : public std::exception
{
public:
    virtual const char* what() const throw();
};

class Vector
{
public:
    Vector();
    Vector(float);
    Vector(float, float, float);
    ~Vector();

    const float getX();
    const float getY();
    const float getZ();
    
    void setX(float);
    void setY(float);
    void setZ(float);

    float operator() (int i);
    float operator[] (int i);

    Vector operator* (const Vector&);
    Vector operator+ (const Vector&);
    Vector operator- (const Vector&);
    Vector operator* (float);
    Vector operator/ (float);

    bool operator== (const Vector&);
    bool operator!= (const Vector&);

    friend float norm(const Vector&);
    friend Vector abs(const Vector&);

    friend Vector operator* (float, const Vector&);
    friend Vector operator- (const Vector&);
    friend float dot(const Vector&, const Vector&);
    friend float distance(const Vector&, const Vector&);
    friend float distancePbc(const Vector&, const Vector&, const Vector&);
    friend Vector min(const Vector&, const Vector&);
    friend std::ostream& operator<< (std::ostream&, const Vector&);
protected:

private:
    float x_;
    float y_;
    float z_;
};

Vector randomVector();

#endif
