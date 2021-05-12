#include "gtest/gtest.h"

#include "MCell.h"
#include "MTetrahedron.h"
#include "MPyramid.h"
#include "MHexahedron.h"
#include "MVector.h"

#include "Material.h"

std::shared_ptr<Material> SharedMaterial = std::make_shared<Material>("testM",
                                                                      "FFFF",
                                                                      8940,
                                                                      11);

TEST(MHexahedronTest, getVolume) {
    double expectedVolume = 8;
    std::vector<std::shared_ptr<MVector>> vertices;
    std::shared_ptr<MVector> v1 = std::make_shared<MVector>(-1, -1, -1);
    std::shared_ptr<MVector> v2 = std::make_shared<MVector>(1, -1, -1);
    std::shared_ptr<MVector> v3 = std::make_shared<MVector>(1, 1, -1);
    std::shared_ptr<MVector> v4 = std::make_shared<MVector>(-1, 1, -1);
    std::shared_ptr<MVector> v5 = std::make_shared<MVector>(-1, -1, 1);
    std::shared_ptr<MVector> v6 = std::make_shared<MVector>(1, -1, 1);
    std::shared_ptr<MVector> v7 = std::make_shared<MVector>(1, 1, 1);
    std::shared_ptr<MVector> v8 = std::make_shared<MVector>(-1, 1, 1);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);
    vertices.push_back(v8);

    MHexahedron hexa(vertices, SharedMaterial, 0);

    double volume = hexa.getVolume();

    ASSERT_NEAR(volume, expectedVolume, 0.009);
}

TEST(MHexahedronTest, getWeight) {
    double expectedMass = 71520;
    std::vector<std::shared_ptr<MVector>> vertices;
    std::shared_ptr<MVector> v1 = std::make_shared<MVector>(-1, -1, -1);
    std::shared_ptr<MVector> v2 = std::make_shared<MVector>(1, -1, -1);
    std::shared_ptr<MVector> v3 = std::make_shared<MVector>(1, 1, -1);
    std::shared_ptr<MVector> v4 = std::make_shared<MVector>(-1, 1, -1);
    std::shared_ptr<MVector> v5 = std::make_shared<MVector>(-1, -1, 1);
    std::shared_ptr<MVector> v6 = std::make_shared<MVector>(1, -1, 1);
    std::shared_ptr<MVector> v7 = std::make_shared<MVector>(1, 1, 1);
    std::shared_ptr<MVector> v8 = std::make_shared<MVector>(-1, 1, 1);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);
    vertices.push_back(v8);

    MHexahedron hexa(vertices, SharedMaterial, 0);

    double mass = hexa.getWeight();

    ASSERT_NEAR(mass, expectedMass, 0.009);
}

TEST(MHexahedronTest, getCentreOfGrav) {
    MVector centreExpected(0, 0, 0);
    std::vector<std::shared_ptr<MVector>> vertices;
    std::shared_ptr<MVector> v1 = std::make_shared<MVector>(-1, -1, -1);
    std::shared_ptr<MVector> v2 = std::make_shared<MVector>(1, -1, -1);
    std::shared_ptr<MVector> v3 = std::make_shared<MVector>(1, 1, -1);
    std::shared_ptr<MVector> v4 = std::make_shared<MVector>(-1, 1, -1);
    std::shared_ptr<MVector> v5 = std::make_shared<MVector>(-1, -1, 1);
    std::shared_ptr<MVector> v6 = std::make_shared<MVector>(1, -1, 1);
    std::shared_ptr<MVector> v7 = std::make_shared<MVector>(1, 1, 1);
    std::shared_ptr<MVector> v8 = std::make_shared<MVector>(-1, 1, 1);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);
    vertices.push_back(v5);
    vertices.push_back(v6);
    vertices.push_back(v7);
    vertices.push_back(v8);

    MHexahedron hexa(vertices, SharedMaterial, 0);

    std::shared_ptr<MVector> centreObtained = hexa.getCentreOfGrav();


    ASSERT_NEAR(centreObtained->getX(), centreExpected.getX(), 0.009);
    ASSERT_NEAR(centreObtained->getY(), centreExpected.getY(), 0.009);
    ASSERT_NEAR(centreObtained->getZ(), centreExpected.getZ(), 0.009);
}


