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
    // default constructor  Vector ve
    MVector();

    // NOTE ID represented vectors are used for display, read from a file and written to a file. Anonymous vectors are
    // used internally (for example in the cell class to return Centre or Gravity)

    //Constructor to initialize the vector in 3D
    MVector(double x, double y, double z);

    //Constructor to initialize the vector in 3D with ID
    MVector(double x, double y, double z, long int id);

    // default destructor
    ~MVector() = default;

    // Getter functions for x, y and z
    double getX() const;

    double getY() const;

    double getZ() const;

    //Setter functions for x, y and z
    void setX(double x);

    void setY(double y);

    void setZ(double z);


    // TODO: Would we ever transform an ID represented vector? If so, what do we do with the ID. - See comment on ctors
    // At the moment transformed vectors are anonymous - but maybe they should maintain in ID of the LHS element.

    // Vector Addition
    MVector operator+(const MVector &obj) const;

    // Vector subtraction
    MVector operator-(const MVector &obj) const;

    // Vector/Cross/Outer Product
    MVector operator*(const MVector &obj) const;

    // Find modulus of a specific vector to calculate scalar/dot product
    double getModulus() const;

    long int getID() const;

private:
    //3-Dimensional data of a vector
    double x;
    double y;
    double z;

    // MVector ID
    long int MVectorID;
};

#endif //INC_3DLIB_MVECTOR_H
