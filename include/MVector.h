/**
 * @file MCell.h
 * @author github.com/georgekhanachat
 * @brief Header file for MVector class
 *
 * This object encapsulates a vector with coordinates in 3D and an ID
 */

#ifndef INC_3DLIB_MVECTOR_H
#define INC_3DLIB_MVECTOR_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

class Matrix;

class MVector {
    /**
     * @brief classic friend overloaded ostream operator<< declaration - prints MVector properties to stdout in human
     *        friendly format
     * @param os - lhs element [ a.operator<<(b) ], might be std::cout or see return
     * @param obj - MVector instance
     * @return os reference for cascading <<
     */
    friend std::ostream &operator<<(std::ostream &os, MVector &obj);

    /**
     * @brief classic friend overloaded ofstream operator<< declaration - prints MVector properties to file in Model
     *        Loader parser format
     * @param os - lhs element [ a.operator<<(b) ], might be std::cout or see return
     * @param obj - MVector instance
     * @return os reference for cascading <<
     */
    friend std::ofstream &operator<<(std::ofstream &os, MVector &obj);

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

    // Multiplication with scalar
    MVector operator*(const double &scalar) const;

    // Multiplication with scalar
    MVector operator/(const double &scalar) const;

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

double dotProduct(MVector v1, MVector v2);

#endif //INC_3DLIB_MVECTOR_H
