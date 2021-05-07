#include "gtest/gtest.h"

#include "MCell.h"
#include "MTetrahedron.h"
#include "MPyramid.h"
#include "MHexahedron.h"

#include "Material.h"

// Define a vertex
std::shared_ptr<MVector> SharedVector = std::make_shared<MVector>(1, 2, 3, 99);

// Define a Material
std::shared_ptr<Material> SharedMaterial = std::make_shared<Material>("testM",
                                                                      "FFFF",
                                                                      123456,
                                                                      11);

// create vector of vertexes
std::vector<std::shared_ptr<MVector>> tetra_vertex = {SharedVector, SharedVector,
                                                      SharedVector, SharedVector};

std::vector<std::shared_ptr<MVector>> pyra_vertex = {SharedVector, SharedVector,
                                                     SharedVector, SharedVector, SharedVector};

std::vector<std::shared_ptr<MVector>> hexa_vertex = {SharedVector, SharedVector, SharedVector, SharedVector,
                                                     SharedVector, SharedVector, SharedVector, SharedVector };

// create some cells to increase the instance count (instance count = 3)
MTetrahedron tetra(tetra_vertex, SharedMaterial, 0);
MPyramid pyra(pyra_vertex, SharedMaterial, 0);
MHexahedron hexa(hexa_vertex, SharedMaterial, 0);


TEST(MCellTest, instanceCounter) {

    {
        // this one gets destructed and should not increase the count
        MTetrahedron tetraY(tetra_vertex, SharedMaterial, 0);
    }

    ASSERT_TRUE(MCell::getCount() == 3);
}

TEST(MCellTest, volume) {

    // shared vertexes, zero volume
    ASSERT_TRUE(tetra.getVolume() == 0);
    ASSERT_TRUE(pyra.getVolume() == 0);
    ASSERT_TRUE(hexa.getVolume() == 0);
}

TEST(MCellTest, id) {

    // anonymous vectors
    ASSERT_TRUE(tetra.getID() == 0);
    ASSERT_TRUE(tetra.getID() == 0);
    ASSERT_TRUE(tetra.getID() == 0);
}







