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

TEST(MTetrahedronTest, getVolume) {
    double expectedVolume = 1.33;
    std::vector<std::shared_ptr<MVector>> vertices;
    std::shared_ptr<MVector> v1 = std::make_shared<MVector>(5.0, 0.0, 0.0);
    std::shared_ptr<MVector> v2 = std::make_shared<MVector>(5.0, 1.0, 1.0);
    std::shared_ptr<MVector> v3 = std::make_shared<MVector>(6.0, 1.0, 5.0);
    std::shared_ptr<MVector> v4 = std::make_shared<MVector>(5.5, 6.5, 0.5);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    MTetrahedron tetra(vertices, SharedMaterial, 0);

    double volume = tetra.getVolume();

    ASSERT_NEAR(volume, expectedVolume, 0.009);
}

TEST(MTetrahedronTest, getWeight) {
    double expectedMass = 11920;
    std::vector<std::shared_ptr<MVector>> vertices;
    std::shared_ptr<MVector> v1 = std::make_shared<MVector>(5.0, 0.0, 0.0);
    std::shared_ptr<MVector> v2 = std::make_shared<MVector>(5.0, 1.0, 1.0);
    std::shared_ptr<MVector> v3 = std::make_shared<MVector>(6.0, 1.0, 5.0);
    std::shared_ptr<MVector> v4 = std::make_shared<MVector>(5.5, 6.5, 0.5);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    MTetrahedron tetra(vertices, SharedMaterial, 0);

    double mass = tetra.getWeight();

    ASSERT_NEAR(mass, expectedMass, 0.009);
}

TEST(MTetrahedronTest, getCentreOfGrav) {
    MVector centreExpected(5.375, 2.125, 1.625);
    std::vector<std::shared_ptr<MVector>> vertices;
    std::shared_ptr<MVector> v1 = std::make_shared<MVector>(5.0, 0.0, 0.0);
    std::shared_ptr<MVector> v2 = std::make_shared<MVector>(5.0, 1.0, 1.0);
    std::shared_ptr<MVector> v3 = std::make_shared<MVector>(6.0, 1.0, 5.0);
    std::shared_ptr<MVector> v4 = std::make_shared<MVector>(5.5, 6.5, 0.5);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    MTetrahedron tetra(vertices, SharedMaterial, 0);

    std::shared_ptr<MVector> centreObtained = tetra.getCentreOfGrav();


    ASSERT_NEAR(centreObtained->getX(), centreExpected.getX(), 0.009);
    ASSERT_NEAR(centreObtained->getY(), centreExpected.getY(), 0.009);
    ASSERT_NEAR(centreObtained->getZ(), centreExpected.getZ(), 0.009);
}


