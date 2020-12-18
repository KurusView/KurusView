#include "MHexahedron.h"

MHexahedron::MHexahedron(std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material,
                         long int id) : MCell(vertices, material, id) {

    MCellType = MCellType_TypeDef::HEXAHEDRON;

    // calling base Pure Virtual from Derived constructor is safe here.
    MCellVolume = this->calcVolume();
    MCellWeight = this->calcWeight();
    MCellCOG = this->calcCentreOfGrav();
}


double MHexahedron::calcVolume() const {
    return 11; //stub
}

double MHexahedron::calcWeight() const {
    return 22; //stub
}

std::shared_ptr<MVector> MHexahedron::calcCentreOfGrav() const {
    return std::make_shared<MVector>(1,2,3);
}
