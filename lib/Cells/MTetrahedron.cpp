#include "MTetrahedron.h"

MTetrahedron::MTetrahedron(std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material,
                           long int id) : MCell(vertices, material, id) {

    MCellType = MCellType_TypeDef::TETRAHEDRON;

    // calling base Pure Virtuals from Derived constructor is safe here.
    MCellVolume = calcVolume();
    MCellWeight = calcWeight();
    MCellCOG = calcCentreOfGrav();
}


double MTetrahedron::calcVolume() const {
    return 11; //stub
}

double MTetrahedron::calcWeight() const {
    return 22; //stub
}

std::shared_ptr<MVector> MTetrahedron::calcCentreOfGrav() const {
    return std::make_shared<MVector>(1,2,3);
}
