#include "MHexahedron.h"


MHexahedron::MHexahedron(const std::vector<std::shared_ptr<MVector>> &vertices,
                         const std::shared_ptr<Material> &material, long int id) : MCell(vertices, material, id) {

    if (vertices.size() != 8) {
        std::cerr << std::endl << "Error: Hexahedron must have 8 vertices, but " +
                                  std::to_string(vertices.size()) + " given. Cell ID: " +
                                  std::to_string(id) << std::endl;
        exit(1);
    }

    MCellType = MCellType_TypeDef::HEXAHEDRON;

    // calling base Pure Virtual from Derived constructor is safe here.
    MCellVolume = this->calcVolume();
    MCellWeight = this->calcWeight();
    MCellCOG = this->calcCentreOfGrav();

    // reserve space for the vertex IDs
    vertexIDs.reserve(8);

    // populate vertexIDs
    for (auto &it : vertices) {
        vertexIDs.emplace_back(it->getID());
    }
}


double MHexahedron::calcVolume() const {
    return 11; //stub
}

double MHexahedron::calcWeight() const {
    // calcVolume is called before on ctor so MCell volume is populated
    return MCellVolume * MCellMaterial->getDensity();
}

std::shared_ptr<MVector> MHexahedron::calcCentreOfGrav() const {
    auto vertex = getVertices();

    // Partially Overlapping Cells can share Centre of Gravity
    return std::make_shared<MVector>( (*vertex[0] + *vertex[1] +
                                       *vertex[2] + *vertex[3] +
                                       *vertex[4] + *vertex[5] +
                                       *vertex[6] + *vertex[7]) / 8.0 );
}
