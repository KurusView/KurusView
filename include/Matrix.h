/**
 * @file Matrix.h
 * @author github.com/georgekhanachat
 * @brief Header file for Matrix class
 *
 * This object encapsulates a 3x3 matrix
 */

#ifndef INC_3DLIB_MATRIX_H
#define INC_3DLIB_MATRIX_H

#include "MVector.h"
#include <iostream>
#include <vector>

class Matrix {
    /**
     * @brief classic friend overloaded ostream operator<< declaration - prints Matrix properties to stdout in human
     *        friendly format
     * @param os - lhs element [ a.operator<<(b) ], might be std::cout or see return
     * @param obj - Matrix instance
     * @return - os reference for cascading <<
     */
    friend std::ostream &operator<<(std::ostream &os, Matrix &obj);

public:
    /**
     * @brief constructor - Initializes MVectors in the matrix
     */
    Matrix(const MVector &v1, const MVector &v2, const MVector &v3);

    /**
     * @brief constructor - Initializes coordinates in the matrix
     */
    Matrix(double v1x, double v1y, double v1z, double v2x, double v2y, double v2z, double v3x, double v3y, double v3z);

    /**
     * @brief default constructor - Initializes matrix to zero with MVector constructor
     */
    Matrix() : mat3x3(3, MVector()) {};

    /**
     * @brief operator+ - overloads the + operator to add two matrices directly together
     * @param obj - Matrix instance to add to the called object Matrix
     * @return - Matrix that is a result of two added matrices
     */
    Matrix operator+(Matrix const &obj) const;

    /**
     * @brief operator- - overloads the - operator to subtract two matrices directly together
     * @param obj - Matrix instance to subtract to the called object Matrix
     * @return - Matrix that is a result of two subtracted matrices
     */
    Matrix operator-(Matrix const &obj) const;

    /**
     * @brief operator* - overloads the * operator to multiply a matrix by a matrix
     * @param obj - Matrix instance
     * @return Matrix - result of matrix multiplied by a Matrix
     */
    Matrix operator*(const Matrix &obj) const;

    /**
     * @brief operator* - overloads the * operator to multiply a matrix by a vector
     * @param obj - MVector instance
     * @return MVector - result of matrix multiplied by a vector
     */
    MVector operator*(const MVector &obj) const;

    /**
     * @brief operator* - overloads the * operator to multiply a matrix by a scalar
     * @param scalar - a double value to multiply an Matrix with
     * @return - Matrix multiplied by a scalar
     */
    Matrix operator*(double const &scalar) const;

    /**
     * @brief operator/ - overloads the / operator to divide a matrix by a scalar
     * @param scalar - a double value to divide an Matrix with
     * @return - Matrix divided by a scalar
     */
    Matrix operator/(double const &scalar) const;

    /**
     * @brief transpose - Reflects matrix across the diagonal
     */
    void transpose();

    /**
     * @brief determinant - finds the determinant of the matrix
     * @return - Determinant of the matrix
     */
    double determinant() const;

    /**
     * @brief inverse - Finds inverse of a matrix
     */
    void inverse();

    /**
     * @brief setMat - Sets a specific column in the 3x3 matrix (initializes one column to an MVector
     */
    void setMat(int col, MVector vector);

    /// @brief default destructor
    ~Matrix() = default;

private:
    /// @brief Vector of 3D vectors from an imported header file
    std::vector<MVector> mat3x3;

};

/**
     * @brief RotationMatrix - calculates the rotation matrix of a specific set of angles
     * @param Rotation - vector of MVectors specifying the rotation in each axis
     * @return - Rotation Matrix
     */
Matrix RotationMatrix(const std::vector<MVector> &Rotation);

#endif //INC_3DLIB_MATRIX_H
