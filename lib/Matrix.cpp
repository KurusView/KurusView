#include "Matrix.h"

std::ostream &operator<<(std::ostream &os, Matrix &obj) {
    os << obj.mat3x3[0].getX() << "  " << obj.mat3x3[1].getX() << "  " << obj.mat3x3[2].getX() << std::endl;
    os << obj.mat3x3[0].getY() << "  " << obj.mat3x3[1].getY() << "  " << obj.mat3x3[2].getY() << std::endl;
    os << obj.mat3x3[0].getZ() << "  " << obj.mat3x3[1].getZ() << "  " << obj.mat3x3[2].getZ() << std::endl;
    os << std::endl;
    return os;
}


Matrix::Matrix(const MVector &v1, const MVector &v2, const MVector &v3) {

    mat3x3.push_back(v1);
    mat3x3.push_back(v2);
    mat3x3.push_back(v3);

}

Matrix::~Matrix() {}

Matrix Matrix::operator+(const Matrix &obj) const {
    Matrix result;
    for (int i = 0; i < 3; ++i) {
        result.mat3x3[i].setX(mat3x3[i].getX() + obj.mat3x3[i].getX());
        result.mat3x3[i].setY(mat3x3[i].getY() + obj.mat3x3[i].getY());
        result.mat3x3[i].setZ(mat3x3[i].getZ() + obj.mat3x3[i].getZ());
    }
    return result;
}

Matrix Matrix::operator-(const Matrix &obj) const {
    Matrix result;
    for (int i = 0; i < 3; ++i) {
        result.mat3x3[i].setX(mat3x3[i].getX() - obj.mat3x3[i].getX());
        result.mat3x3[i].setY(mat3x3[i].getY() - obj.mat3x3[i].getY());
        result.mat3x3[i].setZ(mat3x3[i].getZ() - obj.mat3x3[i].getZ());
    }
    return result;
}

Matrix Matrix::operator*(const double &scalar) const {
    Matrix result;
    for (int i = 0; i < 3; ++i) {
        result.mat3x3[i].setX(mat3x3[i].getX() * scalar);
        result.mat3x3[i].setY(mat3x3[i].getY() * scalar);
        result.mat3x3[i].setZ(mat3x3[i].getZ() * scalar);
    }
    return result;
}

Matrix Matrix::operator*(const Matrix &obj) const {
    Matrix result;
    double x1, x2, x3, y1, y2, y3, z1, z2, z3;

    x1 = mat3x3[0].getX() * obj.mat3x3[0].getX() + mat3x3[1].getX() * obj.mat3x3[0].getY() +
         mat3x3[2].getX() * obj.mat3x3[0].getZ();
    x2 = mat3x3[0].getX() * obj.mat3x3[1].getX() + mat3x3[1].getX() * obj.mat3x3[1].getY() +
         mat3x3[2].getX() * obj.mat3x3[1].getZ();
    x3 = mat3x3[0].getX() * obj.mat3x3[2].getX() + mat3x3[1].getX() * obj.mat3x3[2].getY() +
         mat3x3[2].getX() * obj.mat3x3[2].getZ();

    result.mat3x3[0].setX(x1);
    result.mat3x3[1].setX(x2);
    result.mat3x3[2].setX(x3);

    y1 = mat3x3[0].getY() * obj.mat3x3[0].getX() + mat3x3[1].getY() * obj.mat3x3[0].getY() +
         mat3x3[2].getY() * obj.mat3x3[0].getZ();
    y2 = mat3x3[0].getY() * obj.mat3x3[1].getX() + mat3x3[1].getY() * obj.mat3x3[1].getY() +
         mat3x3[2].getY() * obj.mat3x3[1].getZ();
    y3 = mat3x3[0].getY() * obj.mat3x3[2].getX() + mat3x3[1].getY() * obj.mat3x3[2].getY() +
         mat3x3[2].getY() * obj.mat3x3[2].getZ();

    result.mat3x3[0].setY(y1);
    result.mat3x3[1].setY(y2);
    result.mat3x3[2].setY(y3);

    z1 = mat3x3[0].getZ() * obj.mat3x3[0].getX() + mat3x3[1].getZ() * obj.mat3x3[0].getY() +
         mat3x3[2].getZ() * obj.mat3x3[0].getZ();
    z2 = mat3x3[0].getZ() * obj.mat3x3[1].getX() + mat3x3[1].getZ() * obj.mat3x3[1].getY() +
         mat3x3[2].getZ() * obj.mat3x3[1].getZ();
    z3 = mat3x3[0].getZ() * obj.mat3x3[2].getX() + mat3x3[1].getZ() * obj.mat3x3[2].getY() +
         mat3x3[2].getZ() * obj.mat3x3[2].getZ();

    result.mat3x3[0].setZ(z1);
    result.mat3x3[1].setZ(z2);
    result.mat3x3[2].setZ(z3);

    return result;
}

MVector Matrix::operator*(const MVector &obj) const {
    MVector result;
    double x = mat3x3[0].getX() * obj.getX() + mat3x3[1].getX() * obj.getY() + mat3x3[2].getX() * obj.getZ();
    double y = mat3x3[0].getY() * obj.getX() + mat3x3[1].getY() * obj.getY() + mat3x3[2].getY() * obj.getZ();
    double z = mat3x3[0].getZ() * obj.getX() + mat3x3[1].getZ() * obj.getY() + mat3x3[2].getZ() * obj.getZ();
    result.setX(x);
    result.setY(y);
    result.setZ(z);
    return result;
}

//double Matrix::det() const {
//    //TODO
//    return;
//}

Matrix Matrix::transponse() const {
    Matrix result;
    result.mat3x3[0].setX(mat3x3[0].getX());
    result.mat3x3[0].setY(mat3x3[1].getX());
    result.mat3x3[0].setZ(mat3x3[2].getX());
    result.mat3x3[1].setX(mat3x3[0].getY());
    result.mat3x3[1].setY(mat3x3[1].getY());
    result.mat3x3[1].setZ(mat3x3[2].getY());
    result.mat3x3[2].setX(mat3x3[0].getZ());
    result.mat3x3[2].setY(mat3x3[1].getZ());
    result.mat3x3[2].setZ(mat3x3[2].getZ());
    return result;
}

double Matrix::det() const {
    double det;
    det = mat3x3[0].getX() * (mat3x3[1].getY() * mat3x3[2].getZ() - mat3x3[2].getY() * mat3x3[1].getZ()) -
          mat3x3[1].getX() * (mat3x3[0].getY() * mat3x3[2].getZ() - mat3x3[2].getY() * mat3x3[0].getZ()) +
          mat3x3[2].getX() * (mat3x3[0].getY() * mat3x3[1].getZ() - mat3x3[1].getY() * mat3x3[0].getZ());
    return det;
}