#ifndef SIMPLEVECTOR_H_
#define SIMPLEVECTOR_H_

#include <cmath>
#include <exception>


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
    
    void setX(float x);
    void setX(float x);
    void setX(float x);

    float operator() (int i);
    float operator[] (int i);

    Vector operator* (const Vector&);
    Vector operator+ (const Vector&);
    Vector operator- (const Vector&);
    Vector operator* (float);
    Vector operator/ (float);

    friend float norm(const Vector&);
    friend Vector abs(const Vector&);

    friend operator* (float, const Vector&);
    friend float dot(const Vector&, const Vector&);
    friend float distance(const Vector&, const Vector&);
    friend float distancePbc(const Vector&, const Vector&, const Vector&);
    friend Vector min(const Vector&, const Vector&);
protected:

private:
    float x_;
    float y_;
    float z_;
};

#endif
