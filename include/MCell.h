#ifndef INC_3DLIB_MCELL_H
#define INC_3DLIB_MCELL_H

#include <iostream>
#include <fstream>
#include <vector>

#include "MVector.h"
#include "Material.h"

class MCell {
private:

    // Private Data Member - holds count of all cells
    static unsigned long int MCellInstanceCount;


protected:

    // Data Members
    mutable double MCellVolume;
    mutable double MCellWeight;
    double MCellDensity;
    const long int MCellID;

    // color? - check with Material

    std::vector<MVector> MCellVertices; // shared ptr
    Material MCellMaterial;             // shared ptr
    mutable MVector MCellCOG;           // shared ptr

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
    virtual double calcVolume() const = 0;

    virtual double calcWeight() const = 0;

    virtual MVector calcCentreOfGrav() const = 0;


public:

    // ================ CONSTRUCTORS/DESTRUCTORS ================
    MCell(std::vector<MVector> vertices, Material material, const long int id); // shared_ptr material, vector
    ~MCell();

    // ====================== OPERATORS ==========================
    //MCell& operator=( const MCell& _mcell );

    // stdout, to file streams
    friend std::ostream &operator<<(std::ostream &os, const MCell &mCell);

    friend std::ofstream &operator<<(std::ofstream &os, const MCell &mCell);

    // ======================= ACCESSORS =========================

    // XXX: a note on const: https://stackoverflow.com/questions/8406898/benefits-of-using-const-with-scalar-type-e-g-const-double-or-const-int
    double getVolume() const;

    double getWeight() const;

    double getDensity() const;

    double getID() const;

    static double getCount();

    std::vector<std::string> getType() const; // eg. t, tetrahedron (for file and stdout output)


    // for now return instance copy of these. https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization
    // best implemented by reference, use smart pointers:
    const MVector getCentreOfGrav() const; // unique in 3D space? - NO! shared
    const Material getMaterial() const; // shared
    const std::vector<MVector> getVertices() const; // MVector shared

    // Note on returning vector: std::vector: http://www.cplusplus.com/forum/general/56177/

    void setMaterial(Material material); // shared
    void setVertices(std::vector<MVector> vertices); // MVector shared

    // ==============================================================
};


#endif //INC_3DLIB_MCELL_H
