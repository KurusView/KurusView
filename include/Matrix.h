#ifndef INC_3DLIB_MATRIX_H
#define INC_3DLIB_MATRIX_H

#include "MVector.h"
#include <vector>

class Matrix {
public:
    Matrix(MVector &v1, MVector &v2, MVector &v3);

    Matrix() : mat3x3(0) {};

    Matrix operator+(Matrix const &obj) const;

    Matrix operator-(Matrix const &obj) const;

    Matrix operator*(Matrix const &obj) const;

    MVector operator*(MVector const &obj) const;

    Matrix operator*(double const &scalar) const;

    Matrix transponse(Matrix const &obj) const;

    double det() const;

    ~Matrix();

private:
    std::vector<std::vector<double>> mat3x3;
};


#endif //INC_3DLIB_MATRIX_H
