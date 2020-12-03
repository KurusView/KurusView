#include "MVector.h"
#include "Matrix.h"
#include <iostream>

int main(int argc, char *argv[]) {
    MVector a(1, 2, 3);
    MVector b(1, 2, 3);
    MVector c(1, 2, 3);
    MVector z;
    Matrix x(a, b, c);
    z = a + b + c;
    std::cout << z;
    return 0;
}
