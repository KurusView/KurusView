#ifndef INC_3DLIB_MCELL_H
#define INC_3DLIB_MCELL_H

#include <iostream>
#include <vector>

#include "MVector.h"
#include "Material.h"

class MCell {
private:

    // Private Data Member - holds count of all cells
    static long int MCellInstanceCount;


protected:

    // Data Members
    double MCellVolume;
    double MCellWeight;
    double MCellMass;
    double MCellDensity;
    const long int MCellID;

    // color? - check with Material

    std::vector<MVector> MCellVertices; // shared ptr
    Material MCellMaterial;             // shared ptr
    MVector MCellCOG;                   // shared ptr

    // note: when mapping enum to string for printing, make it Compile Time Evaluated
    // (not std::map), maybe constexpr: https://stackoverflow.com/a/63265237
    // If the above becomes a headache make this any other POD
    enum class _MCellType_t {
        NONE,
        TETRAHEDRON,
        HEXAHEDRON,
        PYRAMID,
    };

    _MCellType_t MCellType = _MCellType_t::NONE; // do this in constructor member initializer list?

    // require shape specific knowledge and are to be implemented by derived classes (virtual)
    virtual double calcVolume(void);

    virtual double calcWeight(void);

    virtual MVector calcCentreOfGrav(void);


public:

    // ================ CONSTRUCTORS/DESTRUCTORS ================
    ~MCell() = default;
    MCell(std::vector<MVector> vertices, Material material, const long int id); // shared_ptr material, vector

    // ====================== OPERATORS ==========================
    //MCell& operator=( const MCell& _mcell );

    // stdout, to file streams
    friend std::ostream &operator<<(std::ostream &os, const MCell &mCell);

    friend std::ofstream &operator<<(std::ofstream &os, const MCell &mCell);

    // ======================= ACCESSORS =========================

    // XXX: a note on const: https://stackoverflow.com/questions/8406898/benefits-of-using-const-with-scalar-type-e-g-const-double-or-const-int
    const double getVolume(void) const;

    const double getWeight(void) const;

    const double getMass(void) const;

    const double getDensity(void) const;

    const double getID(void) const;

    const double getCount(void) const;

    std::vector<std::string> getType(void) const; // eg. t, tetrahedron (for file and stdout output)


    // for now return instance copy of these. https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization
    // best implemented by reference, use smart pointers:
    const MVector getCentreOfGrav(void); // unique in 3D space? - NO! shared
    const Material getMaterial(void); // shared
    const std::vector<MVector>
    getVertices(void); // MVector shared, std::vector: http://www.cplusplus.com/forum/general/56177/

    void setMaterial(Material material); // shared
    void setVertices(std::vector<MVector> vertices); // MVector shared

    // ==============================================================
};

long int MCell::MCellInstanceCount = 0;

#endif //INC_3DLIB_MCELL_H
