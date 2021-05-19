#include "gtest/gtest.h"

#include "MCell.h"
#include "MTetrahedron.h"
#include "MPyramid.h"
#include "MHexahedron.h"
#include "MVector.h"

#include "MMaterial.h"

// Define a vertex
std::shared_ptr<MVector> SharedVector = std::make_shared<MVector>(1, 2, 3, 99);

// Define a Material
std::shared_ptr<MMaterial> SharedMaterial = std::make_shared<MMaterial>("testM",
                                                                      "FFFF",
                                                                      8940,
                                                                      11);

// create vector of vertexes
std::vector<std::shared_ptr<MVector>> tetra_vertex = {SharedVector, SharedVector,
                                                      SharedVector, SharedVector};

std::vector<std::shared_ptr<MVector>> pyra_vertex = {SharedVector, SharedVector,
                                                     SharedVector, SharedVector, SharedVector};

std::vector<std::shared_ptr<MVector>> hexa_vertex = {SharedVector, SharedVector, SharedVector, SharedVector,
                                                     SharedVector, SharedVector, SharedVector, SharedVector};

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

TEST(MCellTest, getVertices) {
    std::vector<std::shared_ptr<MVector>> vertices;
    std::shared_ptr<MVector> v1 = std::make_shared<MVector>(0.0, 0.0, 0.0);
    std::shared_ptr<MVector> v2 = std::make_shared<MVector>(0.0, -2.0, 0.0);
    std::shared_ptr<MVector> v3 = std::make_shared<MVector>(2.0, -2.0, 0.0);
    std::shared_ptr<MVector> v4 = std::make_shared<MVector>(2.0, 0.0, 0.0);
    std::shared_ptr<MVector> v5 = std::make_shared<MVector>(1.0, -1.0, 2.0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);

    MPyramid pyramid(vertices, SharedMaterial, 0);

    std::vector<std::shared_ptr<MVector>> verticesObtained = pyramid.getVertices();

    for (int i = 0; i < vertices.size(); i++) {
        ASSERT_EQ(vertices[i], verticesObtained[i]);
    }
}
