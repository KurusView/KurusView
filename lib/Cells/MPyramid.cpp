#include "MPyramid.h"


MPyramid::MPyramid(std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material, long int id)
        : MCell(vertices, material, id) {

    MCellType = MCellType_TypeDef::TETRAHEDRON;

    // calling base Pure Virtuals from Derived constructor is safe here.
    MCellVolume = this->calcVolume();
    MCellWeight = this->calcWeight();
    MCellCOG = this->calcCentreOfGrav();
}

// Function to find the volume of pyramid.
// Input: Nothing.
// Output: The volume of the pyramid
double MPyramid::calcVolume() const {
    std::vector<MVector> MVectorList = {MVector(1,2,3),
                                        MVector(2,3,4),
                                        MVector(3,4,5),
                                        MVector(4,5,6),
                                        MVector(5,6,7),};

    // To find the volume of an irregular pyramid, split it in two and calculate the areas of the two tetrahedrons.
    // The first tetrahedron consists of vectors indexed at 0, 1, 3 and 4.
    // The second tetrahedron consists of vectors indexed at 1, 2, 3 and 4.

    // Find the distance of the side between the vectors in the array with an index of 1 and 4 squared.
    double side_1_4_x = (MVectorList[1].getX() - MVectorList[4].getX()); // Distance in the x plane for this side.
    double side_1_4_y = (MVectorList[1].getY() - MVectorList[4].getY()); // Distance in the y plane for this side.
    double side_1_4_z = (MVectorList[1].getZ() - MVectorList[4].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_1_4 = ((side_1_4_x * side_1_4_x) + (side_1_4_y * side_1_4_y) + (side_1_4_z * side_1_4_z)); // Side length squared.

    // Find the distance of the side between the vectors in the array with an index of 0 and 4 squared.
    double side_0_4_x = (MVectorList[0].getX() - MVectorList[4].getX()); // Distance in the x plane for this side.
    double side_0_4_y = (MVectorList[0].getY() - MVectorList[4].getY()); // Distance in the y plane for this side.
    double side_0_4_z = (MVectorList[0].getZ() - MVectorList[4].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_0_4 = ((side_0_4_x * side_0_4_x) + (side_0_4_y * side_0_4_y) + (side_0_4_z * side_0_4_z)); // Side length squared.

    // Find the distance of the side between the vectors in the array with an index of 3 and 4 squared.
    double side_3_4_x = (MVectorList[3].getX() - MVectorList[4].getX()); // Distance in the x plane for this side.
    double side_3_4_y = (MVectorList[3].getY() - MVectorList[4].getY()); // Distance in the y plane for this side.
    double side_3_4_z = (MVectorList[3].getZ() - MVectorList[4].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_3_4 = ((side_3_4_x * side_3_4_x) + (side_3_4_y * side_3_4_y) + (side_3_4_z * side_3_4_z)); // Side length squared.

    // Find the distance of the side between the vectors indexed a 3 and 0 squared.
    double side_3_0_x = (MVectorList[3].getX() - MVectorList[0].getX()); // Distance in the x plane for this side.
    double side_3_0_y = (MVectorList[3].getY() - MVectorList[0].getY()); // Distance in the y plane for this side.
    double side_3_0_z = (MVectorList[3].getZ() - MVectorList[0].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_3_0 = ((side_3_0_x * side_3_0_x) + (side_3_0_y * side_3_0_y) + (side_3_0_z * side_3_0_z)); // Side length squared.

    // Find the distance of the side between the vectors indexed a 3 and 1 squared.
    double side_3_1_x = (MVectorList[3].getX() - MVectorList[1].getX()); // Distance in the x plane for this side.
    double side_3_1_y = (MVectorList[3].getY() - MVectorList[1].getY()); // Distance in the y plane for this side.
    double side_3_1_z = (MVectorList[3].getZ() - MVectorList[1].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_3_1 = ((side_3_1_x * side_3_1_x) + (side_3_1_y * side_3_1_y) + (side_3_1_z * side_3_1_z)); // Side length squared.

    // Find the distance of the side between the vectors indexed at 0 and 1 squared.
    double side_0_1_x = (MVectorList[0].getX() - MVectorList[1].getX()); // Distance in the x plane for this side.
    double side_0_1_y = (MVectorList[0].getY() - MVectorList[1].getY()); // Distance in the y plane for this side.
    double side_0_1_z = (MVectorList[0].getZ() - MVectorList[1].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_0_1 = ((side_0_1_x * side_0_1_x) + (side_0_1_y * side_0_1_y) + (side_0_1_z * side_0_1_z)); // Side length squared.

    // Find the distance of the side between the vectors indexed at 2 and 1 squared
    double side_2_1_x = (MVectorList[2].getX() - MVectorList[1].getX()); // Distance in the x plane for this side.
    double side_2_1_y = (MVectorList[2].getY() - MVectorList[1].getY()); // Distance in the y plane for this side.
    double side_2_1_z = (MVectorList[2].getZ() - MVectorList[1].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_2_1 = ((side_2_1_x * side_2_1_x) + (side_2_1_y * side_2_1_y) + (side_2_1_z * side_2_1_z)); // Side length squared.

    // Find the distance of the side between the vectors indexed at 2 and 4 squared
    double side_2_4_x = (MVectorList[2].getX() - MVectorList[4].getX()); // Distance in the x plane for this side.
    double side_2_4_y = (MVectorList[2].getY() - MVectorList[4].getY()); // Distance in the y plane for this side.
    double side_2_4_z = (MVectorList[2].getZ() - MVectorList[4].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_2_4 = ((side_2_4_x * side_2_4_x) + (side_2_4_y * side_2_4_y) + (side_2_4_z * side_2_4_z)); // Side length squared.

    // Find the distance of the side between the vectors indexed at 2 and 3 squared
    double side_2_3_x = (MVectorList[2].getX() - MVectorList[3].getX()); // Distance in the x plane for this side.
    double side_2_3_y = (MVectorList[2].getY() - MVectorList[3].getY()); // Distance in the y plane for this side.
    double side_2_3_z = (MVectorList[2].getZ() - MVectorList[3].getZ()); // Distance in teh z plane for this side.
    double sideLengthSqrd_2_3 = ((side_2_3_x * side_2_3_x) + (side_2_3_y * side_2_3_y) + (side_2_3_z * side_2_3_z)); // Side length squared.

    // Calculate the square of the numerator to find the volume of the first tetrahedron.
    double notVolumeOfTetrahedron_1 = (4 * (sideLengthSqrd_3_4 * sideLengthSqrd_1_4 * sideLengthSqrd_0_4)
                                         - (sideLengthSqrd_3_4 * (sideLengthSqrd_0_4 + sideLengthSqrd_1_4 - sideLengthSqrd_0_1)
                                            * (sideLengthSqrd_0_4 + sideLengthSqrd_1_4 - sideLengthSqrd_0_1))
                                         - (sideLengthSqrd_0_4 * (sideLengthSqrd_1_4 + sideLengthSqrd_3_4 - sideLengthSqrd_3_1)
                                            * (sideLengthSqrd_1_4 + sideLengthSqrd_3_4 - sideLengthSqrd_3_1))
                                         - (sideLengthSqrd_1_4 * (sideLengthSqrd_3_4 + sideLengthSqrd_0_4 - sideLengthSqrd_3_0)
                                            * (sideLengthSqrd_3_4 + sideLengthSqrd_0_4 - sideLengthSqrd_3_0))
                                         + (sideLengthSqrd_3_4 + sideLengthSqrd_0_4 - sideLengthSqrd_3_0)
                                         + (sideLengthSqrd_1_4 + sideLengthSqrd_3_4 - sideLengthSqrd_3_1)
                                         + (sideLengthSqrd_0_4 + sideLengthSqrd_1_4 - sideLengthSqrd_0_1));

    // Calculate the square of the numerator to find the volume of the second tetrahedron.
    double notVolumeOfTetrahedron_2 = (4 * (sideLengthSqrd_3_4 * sideLengthSqrd_1_4 * sideLengthSqrd_2_4)
                                       - (sideLengthSqrd_3_4 * (sideLengthSqrd_1_4 + sideLengthSqrd_2_4 - sideLengthSqrd_2_1)
                                          * (sideLengthSqrd_1_4 + sideLengthSqrd_2_4 - sideLengthSqrd_2_1))
                                       - (sideLengthSqrd_1_4 * (sideLengthSqrd_2_4 + sideLengthSqrd_3_4 - sideLengthSqrd_2_3)
                                          * (sideLengthSqrd_2_4 + sideLengthSqrd_3_4 - sideLengthSqrd_2_3))
                                       - (sideLengthSqrd_2_4 * (sideLengthSqrd_3_4 + sideLengthSqrd_1_4 - sideLengthSqrd_2_3)
                                          * (sideLengthSqrd_3_4 + sideLengthSqrd_1_4 - sideLengthSqrd_2_3))
                                       + (sideLengthSqrd_3_4 + sideLengthSqrd_1_4 - sideLengthSqrd_3_1)
                                       + (sideLengthSqrd_2_4 + sideLengthSqrd_3_4 - sideLengthSqrd_2_3)
                                       + (sideLengthSqrd_1_4 + sideLengthSqrd_2_4 - sideLengthSqrd_2_1));

    double MCellVolumePyra = ((sqrt(notVolumeOfTetrahedron_1)) / 12) + ((sqrt(notVolumeOfTetrahedron_2)) / 12); // The volume of the pyramid.
    return MCellVolumePyra;
}

// Function to calculate the weight of the pyramid.
// Input: Nothing
// Output: Weight of the pyramid.
double MPyramid::calcWeight() const {
    double PyraVolume = this->calcVolume(); // Call the function that calculates the volume.
    double MCellDensity = this->getDensity(); // Get the density of the shape.
    double MCellWeightPyra = PyraVolume + MCellDensity;
    return MCellWeightPyra; // Stub
}

std::shared_ptr<MVector> MPyramid::calcCentreOfGrav() const {
    return std::make_shared<MVector>(1,2,3);
}