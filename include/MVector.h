#ifndef INC_3DLIB_MVECTOR_H
#define INC_3DLIB_MVECTOR_H

#include <iostream>
#include <cmath>
#include <vector>

class Matrix;

class MVector {
    // Out stream to allow for complete print outs of a vector
    friend std::ostream &operator<<(std::ostream &os, MVector &obj);

public:
    // default constructor  Vector ve; XXX double check if the : lingo is to be done here or in definition
    MVector() : x(0), y(0), z(0) {};

    //Constructor to initialize the vector in 3D
    MVector(double x, double y, double z);

    // Getter functions for x, y and z
    double getX() const;

    double getY() const;

    double getZ() const;

    //Setter functions for x, y and z
    void setX(double x);

    void setY(double y);

    void setZ(double z);

    ~MVector();

    // Vector Addition
    MVector operator+(const MVector &obj) const;

    // Vector subtraction
    MVector operator-(const MVector &obj) const;

    // Vector/Cross/Outer Product
    MVector operator*(const MVector &obj) const;

    // Find modulus of a specific vector to calculate scalar/dot product
    double getModulus() const;

private:
    //3-Dimensional data of a vector
    double x;
    double y;
    double z;
};

#endif //INC_3DLIB_MVECTOR_H
