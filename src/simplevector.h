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
    Vector(double);
    Vector(double, double, double);
    ~Vector();

    const double getX();
    const double getY();
    const double getZ();
    
    void setX(double);
    void setY(double);
    void setZ(double);

    void pacmanEffect(const Vector&);

    double& operator() (int i);
    double& operator[] (int i);

    Vector operator* (const Vector&);
    Vector operator+ (const Vector&);
    Vector operator- (const Vector&);
    Vector operator* (double);
    Vector operator/ (double);

    bool operator== (const Vector&);
    bool operator!= (const Vector&);

    friend double norm(const Vector&);
    friend Vector abs(const Vector&);

    friend Vector operator* (double, const Vector&);
    friend Vector operator- (const Vector&);
    friend double dot(const Vector&, const Vector&);
    friend double distance(const Vector&, const Vector&);
    friend Vector diferencePbc(const Vector&, const Vector&, const Vector&, const Vector&);
    friend double distancePbc(const Vector&, const Vector&, const Vector&, const Vector&);
    friend Vector min(const Vector&, const Vector&);
    friend Vector vfmod(const Vector&, const Vector&);
    friend std::ostream& operator<< (std::ostream&, const Vector&);

protected:

private:
    double x_;
    double y_;
    double z_;
};

Vector randomVector();
Vector randomVectorInBox();

#endif
