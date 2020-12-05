#include <MVector.h>
#include <MCell.h>
#include <Material.h>
#include <Model.h>
#include <Matrix.h>
#include <ModelRenderer.h>
#include <iostream>

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
    model.saveModel("../../models/saves/testmodel.txt");

    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    std::cout << "End Model Test" << std::endl;
}

void matrixTest() {
    std::cout << "\n\nBegin Matrix Test" << std::endl;
    std::cout << "---------------------------------------------------------------------------------------" << std::endl;
    MVector a(1, 4, 7);
    MVector b(2, 123, 8);
    MVector c(3, 6, 9);
    Matrix m1(a, b, c);
    Matrix m2(a, b, c);
    MVector timesVec;
    Matrix addition;
    Matrix timesMat;
    Matrix subtract;
    Matrix times;
    Matrix transpose;
    double det;

    addition = m1 + m2;
    subtract = m1 - m2;
    times = m1 * 2.53421;
    timesVec = m1 * a;

    timesMat = m1 * m2;

    transpose = m1.transponse();
    det = m1.det();
    std::cout << addition << subtract << times << timesVec << timesMat;
    std::cout << transpose;
    std::cout << det << std::endl;

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
