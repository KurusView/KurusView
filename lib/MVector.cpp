#include "MVector.h"

std::ostream &operator<<(std::ostream &os, MVector &obj) {
    //Overloads << operator
    // Produces an outstream to print the values of x, y and z of a vector simultaneously
    os << "x: " << obj.x << "  y: " << obj.y << "  z: " << obj.z << std::endl;
    os << std::endl;
    return os;
}

MVector::MVector(double x, double y, double z) {
    // Initializes a vector a specific input coordinates
    this->x = x;
    this->y = y;
    this->z = z;
}

double MVector::getX() const {
    // Returns the value of x for a specific vector
    return x;
}

double MVector::getY() const {
    // Returns the value of y for a specific vector
    return y;
}

double MVector::getZ() const {
    // Returns the value of z for a specific vector
    return z;
}

void MVector::setX(float x) {
    // Sets the value of x for a specific vector
    this->x = x;
}

void MVector::setY(float y) {
    // Sets the value of y for a specific vector
    this->y = y;
}

void MVector::setZ(float z) {
    // Sets the value of z for a specific vector
    this->z = z;
}

double MVector::getModulus() const {
    // Calculates the modulus of a vector
    // The squareroot of the sum of the squares
    double mod = sqrt((x * x) + (y * y) + (z * z));
    return mod;
}

MVector MVector::operator+(const MVector &obj) const {
    // Overloads the + operator to perform vector addition.
    MVector result;
    result.x = x + obj.x;
    result.y = y + obj.y;
    result.z = z + obj.z;
    return result;
}

MVector MVector::operator-(const MVector &obj) const {
    // Overloads the - operator to perform vector subtraction
    MVector result;
    result.x = x - obj.x;
    result.y = y - obj.y;
    result.z = z - obj.z;
    return result;
}

MVector::~MVector() {}

MVector MVector::operator*(const MVector &obj) const {
    // Overloads the * operator to perform the vector product
    // of two vectors
    MVector result;
    result.x = (y * obj.getZ()) - (z * obj.getY());
    result.y = (z * obj.getX()) - (x * obj.getZ());
    result.z = (x * obj.getY()) - (y * obj.getX());
    return result;
}


