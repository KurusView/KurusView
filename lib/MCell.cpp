#include "MCell.h"

#include <map>

long int MCell::MCellInstanceCount = 0;

// Member Initializer Lists should be in declaration order
MCell::MCell(std::vector<MVector> vertices, Material material, const long int id) : MCellID(id),
                                                                                    MCellVertices(vertices),
                                                                                    MCellMaterial(material),
                                                                                    MCellType(MCellType_TypeDef::NONE) {
    MCellInstanceCount++;
    // when instantiating derived class, this constructor is called. Polymorphism can exist between base
    // and derived classes with the "using" keyword.

    // the derived class should initialize the other data members evaluating the calc methods. This methods being
    // Pure Virtual can be called from the derived constructor without UB
}

MCell::~MCell() {
    MCellInstanceCount--;
}

double MCell::getVolume() const {
    return this->MCellVolume;
}

double MCell::getWeight() const {
    return this->MCellWeight;
}

const MVector MCell::getCentreOfGrav() const {
    return this->MCellCOG;
}

double MCell::getDensity() const {
    return this->MCellDensity;
}

double MCell::getID() const {
    return this->MCellID;
}

const Material MCell::getMaterial() const {
    return this->MCellMaterial;
}

const std::vector<MVector> MCell::getVertices() const {
    return this->MCellVertices;
}

double MCell::getCount() const {
    return this->MCellInstanceCount;
}


void MCell::setMaterial(Material material) {
    this->MCellMaterial = material;
}

void MCell::setVertices(std::vector<MVector> vertices) {
    this->MCellVertices = vertices;
}


std::vector<std::string> MCell::getType() const {

    // cant be constexpr, look for Compile Time Evaluated alternative
    // https://stackoverflow.com/questions/16490835/how-to-build-a-compile-time-key-value-store
    std::map<MCell::MCellType_TypeDef, std::vector<std::string>> MCellType_str{
            {MCell::MCellType_TypeDef::TETRAHEDRON, {"t", "tetrahedron"}},
            {MCell::MCellType_TypeDef::PYRAMID,     {"p", "pyramid"}},
            {MCell::MCellType_TypeDef::HEXAHEDRON,  {"h", "hexahedron"}}
    };

    return MCellType_str[this->MCellType];
}

inline std::ostream &operator<<(std::ostream &os, const MCell &mCell) {
    os << mCell.getType()[1];

    return os;
}

inline std::ofstream &operator<<(std::ofstream &os, const MCell &mCell) {
    os << mCell.getType()[0];

    return os;
}
