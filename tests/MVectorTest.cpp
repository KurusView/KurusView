#include "gtest/gtest.h"
#include "MVector.h"

TEST(MVectorTest, addition) {
    MVector a(1, 4, 7), b(2, 5, 8);
    a = a + b;
    MVector c(3, 9, 15);
    ASSERT_TRUE(a == c);
}

TEST(MVectorTest, subtraction) {
    MVector a(1, 4, 7), b(2, 5, 8);
    a = a - b;
    MVector c(-1, -1, -1);
    ASSERT_TRUE(a == c);
}

TEST(MVectorTest, vectorProduct) {
    MVector a(1, 4, 7), b(2, 5, 8);
    a = a * b;
    MVector c(-3, 6, -3);
    ASSERT_TRUE(a == c);
}

TEST(MVectorTest, scalarProduct) {
    MVector a(1, 4, 7);
    a = a * 2;
    MVector c(2, 8, 14);
    ASSERT_TRUE(a == c);
}

TEST(MVectorTest, scalarDivision) {
    MVector b(2, 5, 8);
    b = b / 2;
    MVector c(1, 2.5, 4);
    ASSERT_TRUE(b == c);
}

TEST(MVectorTest, modulus) {
    MVector a(2, 0, 0);
    int mod = (int) a.getModulus();
    std::cout << a.getModulus() << std::endl;
    ASSERT_TRUE(mod == 2);
}

TEST(MVectorTest, dotProduct) {
    MVector a(1, 4, 7), b(2, 5, 8);
    int result = (int) dotProduct(a, b);
    ASSERT_TRUE(result == 78);
}

TEST(MVectorTest, scalarTripleProduct) {
    MVector a(1, 4, 7), b(2, 5, 8), c(3, 6, 9);
    int result = (int) scalarTripleProduct(a, b, c);
    ASSERT_TRUE(result == 0);
}

TEST(MVectorTest, compOperator) {
    MVector a(1, 4, 7), b(1, 4, 7);
    ASSERT_TRUE(a == b);
}

TEST(MVectorTest, notCompOperator) {
    MVector a(1, 4, 7), b(2, 4, 7);
    ASSERT_TRUE(a != b);
}
