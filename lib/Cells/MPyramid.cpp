#include "MPyramid.h"


MPyramid::MPyramid(std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material, long int id)
        : MCell(vertices, material, id) {

    MCellType = MCellType_TypeDef::TETRAHEDRON;

    // calling base Pure Virtuals from Derived constructor is safe here.
    MCellVolume = this->calcVolume();
    MCellWeight = this->calcWeight();
    MCellCOG = this->calcCentreOfGrav();
}


double MPyramid::calcVolume() const {
    return 11; //stub
}

double MPyramid::calcWeight() const {
    return 22; //stub
}

std::shared_ptr<MVector> MPyramid::calcCentreOfGrav() const {
    return std::make_shared<MVector>(1,2,3);
}