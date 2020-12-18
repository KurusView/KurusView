#include "MTetrahedron.h"


MTetrahedron::MTetrahedron(std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material,
                           long int id) : MCell(std::move(vertices), std::move(material), id) {

    if (vertices.size() != 4) {
        throw std::runtime_error("Exception: Tetrahedron must have 4 vertices, but " +
                                 std::to_string(vertices.size()) + " given. Cell ID: " +
                                 std::to_string(id));
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
    std::vector<MVector> MVectorList = {MVector(1,2,3),
                                        MVector(2,3,4),
                                        MVector(3,4,5),
                                        MVector(4,5,6),};
    // Find the distance of the side between the vectors in the array with an index of 1 and 3 squared.
    double side_1_3_x = (MVectorList[1].getX() - MVectorList[3].getX()); // Distance in the x plane for this side.
    double side_1_3_y = (MVectorList[1].getY() - MVectorList[3].getY()); // Distance in the y plane for this side.
    double side_1_3_z = (MVectorList[1].getZ() - MVectorList[3].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_1_3 = ((side_1_3_x * side_1_3_x) + (side_1_3_y * side_1_3_y) + (side_1_3_z * side_1_3_z)); // Side length squared.

    // Find the distance of the side between the vectors in the array with an index of 2 and 3 squared.
    double side_2_3_x = (MVectorList[2].getX() - MVectorList[3].getX()); // Distance in the x plane for this side.
    double side_2_3_y = (MVectorList[2].getY() - MVectorList[3].getY()); // Distance in the y plane for this side.
    double side_2_3_z = (MVectorList[2].getZ() - MVectorList[3].getZ()); // Distance in the z plane for this side.
    double sideLengthSqrd_2_3 = ((side_2_3_x * side_2_3_x) + (side_2_3_y * side_2_3_y) + (side_2_3_z * side_2_3_z)); // Side length squared.

    // Find the distance of the side between the vectors in the array with an index of 0 and 3 squared.
    double side_0_3_x = (MVectorList[0].getX() - MVectorList[3].getX()); // Distance in the x plane for this side.
    double side_0_3_y = (MVectorList[0].getY() - MVectorList[3].getY()); // Distance in the x plane for this side.
    double side_0_3_z = (MVectorList[0].getZ() - MVectorList[3].getZ()); // Distance in the x plane for this side.
    double sideLengthSqrd_0_3 = ((side_0_3_x * side_0_3_x) + (side_0_3_y * side_0_3_y) + (side_0_3_z * side_0_3_z)); // Side length squared.

    // Find the distance of the side between the vectors in the array with an index of 0 and 1 squared.
    double side_0_1_x = (MVectorList[0].getX() - MVectorList[1].getX()); // Distance in the x plane for this side.
    double side_0_1_y = (MVectorList[0].getY() - MVectorList[1].getY()); // Distance in the x plane for this side.
    double side_0_1_z = (MVectorList[0].getZ() - MVectorList[1].getZ()); // Distance in the x plane for this side.
    double sideLengthSqrd_0_1 = ((side_0_1_x * side_0_1_x) + (side_0_1_y * side_0_1_y) + (side_0_1_z * side_0_1_z)); // Side length squared.

    // Find the distance of the side between the vectors in the array with an index of 0 and 2 squared.
    double side_0_2_x = (MVectorList[0].getX() - MVectorList[2].getX()); // Distance in the x plane for this side.
    double side_0_2_y = (MVectorList[0].getY() - MVectorList[2].getY()); // Distance in the x plane for this side.
    double side_0_2_z = (MVectorList[0].getZ() - MVectorList[2].getZ()); // Distance in the x plane for this side.
    double sideLengthSqrd_0_2 = ((side_0_2_x * side_0_2_x) + (side_0_2_y * side_0_2_y) + (side_0_2_z * side_0_2_z)); // Side length squared.

    // Find the distance of the side between the vectors in the array with an index of 1 and 2 squared.
    double side_1_2_x = (MVectorList[1].getX() - MVectorList[2].getX()); // Distance in the x plane for this side.
    double side_1_2_z = (MVectorList[1].getZ() - MVectorList[2].getZ()); // Distance in the x plane for this side.
    double side_1_2_y = (MVectorList[1].getY() - MVectorList[2].getY()); // Distance in the x plane for this side.
    double sideLengthSqrd_1_2 = ((side_1_2_x * side_1_2_x) + (side_1_2_y * side_1_2_y) + (side_1_2_z * side_1_2_z)); // Side length squared.

    // Find the volume of the tetrahedron.
    double notVolumeTetra = (4 * (sideLengthSqrd_2_3 * sideLengthSqrd_1_3 * sideLengthSqrd_0_3)
                        - (sideLengthSqrd_2_3 * (sideLengthSqrd_0_3 + sideLengthSqrd_1_3 - sideLengthSqrd_0_1)
                        * (sideLengthSqrd_0_3 + sideLengthSqrd_1_3 - sideLengthSqrd_0_1))
                        - (sideLengthSqrd_0_3 * (sideLengthSqrd_1_3 + sideLengthSqrd_2_3 - sideLengthSqrd_1_2)
                        * (sideLengthSqrd_1_3 + sideLengthSqrd_2_3 - sideLengthSqrd_1_2))
                        - (sideLengthSqrd_1_3 * (sideLengthSqrd_2_3 + sideLengthSqrd_0_3 - sideLengthSqrd_0_2)
                        * (sideLengthSqrd_2_3 + sideLengthSqrd_0_3 - sideLengthSqrd_0_2))
                        + (sideLengthSqrd_2_3 + sideLengthSqrd_0_3 - sideLengthSqrd_0_2)
                        + (sideLengthSqrd_1_3 + sideLengthSqrd_2_3 - sideLengthSqrd_1_2)
                        + (sideLengthSqrd_0_3 + sideLengthSqrd_1_3 - sideLengthSqrd_0_1));
    double MCellVolumeTetra = (sqrt(notVolumeTetra)) / 12; // Variable for the volume of the shape tetrahedron.
    /*MVector test = MVectorList[0] - MVectorList[1];
    double NOTASUM = MVectorList[0].getX() + MVectorList[1].getX();*/

    return MCellVolumeTetra; //stub
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
