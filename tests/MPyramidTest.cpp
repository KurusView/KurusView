#include "gtest/gtest.h"

#include "MCell.h"
#include "MTetrahedron.h"
#include "MPyramid.h"
#include "MHexahedron.h"
#include "MVector.h"

#include "MMaterial.h"

std::shared_ptr<MMaterial> SharedMaterial = std::make_shared<MMaterial>("testM",
                                                                      "FFFF",
                                                                      8940,
                                                                      11);

TEST(MPyramidTest, getVolume) {
    double expectedVolume = 2.67;
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

    double volume = pyramid.getVolume();

    ASSERT_NEAR(volume, expectedVolume, 0.009);
}

TEST(MPyramidTest, getWeight) {
    double expectedMass = 23840;
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
    double mass = pyramid.getWeight();

    ASSERT_NEAR(mass, expectedMass, 0.009);
}

TEST(MPyramidTest, getCentreOfGrav) {
    MVector centreExpected(1, -1, 0.4);
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
    std::shared_ptr<MVector> centreObtained = pyramid.getCentreOfGrav();

    ASSERT_NEAR(centreObtained->getX(), centreExpected.getX(), 0.009);
    ASSERT_NEAR(centreObtained->getY(), centreExpected.getY(), 0.009);
    ASSERT_NEAR(centreObtained->getZ(), centreExpected.getZ(), 0.009);
}
