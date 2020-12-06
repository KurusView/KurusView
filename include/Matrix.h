#ifndef INC_3DLIB_MATRIX_H
#define INC_3DLIB_MATRIX_H

#include "MVector.h"
#include <iostream>
#include <vector>

class Matrix {
    // Allows for an outstream set of data to print a whole matrix out
    friend std::ostream &operator<<(std::ostream &os, Matrix &obj);

public:
    // Initializes a matrix using three 3x1 vectors
    Matrix(const MVector &v1, const MVector &v2, const MVector &v3);

    // Initializing rotation matrix
    Matrix RotationMatrix(const std::vector<MVector> &Rotation);

    void rotate(const std::vector<MVector> &Rotation);

    // Initializes matrix with 3 columns and initializing each cell using the MVector constructor.
    Matrix() : mat3x3(3, MVector()) {};

    // Overloading + operator to do matrix addition
    Matrix operator+(Matrix const &obj) const;

    // Overloading - operator to do matrix subtraction
    Matrix operator-(Matrix const &obj) const;

    // Overloading * operator to do matrix multiplication with matrix
    Matrix operator*(const Matrix &obj) const;

    // Overloading * operator to do matrix multiplication with vector
    MVector operator*(const MVector &obj) const;

    // Overloading * operator to do matrix multiplication with scalar
    Matrix operator*(double const &scalar) const;

    // Transposing a matrix
    Matrix transponse() const;

    // Finding determinant of matrix
    double det() const;

    //Finding the inverse of a matrix
    Matrix inverse() const;

    ~Matrix();

private:
    // Vector of 3D vectors from an imported header file
    std::vector<MVector> mat3x3;
};


#endif //INC_3DLIB_MATRIX_H
