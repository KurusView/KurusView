#define _USE_MATH_DEFINES

#include <MVector.h>
#include <MCell.h>
#include <Material.h>
#include <Model.h>
#include <Matrix.h>
#include <ModelRenderer.h>
#include <iostream>
#include <cmath>
#include <memory>

#include "Cells/MHexahedron.h"
#include "Cells/MTetrahedron.h"
#include "Cells/MPyramid.h"

void mVectorTest();

void mCellTest();

void materialTest();

void modelTest();

void matrixTest();

void modelRendererTest();

int main(int argc, char *argv[]) {
    mVectorTest();
    mCellTest();
    materialTest();
    modelTest();
    matrixTest();
    modelRendererTest();
    return 0;
}

void mVectorTest() {
    std::cout << "\n\nBegin MVector Test" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;

    MVector addition, subtraction, vectorProd;

    // Initializing 2 vectors to undergo tests
    MVector a(1, 2, 3);
    MVector b(3, 2, 1);

    std::cout << "Vector 1: " << std::endl;
    std::cout << a;
    std::cout << "Vector 2: " << std::endl;
    std::cout << b;

    // Initializing rotation vectors
    MVector rot(M_PI, 0, 0);
    MVector rot2(0, M_PI / 8, 0);
    MVector rot3(0, 0, M_PI / 2);

    // Populating to input into rotation function
    std::vector<MVector> rotation;
    rotation.push_back(rot);
    rotation.push_back(rot3);
    rotation.push_back(rot2);

    // Testing vector functions
    addition = a + b;
    subtraction = a - b;
    vectorProd = a * b;

    std::cout << "Addition: " << std::endl;
    std::cout << addition;
    std::cout << "Subtraction: " << std::endl;
    std::cout << subtraction;
    std::cout << "Vector Product: " << std::endl;
    std::cout << vectorProd;
    std::cout << "Modulus: " << a.getModulus() << std::endl;
    std::cout << std::endl;

    Matrix Rotation;
    Rotation = RotationMatrix(rotation);

    std::cout << "Rotation Matrix: " << std::endl;
    std::cout << Rotation;

    MVector rotated;

    rotated = Rotation * a;

    std::cout << "Rotated Vector: " << std::endl;
    std::cout << rotated;


    // Test ID functionality

    MVector anonymous;
    MVector idRepresented(0, 0, 0, 10);
    MVector transformed;
    transformed = idRepresented * anonymous;

    std::cout << "Anonymous MVector ID: " << anonymous.getID() << std::endl;
    std::cout << "ID represented MVector ID: " << idRepresented.getID() << std::endl;
    std::cout << "Transformed MVector ID: " << transformed.getID() << std::endl;

    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "End MVector Test" << std::endl;
}

void mCellTest() {
    std::cout << "\n\nBegin MCell Test" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
//    TEST CODE HERE

    // Define a vertex
    std::shared_ptr<MVector> testSharedVector = std::make_shared<MVector>(1, 2, 3, 99);

    // Define a Material
    std::shared_ptr<Material> testSharedMaterial = std::make_shared<Material>("testM",
                                                                              "FFFF",
                                                                              123456,
                                                                              11);

    // create vector of vertexes
    std::vector<std::shared_ptr<MVector>> test_vector = {testSharedVector, testSharedVector, testSharedVector};

    // create some tetrahedrons to increase the instance count
    MTetrahedron tetra(test_vector, testSharedMaterial, 0);
    MTetrahedron tetraX(test_vector, testSharedMaterial, 0);

    {
        // this one gets destructed and should not increase the count
        MTetrahedron tetraY(test_vector, testSharedMaterial, 0); // this one should not be seen on count
    }

    std::cout << "volume: " << tetra.getVolume() << std::endl;
    std::cout << "count: " << MCell::getCount() << std::endl;
    std::cout << "To file: " << tetra << std::endl;

    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "End MCell Test" << std::endl;
}

void materialTest() {
    std::cout << "\n\nBegin Material Test" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
//    TEST CODE HERE
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "End Material Test" << std::endl;
}

void modelTest() {
    std::cout << "\n\nBegin Model Test" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;

    Model model("../../models/testmodel.txt");
    model.saveModel("savetestmodel.txt");
    model.loadModel("savetestmodel.txt");

    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "End Model Test" << std::endl;
}

void matrixTest() {
    std::cout << "\n\nBegin Matrix Test" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;

    // Initializing two matrices to undergo multiple tests
    MVector a(1, 4, 7), b(2, 5, 8), c(3, 6, 9);
    Matrix m1(a, b, c);
    Matrix m2(c, b, a);
    Matrix inverse(a, c, c);
    Matrix transpose(a, b, c);

    std::cout << "Matrix 1: " << std::endl;
    std::cout << m1;
    std::cout << "Matrix 2: " << std::endl;
    std::cout << m2;
    std::cout << "Initial inverse matrix: " << std::endl;
    std::cout << inverse;

    std::cout << "Initial transpose matrix: " << std::endl;
    std::cout << inverse;

    // Initializing matrix variables
    Matrix subtraction, productMat, addition, productScalar;
    MVector productVec;

    //Testing matrix functions
    addition = m1 + m2;
    subtraction = m1 - m2;
    productMat = m1 * m2;
    productScalar = m1 * M_PI;
    productVec = m1 * a;

    std::cout << "Addition: " << std::endl;
    std::cout << addition;
    std::cout << "Subtraction: " << std::endl;
    std::cout << subtraction;
    std::cout << "Matrix multiplication: " << std::endl;
    std::cout << productMat;
    std::cout << "Matrix-Vector multiplication: " << std::endl;
    std::cout << productVec;
    std::cout << "Matrix-Scalar multiplication: " << std::endl;
    std::cout << productScalar;
    std::cout << "Matrix-Vector multiplication: " << std::endl;
    std::cout << productVec;
    std::cout << "Determinant: " << m1.det() << std::endl;
    std::cout << std::endl;

    // Initializing rotation vectors
    MVector rot(M_PI, 0, 0);
    MVector rot2(0, M_PI / 8, 0);
    MVector rot3(0, 0, M_PI / 2);

    // Populating to input into rotation function
    std::vector<MVector> rotation;
    rotation.push_back(rot);
    rotation.push_back(rot3);
    rotation.push_back(rot2);

    Matrix rotMatrix = RotationMatrix(rotation);

    std::cout << "Rotation Matrix: " << std::endl;
    std::cout << rotMatrix;

    inverse.inverse();

    std::cout << "Inverse of Matrix: " << std::endl;
    std::cout << inverse;

    transpose.Transponse();

    std::cout << "Transpose of Matrix: " << std::endl;
    std::cout << transpose;

    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "End Matrix Test" << std::endl;
}

void modelRendererTest() {
    std::cout << "\n\nBegin ModelRenderer Test" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    Model model("../../models/tetrahedron.txt");
    ModelRenderer modelRenderer(&model);
    modelRenderer.run();
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "End ModelRenderer Test" << std::endl;
}
