#ifndef INC_3DLIB_MATRIX_H
#define INC_3DLIB_MATRIX_H

#include "MVector.h"
#include <iostream>
#include <vector>

class Matrix {
    friend std::ostream &operator<<(std::ostream &os, Matrix &obj);

public:
    Matrix(const MVector &v1, const MVector &v2, const MVector &v3);

    Matrix() : mat3x3(3, MVector()) {};

    Matrix operator+(Matrix const &obj) const;

    Matrix operator-(Matrix const &obj) const;

    Matrix operator*(Matrix const &obj) const;

    MVector operator*(MVector const &obj) const;

    Matrix operator*(double const &scalar) const;

    Matrix transponse() const;

    double det() const;

    Matrix inverse() const;

    ~Matrix();

private:
    std::vector<MVector> mat3x3;
};


#endif //INC_3DLIB_MATRIX_H
