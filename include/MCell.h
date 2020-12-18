/**
 * @file MCell.h
 * @author github.com/zadewg
 * @brief Header file for MCell base class
 *
 * Implements the MCell interface base class. MTetrahedron, MPyramid, MHexahedron inherit from this class.
 */

#ifndef INC_3DLIB_MCELL_H
#define INC_3DLIB_MCELL_H


#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "MVector.h"
#include "Material.h"

/**
 * @brief MCell - Interface (abstract) class for all the cell shapes
 */
class MCell {
private:

    /// @brief holds count of all cells alive
    static unsigned long int MCellInstanceCount;


protected:

    /**
     * @brief MCell constructor
     * @param vertices - the vertex defining the cell
     * @param material - material of the cell
     * @param id - unique cell identifier
     *
     * @note Protected accessibility because MCell is an abstract class. Explicit because Derived class should not be
     *       able to hardcode ID - it should come straight from the data file
     */
    explicit MCell(std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material, long int id);

    /**
     * @brief default constructor - deleted
     * @note No such thing as anonymous (ID - less) cells should exist
     */
    MCell() = delete;

    /// @brief destructor - decrements InstanceCount
    ~MCell();

    /**
     * @note Data Members Volume, Weight, Density use default ctors as they are initialized in the derived class
     *       immediately and its cleaner syntax to do it here rather than in the Member Initializer List in some ctor.
     *
     *       Mutable as they might need to be updated on the background and I want the accessor interface consistently
     *       const to not hint on internals
     */

    /// @brief MCellVolume - stores the volume of the cell, can not be set by accessors
    mutable double MCellVolume{};

    /// @brief MCellWeight - stores the weight of the cell, can not be set by accessors
    mutable double MCellWeight{};

    /// @brief MCellDensity - stores the density of the cell, , can not be set by accessors
    const long int MCellID;

    /**
     * @TODO color? - check with Material. Should be shared_ptr, likely implemented as char array and
     *       likely _very_ shared.
     */

    /// @brief holds a vector of MVectors defining the vertex of the cell
    std::vector<std::shared_ptr<MVector>> MCellVertices;

    /// @brief holds a the material of the cell
    std::shared_ptr<Material> MCellMaterial;


    /**
     * @brief holds the Centre Of Gravity of the cell as a shared pointer MVector
     * @note shared_ptr because Overlapping Cells might share Centre of Gravity
     */
    mutable std::shared_ptr<MVector> MCellCOG;


    /**
     * @note when mapping enum to string for printing, make it Compile Time Evaluated (not normal std::map), maybe
     *       through constexpr: https://stackoverflow.com/a/63265237 #,
     *       https://stackoverflow.com/questions/61281843/creating-compile-time-key-value-map-in-c
     *
     */
    enum class MCellType_TypeDef {
        NONE,
        TETRAHEDRON,
        HEXAHEDRON,
        PYRAMID,
    };


    /// @brief MCellType - holds the type of the current cell
    MCellType_TypeDef MCellType;

    /**
     * @brief calculates volume of the cell
     * @return scalar volume of the cell in same unit as cell vectors
     *
     * @note requires shape specific knowledge is to be implemented by derived classes (Pure Virtual)
     */
    virtual double calcVolume() const = 0;

    /**
     * @brief calculates weight of the cell
     * @return scalar weight of the cell - unit dependant on volume and density
     *
     * @TODO: This one does NOT need to be virtual.
     */
    virtual double calcWeight() const = 0;

    /**
     * @brief calculates centre of gravity of the cell
     * @return shared pointer to anonymous (ID - less) 3D MVector
     *
     * @TODO This should probably return weak_ptr - shared_ptr has more overhead. We dont care about the freeing
     *       differences as calc* functions are only called internally so at least one instance will exist if the
     *       method was called
     */
    virtual std::shared_ptr<MVector> calcCentreOfGrav() const = 0;


public:

    // TODO: do overloaded operators and friends require this visibility?
    // ====================== OPERATORS ==========================

    /// @TODO Unsure if useful. should perform deep (not shallow) copy to avoid accidental freeing.
    //MCell& operator=( const MCell& _mcell );

    /**
     * @brief classic friend overloaded ostream operator<< declaration - prints MCell properties to stdout in human
     *        friendly format
     * @param os - lhs element [ a.operator<<(b) ], might be std::cout or see return
     * @param mCell - MCell instance
     * @return os reference for cascading <<
     */
    friend std::ostream &operator<<(std::ostream &os, const MCell &mCell);

    /**
     * @brief classic friend overloaded ofstream operator<< declaration - prints MCell properties to file in Model
     *        Loader parser format
     * @param os - lhs element [ a.operator<<(b) ], might be std::cout or see return
     * @param mCell
     * @return os reference for cascading <<
     */
    friend std::ofstream &operator<<(std::ofstream &os, const MCell &mCell);

    // ======================= ACCESSORS =========================

    // XXX: a note on const return: https://stackoverflow.com/questions/8406898/benefits-of-using-const-with-scalar-type-e-g-const-double-or-const-int
    // XXX: a note on return by value https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization
    // XXX: a not on returning vector: std::vector: http://www.cplusplus.com/forum/general/56177/

    /**
     * @brief get MCellVolume (accessor)
     * @return MCellVolume copy
     */
    double getVolume() const;

    /**
     * @brief get MCellWeight (accessor)
     * @return MCellWeight copy
     */
    double getWeight() const;

    /**
     * @brief get MCellID (accessor)
     * @return MCellID copy
     */
    double getID() const;

    /**
     * @brief get MCellInstanceCount (accessor)
     * @return MCellInstanceCount copy
     */
    static double getCount();

    /**
     * @brief get MCellType (accessor)
     * @return Vector of two strings, first element in file format, second in human readable format
     *
     * @TODO: Implement Compile Time Evaluated mapping, maybe use constexpr array.
     */
    std::vector<std::string> getType() const;

    /**
     * @brief get MCellCOG (accessor)
     * @return shared pointer to anonymous (ID - less) 3D MVector instance
     */
    std::shared_ptr<MVector> getCentreOfGrav() const;

    /**
     * @brief  get MCellMaterial (accessor)
     * @return shared pointer to Material instance
     */
    std::shared_ptr<Material> getMaterial() const;

    /**
     * @brief  get MCellVertices (accessor)
     * @return vector of shared pointer to ID represented MVectors
     */
    std::vector<std::shared_ptr<MVector>> getVertices() const;


    /**
     * @TODO: Do we really need these? - These setters are meant to be used to apply transforms on the object
     *        (MColor change, rotation, etc), it is unclear at the moment if they are _actually_ needed, as the
     *        transformer - the gpu or some other function could access the memory directly through getVectors().
     *        It probably makes more sense for this class to remain as visualization oriented as possible.
     *
     *        See reverted commit b0e030c for a reference on implementing const methods.
     */

    /**
     * @brief set MCellMaterial accesor
     * @param material - a shared pointer to Material of the cell
     */
    void setMaterial(std::shared_ptr<Material> material);

    /**
     * @brief set MCellMaterial accesor
     * @param vertices - a vector a shared pointer to vertex of the cell
     */
    void setVertices(std::vector<std::shared_ptr<MVector>> vertices);

};

#endif //INC_3DLIB_MCELL_H
