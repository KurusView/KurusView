#include "MPyramid.h"


MPyramid::MPyramid(const std::vector<std::shared_ptr<MVector>> &vertices,
                   const std::shared_ptr<Material> &material, long int id) : MCell(vertices, material, id) {

    if (vertices.size() != 5) {
        std::cerr << std::endl << "Error: Pyramid must have 5 vertices, but " +
                                  std::to_string(vertices.size()) + " given. Cell ID: " +
                                  std::to_string(id) << std::endl;
        exit(1);
    }

    MCellType = MCellType_TypeDef::PYRAMID;

    // calling base Pure Virtuals from Derived constructor is safe here.
    MCellVolume = this->calcVolume();
    MCellWeight = this->calcWeight();
    MCellCOG = this->calcCentreOfGrav();

    // reserve space for the vertex IDs
    vertexIDs.reserve(5);

    // populate vertexIDs
    for (auto &it : vertices) {
        vertexIDs.emplace_back(it->getID());
    }
}

// Function to find the volume of pyramid.
// Input: Nothing.
// Output: The volume of the pyramid
double MPyramid::calcVolume() const {
    // To find the area of an irregular pyramid; is split in half and the volume of two tetrahedrons are calculated.

    // https://math.stackexchange.com/questions/1603651/volume-of-tetrahedron-using-cross-and-dot-product

    // The sides, as vectors, of the first tetrahedron
    MVector a1 = *MCellVertices[1] - *MCellVertices[0];
    MVector b1 = *MCellVertices[2] - *MCellVertices[0];
    MVector c1 = *MCellVertices[4] - *MCellVertices[0];

    // The sides, as vectors, of the first tetrahedron
    MVector a2 = *MCellVertices[3] - *MCellVertices[2];
    MVector b2 = *MCellVertices[1] - *MCellVertices[2];
    MVector c2 = *MCellVertices[4] - *MCellVertices[2];

    double vol1 = scalarTripleProduct(c1, a1, b1 )/ 6.0; // Volume of the first tetrahedron.
    double vol2 = scalarTripleProduct(c2, a2, b2 )/ 6.0; // Volume of the second tetrahedron.

    // abs
    if (vol1 < 0) vol1 *= -1;
    if (vol2 < 0) vol2 *= -1;


    return  vol1 + vol2;
}

// Function to calculate the weight of the pyramid.
// Input: Nothing
// Output: Weight of the pyramid.
double MPyramid::calcWeight() const {
    // calcVolume is called before on ctor so MCell volume is populated
    return MCellVolume * MCellMaterial->getDensity();
}

std::shared_ptr<MVector> MPyramid::calcCentreOfGrav() const {
    auto vertex = getVertices();

    // Partially Overlapping Cells can share Centre of Gravity
    return std::make_shared<MVector>( (*vertex[0] + *vertex[1] +
                                       *vertex[2] + *vertex[3] + *vertex[4]) / 5.0 );
}