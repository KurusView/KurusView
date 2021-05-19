#include "MTetrahedron.h"

#include <stdlib.h>

MTetrahedron::MTetrahedron(const std::vector<std::shared_ptr<MVector>> &vertices,
                           const std::shared_ptr<MMaterial> &material, long int id) : MCell(vertices, material, id) {

    if (vertices.size() != 4) {
        std::cerr << std::endl << "Error: Tetrahedron must have 4 vertices, but " +
                                  std::to_string(vertices.size()) + " given. Cell ID: " +
                                  std::to_string(id) << std::endl;
        exit(1);
    }
    
    MCellType = MCellType_TypeDef::TETRAHEDRON;

    // calling base Pure Virtuals from Derived constructor is safe here.
    MCellVolume = this->calcVolume();
    MCellWeight = this->calcWeight();
    MCellCOG = this->calcCentreOfGrav();
}

// Function to calculate the volume of the tetrahedron.
// Input: Nothing.
// Output: Volume of the tetrahedron.
double MTetrahedron::calcVolume() const {
    // Find the side lengths in vector form.

    // https://math.stackexchange.com/questions/1603651/volume-of-tetrahedron-using-cross-and-dot-product
    MVector a = *MCellVertices[2] - *MCellVertices[0];
    MVector b = *MCellVertices[1] - *MCellVertices[0];
    MVector c = *MCellVertices[3] - *MCellVertices[0];

    double vol1 =  scalarTripleProduct(c, a, b )/ 6.0;

    // abs
    if (vol1 < 0) vol1 *= -1;

    return vol1;
}

// Function to calculate the weight of the tetrahedron.
// Input: Nothing.
// Output: Weight of the tetrahedron.
double MTetrahedron::calcWeight() const {
    // calcVolume is called before on ctor so MCell volume is populated
    return MCellVolume * MCellMaterial->getDensity();
}

std::shared_ptr<MVector> MTetrahedron::calcCentreOfGrav() const {
    // https://math.stackexchange.com/questions/1592128/finding-center-of-mass-for-tetrahedron
    auto vertex = getVertices();

    // Partially Overlapping Cells can share Centre of Gravity
    return std::make_shared<MVector>( (*vertex[0] + *vertex[1] +
                                       *vertex[2] + *vertex[3]) / 4.0 );
}
