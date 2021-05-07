#define _USE_MATH_DEFINES

#include "gtest/gtest.h"
#include "Matrix.h"
#include <cmath>

MVector a(1, 4, 7), b(2, 5, 8), c(3, 6, 9);
Matrix m1(a, b, c);
Matrix m2(c, b, a);


TEST(MatrixTest, addition) {
    Matrix addition = m1 + m2;
    Matrix result({4, 10, 16}, {4, 10, 16}, {4, 10, 16});
    ASSERT_TRUE(result == addition);
}

TEST(MatrixTest, subtraction) {
    Matrix subtraction = m1 - m2;
    Matrix result({-2, -2, -2}, {0, 0, 0}, {2, 2, 2});
    ASSERT_TRUE(result == subtraction);
}

TEST(MatrixTest, product) {
    Matrix productMat = m1 * m2;
    Matrix result({42, 96, 150}, {36, 81, 126}, {30, 66, 102});
    ASSERT_TRUE(result == productMat);
}

TEST(MatrixTest, scalarProd) {
    Matrix productScalar = m1 * 2;
    Matrix result({2, 8, 14}, {4, 10, 16}, {6, 12, 18});
    ASSERT_TRUE(result == productScalar);
}

TEST(MatrixTest, vectorProd) {
    MVector productVec = m1 * a;
    MVector result(30, 66, 102);
    ASSERT_TRUE(result == productVec);
}

TEST(MatrixTest, det) {
    ASSERT_TRUE(m1.determinant() == 0);
}

TEST(MatrixTest, transpose) {
    Matrix transpose(a, b, c);
    Matrix result({1, 2, 3}, {4, 5, 6}, {7, 8, 9});
    transpose.transpose();
    ASSERT_TRUE(transpose == result);
}

TEST(MatrixTest, inverse) {
    Matrix inverse({1, 1, 0}, {0, 1, 0}, {0, 1, 1});
    Matrix result({1, -1, 0}, {0, 1, 0}, {0, -1, 1});
    inverse.inverse();
    ASSERT_TRUE(inverse == result);
}

TEST(MatrixTest, set) {
    Matrix set({0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    Matrix result1({1, 1, 1}, {0, 0, 0}, {0, 0, 0});
    set.setMat(0, {1, 1, 1});
    ASSERT_TRUE(set == result1);
    Matrix result2({1, 1, 1}, {1, 1, 1}, {0, 0, 0});
    set.setMat(1, {1, 1, 1});
    ASSERT_TRUE(set == result2);
    Matrix result3({1, 1, 1}, {1, 1, 1}, {1, 1, 1});
    set.setMat(2, {1, 1, 1});
    ASSERT_TRUE(set == result3);
}

TEST(MatrixTest, rotationMatrix) {
    // Initializing rotation vectors
    MVector rot(M_PI, 0, 0);
    MVector rot2(0, M_PI, 0);
    MVector rot3(0, 0, M_PI);

    // Populating to input into rotation function
    std::vector<MVector> rotation;
    rotation.push_back(rot);
    rotation.push_back(rot3);
    rotation.push_back(rot2);

    Matrix rotMatrix = RotationMatrix(rotation);
    //Function assumes 0 to be a very small number hence requiring a rounding function for the test to pass
    for (int i = 0; i < 3; ++i) {
        rotMatrix.getMat()[i].setX(round(rotMatrix.getMat()[i][0] * 10000000000.0) / 10000000000.0);
        rotMatrix.getMat()[i].setY(round(rotMatrix.getMat()[i][1] * 10000000000.0) / 10000000000.0);
        rotMatrix.getMat()[i].setZ(round(rotMatrix.getMat()[i][2] * 10000000000.0) / 10000000000.0);

    }
    Matrix result({1, 0, 0}, {0, 1, 0}, {0, 0, 1});
    ASSERT_TRUE(rotMatrix == result);
}

TEST(MatrixTest, scalingMatrix) {
    Matrix scaling = ScalingMatrix(1);
    Matrix result({1, 0, 0}, {0, 1, 0}, {0, 0, 1});
    ASSERT_TRUE(scaling == result);
}

TEST(MatrixTest, compOperator) {
    Matrix matrix1(a, b, c), matrix2(a, b, c);
    ASSERT_TRUE(matrix1 == matrix2);
}

TEST(MatrixTest, notCompOperator) {
    Matrix matrix1(a, b, c), matrix2(c, b, a);
    ASSERT_TRUE(matrix1 != matrix2);
}