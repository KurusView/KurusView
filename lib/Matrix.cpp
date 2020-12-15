#include "Matrix.h"

std::ostream &operator<<(std::ostream &os, Matrix &obj) {
    //Overloads << operator
    // Produces an outstream to print the values of the matrix simultaneously in a matrix formation
    os << obj.mat3x3[0].getX() << "  " << obj.mat3x3[1].getX() << "  " << obj.mat3x3[2].getX() << std::endl;
    os << obj.mat3x3[0].getY() << "  " << obj.mat3x3[1].getY() << "  " << obj.mat3x3[2].getY() << std::endl;
    os << obj.mat3x3[0].getZ() << "  " << obj.mat3x3[1].getZ() << "  " << obj.mat3x3[2].getZ() << std::endl;
    os << std::endl;
    return os;
}


Matrix::Matrix(const MVector &v1, const MVector &v2, const MVector &v3) {
    // Initializes matrix using 3 prepopulated vectors
    mat3x3.push_back(v1);
    mat3x3.push_back(v2);
    mat3x3.push_back(v3);

}


Matrix::Matrix(double v1x, double v1y, double v1z, double v2x, double v2y, double v2z, double v3x, double v3y,
               double v3z) {
    // Initializes matrix using 3 prepopulated vectors
    MVector v1(v1x, v1y, v1z);
    MVector v2(v2x, v2y, v2z);
    MVector v3(v3x, v3y, v3z);
    mat3x3.push_back(v1);
    mat3x3.push_back(v2);
    mat3x3.push_back(v3);
}

// Default Constructor
Matrix::~Matrix() = default;

Matrix Matrix::operator+(const Matrix &obj) const {
    // Matrix addition by looping through each vector in the matrix

    /*
     *     | 1   2   3|   | 1   2   3|   | a   b   c|
     *     | 4   5   6| + | 4   5   6| = | d   e   f|
     *     | 7   8   9|   | 7   8   9|   | g   h   i|
     *
     *     For Example:
     *     a = 1 + 1 = 2
     *     b = 2 + 2 = 4
     */
    Matrix result;
    // Loops through every vector
    for (int i = 0; i < 3; ++i) {
        result.mat3x3[i].setX(mat3x3[i].getX() + obj.mat3x3[i].getX());
        result.mat3x3[i].setY(mat3x3[i].getY() + obj.mat3x3[i].getY());
        result.mat3x3[i].setZ(mat3x3[i].getZ() + obj.mat3x3[i].getZ());
    }
    return result;
}

Matrix Matrix::operator-(const Matrix &obj) const {
    // Matrix subtraction by looping through each vector in the matrix

    /*
     *     | 1   2   3|   | 1   2   3|   | a   b   c|
     *     | 4   5   6| - | 4   5   6| = | d   e   f|
     *     | 7   8   9|   | 7   8   9|   | g   h   i|
     *
     *     For Example:
     *     a = 1 - 1 = 0
     *     b = 2 - 2 = 0
     */

    Matrix result;
    // Loops through every vector
    for (int i = 0; i < 3; ++i) {
        result.mat3x3[i].setX(mat3x3[i].getX() - obj.mat3x3[i].getX());
        result.mat3x3[i].setY(mat3x3[i].getY() - obj.mat3x3[i].getY());
        result.mat3x3[i].setZ(mat3x3[i].getZ() - obj.mat3x3[i].getZ());
    }
    return result;
}

Matrix Matrix::operator*(const double &scalar) const {
    // Matrix multiplication with a scalar
    // by looping through each vector in the matrix
    // and scaling it

    /*
     *     | 1   2   3|       | a   b   c|
     *     | 4   5   6| * 2 = | d   e   f|
     *     | 7   8   9|       | g   h   i|
     *
     *     For Example:
     *     a = 2 * 1 = 2
     */

    Matrix result;
    // Loops through every vector
    for (int i = 0; i < 3; ++i) {
        result.mat3x3[i].setX(mat3x3[i].getX() * scalar);
        result.mat3x3[i].setY(mat3x3[i].getY() * scalar);
        result.mat3x3[i].setZ(mat3x3[i].getZ() * scalar);
    }
    return result;
}

Matrix Matrix::operator*(const Matrix &obj) const {
    // Matrix multiplication with another matrix
    // by looping through each vector in both matrices simultaneously
    Matrix result;
    double x1, x2, x3, y1, y2, y3, z1, z2, z3;

    // This section has 3 parts to calculate the 3 components of each vector by
    // multiplying each row but its respective column in the second matrix

    /*
     *     | 1   2   3|   | 1   2   3|   | a   b   c|
     *     | 4   5   6| * | 4   5   6| = | d   e   f|
     *     | 7   8   9|   | 7   8   9|   | g   h   i|
     *
     *     For Example:
     *                        |1|
     *     a = | 1   2   3| * |4| = 1*1 + 2*4 + 3*7 = 30
     *                        |7|
     */

    x1 = mat3x3[0].getX() * obj.mat3x3[0].getX() + mat3x3[1].getX() * obj.mat3x3[0].getY() +
         mat3x3[2].getX() * obj.mat3x3[0].getZ();
    x2 = mat3x3[0].getX() * obj.mat3x3[1].getX() + mat3x3[1].getX() * obj.mat3x3[1].getY() +
         mat3x3[2].getX() * obj.mat3x3[1].getZ();
    x3 = mat3x3[0].getX() * obj.mat3x3[2].getX() + mat3x3[1].getX() * obj.mat3x3[2].getY() +
         mat3x3[2].getX() * obj.mat3x3[2].getZ();

    // Every set is then set using the setter in the MVector.h imported file
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
    // Matrix multiplication with a vector
    // by looping through each vector in the matrix

    /*
     *     | 1   2   3|   | 1 |   | a |
     *     | 4   5   6| * | 4 | = | b |
     *     | 7   8   9|   | 7 |   | c |
     *
     *     For Example:
     *                        |1|
     *     a = | 1   2   3| * |4| = 1*1 + 2*4 + 3*7 = 30
     *                        |7|
     */

    MVector result;
    double x = mat3x3[0].getX() * obj.getX() + mat3x3[1].getX() * obj.getY() + mat3x3[2].getX() * obj.getZ();
    double y = mat3x3[0].getY() * obj.getX() + mat3x3[1].getY() * obj.getY() + mat3x3[2].getY() * obj.getZ();
    double z = mat3x3[0].getZ() * obj.getX() + mat3x3[1].getZ() * obj.getY() + mat3x3[2].getZ() * obj.getZ();
    result.setX(x);
    result.setY(y);
    result.setZ(z);
    return result;
}

void Matrix::Transponse() {
    // Transposing a matrix by reflecting the matrix of a diagonal

    /*
     *
     *     | 1   2   3| T   | 1   4   7|
     *     | 4   5   6|   = | 6   5   8|
     *     | 7   8   9|     | 3   2   9|
     *
     */

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
    this->mat3x3 = result.mat3x3;
}

double Matrix::det() const {
    // Finding the determinant of a matrix to do various
    // calculations like finding the inverse of the matrix

    /*
     *
     *         | 1   2   3|         | 5  6 |       | 4  6 |       | 4  5 |
     *     det | 4   5   6|   = 1 * | 8  9 | + 2 * | 7  9 | + 3 * | 7  8 | = 1(9*5-6*8) + 2(4*9-7*6)+ 3(4*8-5*7) = 0
     *         | 7   8   9|
     *
     */

    double det;
    det = mat3x3[0].getX() * (mat3x3[1].getY() * mat3x3[2].getZ() - mat3x3[2].getY() * mat3x3[1].getZ()) -
          mat3x3[1].getX() * (mat3x3[0].getY() * mat3x3[2].getZ() - mat3x3[2].getY() * mat3x3[0].getZ()) +
          mat3x3[2].getX() * (mat3x3[0].getY() * mat3x3[1].getZ() - mat3x3[1].getY() * mat3x3[0].getZ());
    return det;
}

void Matrix::inverse() {
    // Finding the inverse of a matrix through producing a cofactor matrix, transposing it
    // and multiplying it by the inverse of the determinant of the input matrix

    /*  To find the inverse you need to find the transposed cofactor matrix
     *
     *     | a   b   c| T   | a   h   g|
     *     | d   e   f|   = | f   e   d|
     *     | g   h   i|     | c   b   i|
     *
     *     Then to get the cofactor matrix, the determinant of each element needs to be found
     *     The determinant is found if the lines that the specific element being looked at are erased
     *     with only being left with a square of 4 elements.
     *
     *     For example:
     *         | e  d |
     *     a = | b  i | = ei - db
     *
     *        | a   h   g|   |           |
     *        | f   e   d| = |    e    d |
     *        | c   b   i|   |    b    i |
     *
     */
    Matrix cofactors, result, mat;

    mat.mat3x3 = this->mat3x3;

    // Checks whether the determinant is zero because matrices with a det = 0 dont have an inverse

    cofactors.mat3x3[0].setX((mat3x3[1].getY() * mat3x3[2].getZ()) - (mat3x3[2].getY() * mat3x3[1].getZ()));
    cofactors.mat3x3[0].setY(-((mat3x3[1].getX() * mat3x3[2].getZ()) - (mat3x3[2].getX() * mat3x3[1].getZ())));
    cofactors.mat3x3[0].setZ((mat3x3[1].getX() * mat3x3[2].getY()) - (mat3x3[2].getX() * mat3x3[1].getY()));

    cofactors.mat3x3[1].setX(-((mat3x3[0].getY() * mat3x3[2].getZ()) - (mat3x3[2].getY() * mat3x3[0].getZ())));
    cofactors.mat3x3[1].setY((mat3x3[0].getX() * mat3x3[2].getZ()) - (mat3x3[2].getX() * mat3x3[0].getZ()));
    cofactors.mat3x3[1].setZ(-((mat3x3[0].getX() * mat3x3[2].getY()) - (mat3x3[2].getX() * mat3x3[0].getY())));

    cofactors.mat3x3[2].setX((mat3x3[0].getY() * mat3x3[1].getZ()) - (mat3x3[1].getY() * mat3x3[0].getZ()));
    cofactors.mat3x3[2].setY(-((mat3x3[0].getX() * mat3x3[1].getZ()) - (mat3x3[1].getX() * mat3x3[0].getZ())));
    cofactors.mat3x3[2].setZ((mat3x3[0].getX() * mat3x3[1].getY()) - (mat3x3[1].getX() * mat3x3[0].getY()));

    // Calculation of the inverse
    cofactors.Transponse();
    result = cofactors * (1 / mat.det());
    this->mat3x3 = result.mat3x3;
}

void Matrix::setMat(int col, MVector vector) {
    this->mat3x3[col] = vector;
}

Matrix RotationMatrix(const std::vector<MVector> &Rotation) {

    // Undergoes multiple rotations that needs to be done on a specific matrix or vector
    // Has an input of an array of MVectors where within the vector lays the
    // angle in radians for the specific axis rotation whilst all others are 0
    /*
     *  For Example:
     *  |3.14|
     *  | 0  | = 3.14 rads rotation in the x direction
     *  | 0  |
     */

    int size = Rotation.size();
    MVector vx1(1, 0, 0), vx2, vx3, vy1, vy2(0, 1, 0), vy3, vz1, vz2, vz3(0, 0, 1);
    Matrix rx, ry, rz;

    // Stores the order at which the rotations occur at
    std::vector<Matrix> orderOfRotation;
    Matrix product;

    // Initializing the matrix with the formulas that correspond to each one
    // Loops through all the MVectors in the vector array
    // Undergoes the calculations in the four elements of
    // the matrix that need to be calculated
    // Source for the explanation is:
    // https://mathworld.wolfram.com/RotationMatrix.html under formulas 4, 5 and 6
    for (int i = 0; i < size; ++i) {
        if (Rotation[i].getX() != 0) {
            vx2.setY(cos(Rotation[i].getX()));
            vx2.setZ(sin(Rotation[i].getX()));
            vx3.setY(-sin(Rotation[i].getX()));
            vx3.setZ(cos(Rotation[i].getX()));
            rx.setMat(0, vx1);
            rx.setMat(1, vx2);
            rx.setMat(2, vx3);
            orderOfRotation.push_back(rx);
        }
        if (Rotation[i].getY() != 0) {
            vy1.setX(cos(Rotation[i].getY()));
            vy1.setZ(-sin(Rotation[i].getY()));
            vy3.setX(sin(Rotation[i].getY()));
            vy3.setZ(cos(Rotation[i].getY()));
            ry.setMat(0, vy1);
            ry.setMat(1, vy2);
            ry.setMat(2, vy3);
            orderOfRotation.push_back(ry);
        }
        if (Rotation[i].getZ() != 0) {
            vz1.setX(cos(Rotation[i].getZ()));
            vz1.setY(sin(Rotation[i].getZ()));
            vz2.setX(-sin(Rotation[i].getZ()));
            vz2.setY(cos(Rotation[i].getZ()));
            rz.setMat(0, vz1);
            rz.setMat(1, vz2);
            rz.setMat(2, vz3);
            orderOfRotation.push_back(rz);
        }
    }
    Matrix result;
    result = orderOfRotation[orderOfRotation.size() - 1];
    for (int i = orderOfRotation.size() - 2; i >= 0; --i) {
        result = result * orderOfRotation[i];
    }

    return result;
}


