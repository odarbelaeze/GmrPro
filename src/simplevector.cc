#include "simplevector.h"


const char* OutOfBoundsException::what() const throw()
{
    std::string error = "Requested element are out of bounds.";
    return error.data();
}


Vector::Vector() : x_(0.0), y_(0.0), z_(0.0)
{

}



Vector::Vector(double value) : x_(value), y_(value), z_(value)
{

}



Vector::Vector(double x, double y, double z) : x_(x), y_(y), z_(z)
{

}



Vector::~Vector()
{

}



const double Vector::getX()
{
    return x_;
}



const double Vector::getY()
{
    return y_;
}



const double Vector::getZ()
{
    return z_;
}



void Vector::setX(double x)
{
    x_ = x;
}



void Vector::setY(double y)
{
    y_ = y;
}



void Vector::setZ(double z)
{
    z_ = z;
}



void Vector::pacmanEffect(const Vector& other)
{
    *this = vfmod(*this, other);
    setX((x_ < 0)? other.x_ + x_ : x_);
    setY((y_ < 0)? other.y_ + y_ : y_);
    setZ((z_ < 0)? other.z_ + z_ : z_);
}



double& Vector::operator() (int i)
{
    switch (i)
    {
        case 0 :
            return x_;
            break;
        case 1 :
            return y_;
            break;
        case 2 :
            return z_;
            break;
        default :
            throw OutOfBoundsException();
    }
}



double& Vector::operator[] (int i)
{
    switch (i)
    {
        case 0 :
            return x_;
            break;
        case 1 :
            return y_;
            break;
        case 2 :
            return z_;
            break;
        default :
            throw OutOfBoundsException();
    }
}



Vector Vector::operator* (const Vector& other)
{
    Vector answer;
    answer.x_ = this -> x_ * other.x_;
    answer.y_ = this -> y_ * other.y_;
    answer.z_ = this -> z_ * other.z_;
    return answer;
}



Vector Vector::operator+ (const Vector& other)
{
    Vector answer;
    answer.x_ = this -> x_ + other.x_;
    answer.y_ = this -> y_ + other.y_;
    answer.z_ = this -> z_ + other.z_;
    return answer;
}



Vector Vector::operator- (const Vector& other)
{
    Vector answer;
    answer.x_ = this -> x_ - other.x_;
    answer.y_ = this -> y_ - other.y_;
    answer.z_ = this -> z_ - other.z_;
    return answer;
}



Vector Vector::operator* (double scalar)
{
    Vector answer;
    answer.x_ = this -> x_ * scalar;
    answer.y_ = this -> y_ * scalar;
    answer.z_ = this -> z_ * scalar;
    return answer;
}



Vector Vector::operator/ (double scalar)
{
    Vector answer;
    answer.x_ = this -> x_ / scalar;
    answer.y_ = this -> y_ / scalar;
    answer.z_ = this -> z_ / scalar;
    return answer;
}



bool Vector::operator== (const Vector& other)
{
    return this -> x_ == other.x_ &&
           this -> y_ == other.y_ &&
           this -> z_ == other.z_ ;
}



bool Vector::operator!= (const Vector& other)
{
    return this -> x_ != other.x_ ||
           this -> y_ != other.y_ ||
           this -> z_ != other.z_ ;
}



double norm(const Vector& vector)
{
    return std::sqrt(dot(vector, vector));
}



Vector abs(const Vector& vector)
{
    Vector answer;
    answer.x_ = (vector.x_ < 0.0)? - vector.x_ : vector.x_;
    answer.y_ = (vector.y_ < 0.0)? - vector.y_ : vector.y_;
    answer.z_ = (vector.z_ < 0.0)? - vector.z_ : vector.z_;
    return answer;
}



Vector operator* (double scalar, const Vector& vector)
{
    return const_cast<Vector&>(vector) * scalar;
}



Vector operator- (const Vector& vector)
{
    return - 1.0 * const_cast<Vector&>(vector);
}



double dot(const Vector& vectorA, const Vector& vectorB)
{
    return vectorA.x_ * vectorB.x_ +
           vectorA.y_ * vectorB.y_ +
           vectorA.z_ * vectorB.z_ ;
}



double distance(const Vector& vectorA, const Vector& vectorB)
{
    return norm(const_cast<Vector&>(vectorB) - const_cast<Vector&>(vectorA));
}



double distancePbc(const Vector& vectorA, const Vector& vectorB, 
                  const Vector& dimensions, const Vector& pbc)
{
    Vector diference = const_cast<Vector&>(vectorB) - 
                       const_cast<Vector&>(vectorA);
    return norm(min(abs(diference), abs(const_cast<Vector&>(dimensions) * 
                                   const_cast<Vector&>(pbc) - diference)));
}



Vector min(const Vector& vectorA, const Vector& vectorB)
{
    Vector answer;
    answer.x_ = (vectorA.x_ < vectorB.x_)? vectorA.x_ : vectorB.x_; 
    answer.y_ = (vectorA.y_ < vectorB.y_)? vectorA.y_ : vectorB.y_; 
    answer.z_ = (vectorA.z_ < vectorB.z_)? vectorA.z_ : vectorB.z_; 
    return answer;
}



Vector vfmod(const Vector& u, const Vector& v)
{
    return Vector(fmod(u.x_, v.x_), fmod(u.y_, v.y_), fmod(u.z_, v.z_));
}



Vector randomVector()
{
    Vector answer;
    double theta = 2.0f * M_PI * drand48();
    double phi   = 1.0f * M_PI * drand48();

    answer.setX(sin (theta) * cos (phi));
    answer.setY(sin (theta) * sin (phi));
    answer.setZ(cos (theta));

    return answer;
}



Vector randomVectorInBox()
{
    return Vector(drand48(), drand48(), drand48());
}



std::ostream& operator<< (std::ostream& ostream, const Vector& vector)
{
    ostream << "(" << vector.x_ << ", " << vector.y_ << ", "
                   << vector.z_ << ")";
    return ostream;
}
