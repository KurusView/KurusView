#include <gtest/gtest.h>
#include "Model.h"
#include "Material.h"

TEST(ModelTest, getMaterials) {

    Model mod("D://Github Repos//2020_GROUP_21//cmake-build-debug//KurusView//bin//Debug//models//ExampleModel1.mod");

    std::vector<std::shared_ptr<Material>> materials = mod.getMaterials();

    std::string name1 = materials[0]->getName();
    std::string colour1 = materials[0]->getColour();
    int id1 = materials[0]->getId();
    double density1 = materials[0]->getDensity();

    ASSERT_EQ(name1, "copper");
    ASSERT_EQ(colour1, "b87333");
    ASSERT_EQ(id1, 0);
    ASSERT_EQ(density1, 8960);
}

TEST(ModelTest, getVectors) {

    Model mod("D://Github Repos//2020_GROUP_21//cmake-build-debug//KurusView//bin//Debug//models//ExampleModel1.mod");

    std::vector<MVector> vertices = mod.getVectors();

    std::cout << vertices.size() << std::endl;

    double x1 = vertices[0].getX();
    double x2 = vertices[1].getX();
    double y1 = vertices[0].getY();
    double y2 = vertices[1].getY();
    double z1 = vertices[0].getZ();
    double z2 = vertices[1].getZ();

    ASSERT_NEAR(x1, 0, 0.009);
    ASSERT_NEAR(y1, 0, 0.009);
    ASSERT_NEAR(z1, 0, 0.009);
    ASSERT_NEAR(x2, 1, 0.009);
    ASSERT_NEAR(y2, 0, 0.009);
    ASSERT_NEAR(z2, 0, 0.009);
}

TEST(ModelTest, getCells) {

    Model mod("D://Github Repos//2020_GROUP_21//cmake-build-debug//KurusView//bin//Debug//models//ExampleModel1.mod");
    std::vector<std::shared_ptr<MCell>> cells = mod.getCells();
    std::vector<std::shared_ptr<MVector>> cellVertices = cells[0]->getVertices();

    double x1 = cellVertices[0]->getX();
    double y1 = cellVertices[0]->getY();
    double z1 = cellVertices[0]->getZ();

    ASSERT_NEAR(x1, 0, 0.009);
    ASSERT_NEAR(y1, 0, 0.009);
    ASSERT_NEAR(z1, 0, 0.009);
}

TEST(ModelTest, materialsCount) {

    int countExpected = 2;

    Model mod("D://Github Repos//2020_GROUP_21//cmake-build-debug//KurusView//bin//Debug//models//ExampleModel1.mod");

    int countObtained = mod.getMaterialCount();

    ASSERT_EQ(countObtained, countExpected);
}

TEST(ModelTest, vectorCount) {

    int countExpected = 12;

    Model mod("D://Github Repos//2020_GROUP_21//cmake-build-debug//KurusView//bin//Debug//models//ExampleModel1.mod");

    int countObtained = mod.getVectorCount();

    ASSERT_EQ(countObtained, countExpected);
}

TEST(ModelTest, cellCount) {

    int countExpected = 2;

    Model mod("D://Github Repos//2020_GROUP_21//cmake-build-debug//KurusView//bin//Debug//models//ExampleModel1.mod");

    int countObtained = mod.getCellCount();

    ASSERT_EQ(countObtained, countExpected);
}

TEST(ModelTest, calcWeight) {

    int weightExpected = 9185;

    Model mod("D://Github Repos//2020_GROUP_21//cmake-build-debug//KurusView//bin//Debug//models//ExampleModel1.mod");

    double weightObtained = mod.calcWeight();

    ASSERT_EQ(weightObtained, weightExpected);
}

TEST(ModelTest, calcVolume) {

    double volumeExpected = 1.08;

    Model mod("D://Github Repos//2020_GROUP_21//cmake-build-debug//KurusView//bin//Debug//models//ExampleModel1.mod");

    double volumeObtained = mod.calcVolume();

    ASSERT_NEAR(volumeObtained, volumeExpected, 0.01);
}

TEST(ModelTest, calcCentreOfGrav) {

    MVector centreOfGravExpected(0.625, 0.619, 0.491);

    Model mod("D://Github Repos//2020_GROUP_21//cmake-build-debug//KurusView//bin//Debug//models//ExampleModel1.mod");

    MVector centreOfGravObtained = mod.calcCentre();

    ASSERT_NEAR(centreOfGravObtained.getX(), centreOfGravExpected.getX(), 0.009);
    ASSERT_NEAR(centreOfGravObtained.getY(), centreOfGravExpected.getY(), 0.009);
    ASSERT_NEAR(centreOfGravObtained.getZ(), centreOfGravExpected.getZ(), 0.009);
}