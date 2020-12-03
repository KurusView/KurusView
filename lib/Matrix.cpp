#include "Matrix.h"


Matrix::Matrix(MVector &v1, MVector &v2, MVector &v3) {
    mat3x3[0].push_back(v1.getX());
    mat3x3[0].push_back(v2.getX());
    mat3x3[0].push_back(v3.getX());

    mat3x3[1].push_back(v1.getY());
    mat3x3[1].push_back(v2.getY());
    mat3x3[1].push_back(v3.getY());

    mat3x3[2].push_back(v1.getZ());
    mat3x3[2].push_back(v2.getZ());
    mat3x3[2].push_back(v3.getZ());
}

Matrix::~Matrix() {}

Matrix Matrix::operator+(const Matrix &obj) const {
    Matrix result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.mat3x3[i][j] = this->mat3x3[i][j] + obj.mat3x3[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix &obj) const {
    Matrix result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.mat3x3[i][j] = this->mat3x3[i][j] - obj.mat3x3[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const double &scalar) const {
    Matrix result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.mat3x3[i][j] = scalar * mat3x3[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix &obj) const {
    Matrix result;
    double temp = 0.0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            temp = 0.0;
            for (int k = 0; k < 3; ++k) {
                temp += mat3x3[i][k] * obj.mat3x3[k][j];
            }
            result.mat3x3[i][j] = temp;
        }
    }
    return result;
}

MVector Matrix::operator*(const MVector &obj) const {
    MVector result;
    double x = 0, y = 0, z = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == 0) x += mat3x3[i][j] * obj.getX();
            if (i == 1) y += mat3x3[i][j] * obj.getY();
            if (i == 2) z += mat3x3[i][j] * obj.getZ();
        }
    }
    result.setX(x);
    result.setY(y);
    result.setZ(z);
    return result;
}

double Matrix::det() const {
    double det = 0.0;
    det = (mat3x3[0][0] * mat3x3[1][1] * mat3x3[2][2] + mat3x3[0][1] * mat3x3[1][2] * mat3x3[2][0] +
           mat3x3[0][2] * mat3x3[1][0] * mat3x3[2][1]) -
          (mat3x3[0][0] * mat3x3[1][2] * mat3x3[2][1] + mat3x3[0][1] * mat3x3[1][0] * mat3x3[2][2] +
           mat3x3[0][2] * mat3x3[1][1] * mat3x3[2][0]);
    return det;
}

Matrix Matrix::transponse(const Matrix &obj) const {
    Matrix result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.mat3x3[i][j] = this->mat3x3[j][i];
        }
    }
    return result;
}



