#include <MVector.h>

std::ostream &operator<<(std::ostream &os, MVector &obj) {
    //Overloads << operator
    // Produces an outstream to print the values of x, y and z of a vector simultaneously
    os << "x: " << obj.x << "  y: " << obj.y << "  z: " << obj.z << std::endl;
    os << std::endl;
    return os;
}

std::ofstream &operator<<(std::ofstream &os, MVector &obj) {
    os << "v " << obj.getID()
       << " " << obj.getX()
       << " " << obj.getY()
       << " " << obj.getZ() << std::endl;

    return os;
}

MVector::MVector() : x(0), y(0), z(0), MVectorID(-1) {}

MVector::MVector(double x, double y, double z) : MVectorID(-1) {
    // Initializes a vector a specific input coordinates
    this->x = x;
    this->y = y;
    this->z = z;
}

MVector::MVector(double x, double y, double z, const long id) : MVectorID(id) {
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

long int MVector::getID() const {
    // Returns value of the MVector ID
    return this->MVectorID;
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

MVector MVector::operator/(const double &scalar) const {
    // Overloads the / operator to perform division between
    // two vectors

    /*
     *     | 1 |       | a |
     *     | 4 | / 3 = | b |
     *     | 7 |       | c |
     *
     *     For Example:
     *     a = 1/3
     *     b = 4/3
     */

    MVector result;
    result.setX(x / scalar);
    result.setY(y / scalar);
    result.setZ(z / scalar);
    return result;
}

MVector MVector::operator*(const double &scalar) const {
    // Overloads the / operator to perform division between
    // two vectors

    /*
     *     | 1 |       | a |
     *     | 4 | * 3 = | b |
     *     | 7 |       | c |
     *
     *     For Example:
     *     a = 1*3 = 3
     *     b = 4*3 = 12
     */

    MVector result;
    result.setX(x * scalar);
    result.setY(y * scalar);
    result.setZ(z * scalar);
    return result;
}

const double &MVector::operator[](int index) const  {
    switch (index) {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        default:
            std::cerr << std::endl
                      << "Error: Vectors contain 3 coordinates only, make sure the index is less than or equal to two."
                      << std::endl;
            exit(2);
            break;
    }
}

double dotProduct(MVector v1, MVector v2) {
    // Dot product calculated between v1 and v2
    // Source: https://tutorial.math.lamar.edu/classes/calcii/dotproduct.aspx
    return (v1.getX() * v2.getX() + v1.getY() * v2.getY() + v1.getZ() * v2.getZ());
}

double scalarTripleProduct(MVector dotA, MVector xB, MVector xC) {
    return dotProduct(dotA, (xB * xC));
}

