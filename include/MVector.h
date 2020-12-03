#ifndef INC_3DLIB_MVECTOR_H
#define INC_3DLIB_MVECTOR_H

#include <iostream>
#include <cmath>

class MVector {
    friend std::ostream &operator<<(std::ostream &os, MVector &obj);

public:
    // default constructor  Vector ve; XXX double check if the : lingo is to be done here or in definition
    MVector() : x(0), y(0), z(0) {};

    MVector(double x, double y, double z);

    double getX() const;

    double getY() const;

    double getZ() const;

    void setX(float x);

    void setY(float y);

    void setZ(float z);

    ~MVector();

    MVector operator+(MVector const &obj) const;

    MVector operator-(MVector const &obj) const;

    MVector operator*(MVector const &obj) const;

    double getModulus() const;

private:
    double x;
    double y;
    double z;
};

#endif //INC_3DLIB_MVECTOR_H
