/**
 * @file MVector.h
 * @author github.com/georgekhanachat
 * @brief Header file for MVector class
 *
 * This object encapsulates a vector with coordinates in 3D and an ID
 */

#ifndef INC_3DLIB_MVECTOR_H
#define INC_3DLIB_MVECTOR_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

class Matrix;

class MVector {
    /**
     * @brief classic friend overloaded ostream operator<< declaration - prints MVector properties to stdout in human
     *        friendly format
     * @param os - lhs element [ a.operator<<(b) ], might be std::cout or see return
     * @param obj - MVector instance
     * @return - os reference for cascading <<
     */
    friend std::ostream &operator<<(std::ostream &os, MVector &obj);

    /**
     * @brief classic friend overloaded ofstream operator<< declaration - prints MVector properties to file in Model
     *        Loader parser format
     * @param os - lhs element [ a.operator<<(b) ], might be std::cout or see return
     * @param obj - MVector instance
     * @return - os reference for cascading <<
     */
    friend std::ofstream &operator<<(std::ofstream &os, MVector &obj);

public:
    /**
     * @brief default constructor - Initializes coordinates to zero
     */
    MVector();

    /**
     * @brief constructor - Initializes coordinates
     */
    MVector(double x, double y, double z);

    /**
     * @brief constructor - Initializes coordinates and vector ID
     */
    MVector(double x, double y, double z, long int id);

    /// @brief default destructor
    ~MVector() = default;

    /**
     * @brief getX (accessor)
     * @return - x-coordinate
     */
    double getX() const;

    /**
     * @brief getY (accessor)
     * @return - y-coordinate
     */
    double getY() const;

    /**
     * @brief getZ (accessor)
     * @return - z-coordinate
     */
    double getZ() const;

    /**
     * @brief setX - Sets x-coordinate of vector
     */
    void setX(double x);

    /**
     * @brief setY - Sets y-coordinate of vector
     */
    void setY(double y);

    /**
     * @brief setZ - Sets z-coordinate of vector
     */
    void setZ(double z);

    /**
     * @brief operator+ - overloads the + operator to add two vectors directly together
     * @param obj - MVector instance to add to the called object MVector
     * @return - MVector that is a result of two added vectors
     */
    MVector operator+(const MVector &obj) const;

    /**
     * @brief operator- - overloads the - operator to add two vectors directly together
     * @param obj - MVector instance to subtract to the called object MVector
     * @return - MVector that is a result of two subtracted vectors
     */
    MVector operator-(const MVector &obj) const;

    /**
     * @brief operator* - overloads the * operator to find the cross product between two vectors
     * @param obj - MVector instance to find the cross product with the called object MVector
     * @return - Result of the cross product being an MVector
     */
    MVector operator*(const MVector &obj) const;

    /**
     * @brief operator* - overloads the * operator to multiply a vector by a scalar
     * @param scalar - a double value to multiply an MVector with
     * @return - MVector multiplied by a scalar
     */
    MVector operator*(const double &scalar) const;

    /**
    * @brief operator/ - overloads the / operator to divide a vector by a scalar
    * @param scalar - a double value to divide an MVector with
    * @return - MVector multiplied by a scalar
    */
    MVector operator/(const double &scalar) const;

    /**
     * @brief getModulus - obtains the modulus of an MVector
     * @return - Modulus of MVector
     */
    double getModulus() const;

    /**
     * @brief getID
     * @return - ID member variable
     */
    long int getID() const;

private:
    /// @brief holds the coordinates a vector in 3D - X, Y and Z coordinates
    double x;
    double y;
    double z;

    /// @brief holds ID of a vector to prevent anonymity
    long int MVectorID;
};

/**
* @brief dotProduct - finds the dot product between two vectors
* @param v1 - first instance of MVector
* @param v2 - second instance of MVector
* @return - Dot product of v1 and v2
*/
double dotProduct(MVector v1, MVector v2);

#endif //INC_3DLIB_MVECTOR_H
