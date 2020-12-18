/**
 * @file MHexahedron.h
 * @author github.com/zadewg
 * @brief Header file for MHexahedron derived class
 */

#ifndef INC_3DLIB_MHEXAHEDRON_H
#define INC_3DLIB_MHEXAHEDRON_H


#include "MCell.h"

/// @brief MHexahedron object - inherits from MCell
class MHexahedron : public MCell {
public:

    /**
     * @brief MHexahedron constructor
     * @param vertices - the vertex defining the cell
     * @param material - material of the cell
     * @param id - unique cell identifier
     */
    MHexahedron (std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material, long int id) ;

    /**
     * @brief deleted default constructor. See MCell default ctor for reference.
     */
    MHexahedron() = delete;

    /**
     * @brief default destructor - wraps MCell destructor (decrements Instance Count)
     *
     * @internal default destructors call base class destructors
     */
    ~MHexahedron() = default;

private:
    /**
     * @brief calculates volume of the cell
     * @return scalar volume of the cell in same unit as cell vectors
     */
    double calcVolume() const override;

    /**
     * @brief calculates weight of the cell
     * @return scalar weight of the cell - unit dependant on volume and density
     *
     * @TODO: This one does NOT need to be virtual.
     */
    double calcWeight() const override;

    /**
     * @brief calculates centre of gravity of the cell
     * @return shared pointer to anonymous (ID - less) 3D MVector
     *
     * @TODO This should probably return weak_ptr - shared_ptr has more overhead. We dont care about the freeing
     *       differences as calc* functions are only called internally so at least one instance will exist if the
     *       method was called
     *
     * @TODO How do we account for the difference masses?
     */
    std::shared_ptr<MVector> calcCentreOfGrav() const override;
};


#endif //INC_3DLIB_MHEXAHEDRON_H
