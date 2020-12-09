#include <Matrix.h>
#include <MVector.h>

std::ostream &operator<<(std::ostream &os, MVector &obj) {
    //Overloads << operator
    // Produces an outstream to print the values of x, y and z of a vector simultaneously
    os << "x: " << obj.x << "  y: " << obj.y << "  z: " << obj.z << std::endl;
    os << std::endl;
    return os;
}

MVector::MVector(double x, double y, double z) {
    // Initializes a vector a specific input coordinates
    this->x = x;
    this->y = y;
    this->z = z;
}

double MVector::getX() const {
    // Returns the value of x for a specific vector
    return x;
}

double MVector::getY() const {
    // Returns the value of y for a specific vector
    return y;
}

double MVector::getZ() const {
    // Returns the value of z for a specific vector
    return z;
}

void MVector::setX(double x) {
    // Sets the value of x for a specific vector
    this->x = x;
}

void MVector::setY(double y) {
    // Sets the value of y for a specific vector
    this->y = y;
}

void MVector::setZ(double z) {
    // Sets the value of z for a specific vector
    this->z = z;
}

double MVector::getModulus() const {
    // Calculates the modulus of a vector
    // The square root of the sum of the squares
    double mod = sqrt((x * x) + (y * y) + (z * z));
    return mod;
}

MVector MVector::operator+(const MVector &obj) const {
    // Overloads the + operator to perform vector addition.

    /*
     *     | 1 |   | 1 |   | a |
     *     | 4 | + | 4 | = | b |
     *     | 7 |   | 7 |   | c |
     *
     *     For Example:
     *     a = 1 + 1 = 2
     *     b = 4 + 4 = 8
     */

    MVector result;
    result.x = x + obj.x;
    result.y = y + obj.y;
    result.z = z + obj.z;
    return result;
}

MVector MVector::operator-(const MVector &obj) const {
    // Overloads the - operator to perform vector subtraction

    /*
     *     | 1 |   | 1 |   | a |
     *     | 4 | - | 4 | = | b |
     *     | 7 |   | 7 |   | c |
     *
     *     For Example:
     *     a = 1 - 1 = 0
     *     b = 4 - 4 = 0
     */

    MVector result;
    result.x = x - obj.x;
    result.y = y - obj.y;
    result.z = z - obj.z;
    return result;
}

MVector::~MVector() = default;

MVector MVector::operator*(const MVector &obj) const {
    // Overloads the * operator to perform the vector product
    // of two vectors

    /*
     *     | 1 |   | 2 |   | a |
     *     | 4 | * | 3 | = | b |
     *     | 7 |   | 6 |   | c |
     *
     *     For Example:
     *     a = 4*6 - 7*3 = 3
     *     b = 7*2 - 1*6 = 8
     */

    MVector result;
    result.x = (y * obj.getZ()) - (z * obj.getY());
    result.y = (z * obj.getX()) - (x * obj.getZ());
    result.z = (x * obj.getY()) - (y * obj.getX());
    return result;
}

void MVector::rotate(const std::vector<MVector> &Rotation) {
    MVector result;
    std::vector<Matrix> matrices;
    result.x = this->x;
    result.y = this->y;
    result.z = this->z;
    matrices = Matrix::RotationMatrix(Rotation);
    // Multiplies the rotation matrices within the array of matrices
    // To give a result of the final rotation matrix to be
    // multiplied with the input vector
    for (auto &matrix : matrices) {
        result = matrix * result;
    }
    this->x = result.x;
    this->y = result.y;
    this->z = result.z;
}


