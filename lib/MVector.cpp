#include "MVector.h"

std::ostream &operator<<(std::ostream &os, MVector &obj) {
    os << "x: " << obj.x << "  y: " << obj.y << "  z: " << obj.z << std::endl;
    return os;
}

MVector::MVector(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

double MVector::getX() const {
    return x;
}

double MVector::getY() const {
    return y;
}

double MVector::getZ() const {
    return z;
}

void MVector::setX(float x) {
    this->x = x;
}

void MVector::setY(float y) {
    this->y = z;
}

void MVector::setZ(float z) {
    this->z = z;
}

double MVector::getModulus() const {
    double mod = sqrt((x * x) + (y * y) + (z * z));
    return mod;
}

MVector MVector::operator+(MVector const &obj) const {
    MVector result;
    result.x = x + obj.x;
    result.y = y + obj.y;
    result.z = z + obj.z;
    return result;
}

MVector MVector::operator-(MVector const &obj) const {
    MVector result;
    result.x = x - obj.x;
    result.y = y - obj.y;
    result.z = z - obj.z;
    return result;
}

MVector::~MVector() {}

MVector MVector::operator*(MVector const &obj) const {
    MVector result;
    result.x = y * obj.z - z * obj.y;
    result.y = z * obj.x - x * obj.z;
    result.z = x * obj.y - y * obj.x;
    return result;
}
