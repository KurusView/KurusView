#include <MVector.h>
#include <MCell.h>
#include <Material.h>
#include <Model.h>
#include <Matrix.h>
#include <ModelRenderer.h>
#include <iostream>

#define pi 3.141592653589793238462643383279502884197169399375105820974944592307816406286

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

    MVector a(69, 2, 3);
    MVector b(1, 2, 3);
    MVector c(1, 2, 3);
    MVector rot(pi, 0, 0);
    MVector rot2(0, pi, 0);
    MVector rot3(0, 0, pi);

    std::vector<MVector> rotation;
    rotation.push_back(rot);
    rotation.push_back(rot2);
    rotation.push_back(rot3);

    a.rotate(rotation);

    std::cout << a;

    MVector addition, subtraction, times;
    addition = a + b + c;
    subtraction = a - b - c;
    times = a * b;
    std::cout << addition << subtraction << times;

    std::cout << "Mod: " << a.getModulus() << std::endl;

    std::cout << "---------------------------------------------------------------------------------------"
              << std::endl;
    std::cout << "End MVector Test" << std::endl;
}

void mCellTest() {
    std::cout << "\n\nBegin MCell Test" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
//    TEST CODE HERE
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
    MVector a(1, 1, 1);
    MVector b(1, 1, 1);
    MVector c(1, 1, 1);
    MVector rot(pi / 2, 0, 0);
    MVector rot2(pi / 2, 0, 0);
    MVector rot3(pi / 2, 0, 0);
    MVector rot4(pi / 2, 0, 0);
    Matrix m1(a, b, c);
    Matrix m2(a, b, c);
    MVector timesVec;
    Matrix addition;
    Matrix timesMat;
    Matrix subtract;
    Matrix times;
    Matrix transpose;
    Matrix inverse;
    double det;

    addition = m1 + m2;
    subtract = m1 - m2;
    times = m1 * 2.53421;
    timesVec = m1 * a;
    timesMat = m1 * m2;
    transpose = m1.transponse();
    det = m1.det();
    inverse = m1.inverse();

    std::cout << addition << subtract << times << timesVec << timesMat;
    std::cout << transpose << inverse;
    std::cout << det << std::endl;
    std::cout << std::endl;
    std::vector<MVector> rotation;
    rotation.push_back(rot);
    rotation.push_back(rot2);
    rotation.push_back(rot3);
    rotation.push_back(rot4);
    m1.rotate(rotation);
    std::cout << m1;

    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "End Matrix Test" << std::endl;
}

void modelRendererTest() {
    std::cout << "\n\nBegin ModelRenderer Test" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
//    TEST CODE HERE
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "End ModelRenderer Test" << std::endl;
}
