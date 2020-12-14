#include "MHexahedron.h"

MHexahedron::MHexahedron(std::vector<MVector> vertices, Material material, const long id) : MCell(vertices, material,
                                                                                                  id) {
    MCellType = MCellType_TypeDef::TETRAHEDRON;

    // calling base Pure Virtuals from Derived constructor is safe here.
    MCellVolume = calcVolume();
    MCellWeight = calcWeight();
    MCellCOG = calcCentreOfGrav();
}


double MHexahedron::calcVolume() const {
    return 11; //stub
}

double MHexahedron::calcWeight() const {
    return 22; //stub
}

MVector MHexahedron::calcCentreOfGrav() const {
    return MVector(1, 2, 3); //stub
}
