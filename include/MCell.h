#ifndef INC_3DLIB_MCELL_H
#define INC_3DLIB_MCELL_H

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "MVector.h"
#include "Material.h"

class MCell {
private:

    // Private Data Member - holds count of all cells
    static unsigned long int MCellInstanceCount;


protected:

    // Protected constructor/destructors (abstract class)
    // Derived class should not be able to hardcode ID - they should come straight from the data file (explicit)
    explicit MCell(std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material, long int id);

    // no such thing as anonymous cells should exist
    MCell() = delete;

    ~MCell();

    // Data Members - Volume, Weight, Density use default ctors as they are initialized in the derived class immediately
    // mutable as they might need to be updated on the background and I want the accessor interface consistently const
    mutable double MCellVolume{};
    mutable double MCellWeight{};
    mutable double MCellDensity{};
    
    const long int MCellID;

    // color? - check with Material

    std::vector<std::shared_ptr<MVector>> MCellVertices; // shared ptr
    std::shared_ptr<Material> MCellMaterial;             // shared ptr

    mutable std::shared_ptr<MVector> MCellCOG;           // shared ptr - Overlapping Cells might share Centre of Gravity

    // note: when mapping enum to string for printing, make it Compile Time Evaluated
    // (not std::map), maybe constexpr: https://stackoverflow.com/a/63265237
    // If the above becomes a headache make this any other POD
    enum class MCellType_TypeDef {
        NONE,
        TETRAHEDRON,
        HEXAHEDRON,
        PYRAMID,
    };

    MCellType_TypeDef MCellType;

    // require shape specific knowledge and are to be implemented by derived classes (Pure Virtual)
    // TODO: is it better if these return weak_ptr ? - shared would work but have more overhead
    virtual double calcVolume() const = 0;

    virtual double calcWeight() const = 0;

    virtual std::shared_ptr<MVector> calcCentreOfGrav() const = 0;


public:

    // TODO: do operators and friends require this visibility?
    // ====================== OPERATORS ==========================
    //MCell& operator=( const MCell& _mcell );

    // stdout, to file streams
    friend std::ostream &operator<<(std::ostream &os, const MCell &mCell);

    friend std::ofstream &operator<<(std::ofstream &os, const MCell &mCell);

    // ======================= ACCESSORS =========================

    // XXX: a note on const: https://stackoverflow.com/questions/8406898/benefits-of-using-const-with-scalar-type-e-g-const-double-or-const-int
    // XXX: a note on return copy https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization
    double getVolume() const;

    double getWeight() const;

    double getDensity() const;

    double getID() const;

    static double getCount();

    std::vector<std::string> getType() const; // eg. t, tetrahedron (for file and stdout output)


    std::shared_ptr<MVector> getCentreOfGrav() const; // unique in 3D space? - NO! shared
    std::shared_ptr<Material> getMaterial() const; // shared
    std::vector<std::shared_ptr<MVector>> getVertices() const; // MVector shared

    // Note on returning vector: std::vector: http://www.cplusplus.com/forum/general/56177/

    //TODO: do we really need these? - probably not
    void setMaterial(std::shared_ptr<Material> material); // shared
    void setVertices(std::vector<std::shared_ptr<MVector>> vertices); // MVector shared

    // ==============================================================
};


#endif //INC_3DLIB_MCELL_H
