#include "MHexahedron.h"

#include <stdlib.h>

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
}


double MHexahedron::calcVolume() const {

    // https://math.stackexchange.com/questions/1603651/volume-of-tetrahedron-using-cross-and-dot-product

    // vertex of the first pyramid composing the hexahedron
    MVector a1 = *MCellVertices[0] - *MCellVertices[1];
    MVector b1 = *MCellVertices[0] - *MCellVertices[3];
    MVector c1 = *MCellVertices[0] - *MCellVertices[4];

    // vertex of the first pyramid composing the hexahedron
    MVector a2 = *MCellVertices[3] - *MCellVertices[0];
    MVector b2 = *MCellVertices[3] - *MCellVertices[7];
    MVector c2 = *MCellVertices[3] - *MCellVertices[2];

    // vertex of the first pyramid composing the hexahedron
    MVector a3 = *MCellVertices[1] - *MCellVertices[5];
    MVector b3 = *MCellVertices[1] - *MCellVertices[2];
    MVector c3 = *MCellVertices[1] - *MCellVertices[0];

    double vol1 = std::abs(scalarTripleProduct(c1, a1, b1) / 3.0); // Volume of the first pyramid.
    double vol2 = std::abs(scalarTripleProduct(c2, a2, b2) / 3.0); // Volume of the first pyramid.
    double vol3 = std::abs(scalarTripleProduct(c3, a3, b3) / 3.0); // Volume of the first pyramid.

    return (vol1 + vol2 + vol3);
}

double MHexahedron::calcWeight() const {
    // calcVolume is called before on ctor so MCell volume is populated
    return MCellVolume * MCellMaterial->getDensity();
}

std::shared_ptr<MVector> MHexahedron::calcCentreOfGrav() const {
    auto vertex = getVertices();

    // Partially Overlapping Cells can share Centre of Gravity
    return std::make_shared<MVector>((*vertex[0] + *vertex[1] +
                                      *vertex[2] + *vertex[3] +
                                      *vertex[4] + *vertex[5] +
                                      *vertex[6] + *vertex[7]) / 8.0);
}
