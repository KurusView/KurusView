#include "MCell.h"

#include <map>

MCell::MCell(std::vector<MVector> vertices, Material material, const long int id) : MCellID(id),
                                                                                    MCellType(_MCellType_t::NONE) {
    MCellInstanceCount++;
    // when instantiating derived class, call this constructor to update global cell counter and id
    // https://stackoverflow.com/a/12045286
}

MCell::~MCell() {
    MCellInstanceCount--;
}

const double MCell::getVolume() const {
    return this->MCellVolume;
}

const double MCell::getWeight() const {
    return this->MCellWeight;
}

const MVector MCell::getCentreOfGrav() const {
    return this->MCellCOG;
}

const double MCell::getDensity() const {
    return this->MCellDensity;
}

const double MCell::getID() const {
    return this->MCellID;
}

const Material MCell::getMaterial() const {
    return this->MCellMaterial;
}

const std::vector<MVector> MCell::getVertices() const {
    return this->MCellVertices;
}

const double MCell::getCount() const {
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
    std::map<MCell::_MCellType_t, std::vector<std::string>> MCellType_str{
            {MCell::_MCellType_t::TETRAHEDRON, {"t", "tetrahedron"}},
            {MCell::_MCellType_t::PYRAMID,     {"p", "pyramid"}},
            {MCell::_MCellType_t::HEXAHEDRON,  {"h", "hexahedron"}}
    };

    return MCellType_str[this->MCellType];
}

std::ostream &operator<<(std::ostream &os, const MCell &mCell) {
    os << mCell.getType()[1];

    return os;
}

std::ofstream &operator<<(std::ofstream &os, const MCell &mCell) {
    os << mCell.getType()[0];

    return os;
}
