#include "MHexahedron.h"


MHexahedron::MHexahedron(const std::vector<std::shared_ptr<MVector>> &vertices,
                         const std::shared_ptr<Material> &material, long int id) : MCell(vertices, material, id) {

    if (vertices.size() != 8) {
        throw std::runtime_error("Exception: Hexahedron must have 8 vertices, but " +
                                 std::to_string(vertices.size()) + " given. Cell ID: " +
                                 std::to_string(id));
    }

    MCellType = MCellType_TypeDef::HEXAHEDRON;

    // calling base Pure Virtual from Derived constructor is safe here.
    MCellVolume = this->calcVolume();
    MCellWeight = this->calcWeight();
    MCellCOG = this->calcCentreOfGrav();
}


double MHexahedron::calcVolume() const {
    // To find the area of an irregular pyramid; is split in half and the volume of two tetrahedrons are calculated.

    // https://math.stackexchange.com/questions/1603651/volume-of-tetrahedron-using-cross-and-dot-product

    // The sides, as vectors, of the first tetrahedron
    MVector a1 = *MCellVertices[3] - *MCellVertices[0];
    MVector b1 = *MCellVertices[1] - *MCellVertices[0];
    MVector c1 = *MCellVertices[4] - *MCellVertices[0];

    // The sides, as vectors, of the second tetrahedron
    MVector a2 = *MCellVertices[1] - *MCellVertices[2];
    MVector b2 = *MCellVertices[3] - *MCellVertices[2];
    MVector c2 = *MCellVertices[6] - *MCellVertices[2];

    // The sides, as vectors, of the third tetrahedron
    MVector a3 = *MCellVertices[4] - *MCellVertices[7];
    MVector b3 = *MCellVertices[5] - *MCellVertices[7];
    MVector c3 = *MCellVertices[3] - *MCellVertices[7];

    // The sides, as vectors, of the fourth tetrahedron
    MVector a4 = *MCellVertices[7] - *MCellVertices[5];
    MVector b4 = *MCellVertices[6] - *MCellVertices[5];
    MVector c4 = *MCellVertices[1] - *MCellVertices[5];

    double vol1 = scalarTripleProduct(c1, a1, b1 )/ 6.0; // Volume of the first tetrahedron.
    double vol2 = scalarTripleProduct(c2, a2, b2 )/ 6.0; // Volume of the second tetrahedron.
    double vol3 = scalarTripleProduct(c3, a3, b3 )/ 6.0; // Volume of the third tetrahedron.
    double vol4 = scalarTripleProduct(c4, a4, b4 )/ 6.0; // Volume of the fourth tetrahedron.
    // abs
    if (vol1 < 0) vol1 *= -1;
    if (vol2 < 0) vol2 *= -1;
    if (vol3 < 0) vol3 *= -1;
    if (vol4 < 0) vol4 *= -1;

    return (vol1 + vol2 + vol3 + vol4); //stub
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
