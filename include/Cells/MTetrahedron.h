/**
 * @file MTetrahedron.h
 * @author github.com/zadewg
 * @brief Header file for MTetrahedron derived class
 */

#ifndef INC_KURUSVIEW_MTETRAHEDRON_H
#define INC_KURUSVIEW_MTETRAHEDRON_H


#include "MCell.h"

/// @brief MTetrahedron object - inherits from MCell
class MTetrahedron : public MCell {
public:

    /**
     * @brief MTetrahedron constructor
     * @param vertices - the vertex defining the cell
     * @param material - material of the cell
     * @param id - unique cell identifier
     */
    MTetrahedron(const std::vector<std::shared_ptr<MVector>> &vertices, const std::shared_ptr<MMaterial> &material,
                 long int id);

    /**
     * @brief deleted default constructor. See MCell default ctor for reference.
     */
    MTetrahedron() = delete;

    /**
     * @brief default destructor - wraps MCell destructor (decrements Instance Count)
     *
     * @internal default destructors call base class destructors
     */
    ~MTetrahedron() = default; // this should call the base destructor regardless

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


#endif //INC_KURUSVIEW_MTETRAHEDRON_H
