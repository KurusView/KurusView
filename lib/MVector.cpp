//#include <Matrix.h>
#include <MVector.h>

std::ostream &operator<<(std::ostream &os, MVector &obj) {
    //Overloads << operator
    // Produces an outstream to print the values of x, y and z of a vector simultaneously
    os << "x: " << obj.vertex.x << "  y: " << obj.vertex.y << "  z: " << obj.vertex.z << std::endl;
    os << std::endl;
    return os;
}

MVector::MVector() : vertex{0,0,0}, MVectorID(-1) {}

// Initializes a vector a specific input coordinates
MVector::MVector(double x, double y, double z) : vertex{x,y,z}, MVectorID(-1) {}

MVector::MVector(double x, double y, double z, const long id) : vertex{x,y,z}, MVectorID(id) {}

double MVector::getX() const {
    // Returns the value of x for a specific vector
    return vertex.x;
}

double MVector::getY() const {
    // Returns the value of y for a specific vector
    return vertex.y;
}

double MVector::getZ() const {
    // Returns the value of z for a specific vector
    return vertex.z;
}

void MVector::setX(double x) {
    // Sets the value of x for a specific vector
    this->vertex.x = x;
}

void MVector::setY(double y) {
    // Sets the value of y for a specific vector
    this->vertex.y = y;
}

void MVector::setZ(double z) {
    // Sets the value of z for a specific vector
    this->vertex.z = z;
}

double MVector::getModulus() const {
    // Calculates the modulus of a vector
    // The square root of the sum of the squares
    double mod = sqrt((vertex.x * vertex.x) + (vertex.y * vertex.y) + (vertex.z * vertex.z));
    return mod;
}

long int MVector::getID() const {
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
    result.vertex.x = vertex.x + obj.vertex.x;
    result.vertex.y = vertex.y + obj.vertex.y;
    result.vertex.z = vertex.z + obj.vertex.z;
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
    result.vertex.x = vertex.x - obj.vertex.x;
    result.vertex.y = vertex.y - obj.vertex.y;
    result.vertex.z = vertex.z - obj.vertex.z;
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
    result.vertex.x = (vertex.y * obj.getZ()) - (vertex.z * obj.getY());
    result.vertex.y = (vertex.z * obj.getX()) - (vertex.x * obj.getZ());
    result.vertex.z = (vertex.x * obj.getY()) - (vertex.y * obj.getX());
    return result;
}

vertexData MVector::getVertex() {
    return vertex;
}



