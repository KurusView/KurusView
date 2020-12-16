#include "MCell.h"

#include <map>
#include <utility>

// cell instance count
unsigned long int MCell::MCellInstanceCount = 0;

// Member Initializer Lists should be in declaration order
MCell::MCell(std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material, const long int id) :
        MCellID(id),
        MCellVertices(std::move(vertices)), // https://stackoverflow.com/a/41874953
        MCellMaterial(std::move(material)),
        MCellType(MCellType_TypeDef::NONE) {

    MCellInstanceCount++;
    //MCellDensity = material.getDensity();
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

std::shared_ptr<MVector> MCell::getCentreOfGrav() const {
    return this->MCellCOG;
}

double MCell::getDensity() const {
    return this->MCellDensity;
}

double MCell::getID() const {
    return this->MCellID;
}

std::shared_ptr<Material> MCell::getMaterial() const {
    return this->MCellMaterial;
}

std::vector<std::shared_ptr<MVector>> MCell::getVertices() const {
    return this->MCellVertices;
}

double MCell::getCount() {
    return MCell::MCellInstanceCount;
}


void MCell::setMaterial(std::shared_ptr<Material> material) {
    this->MCellMaterial = material;
}

void MCell::setVertices(std::vector<std::shared_ptr<MVector>> vertices) {
    //this->MCellVertices = std::make_shared<MVector>(vertices);
    this->MCellVertices = vertices;

}


std::vector<std::string> MCell::getType() const {

    // TODO cant be constexpr, look for Compile Time Evaluated alternative
    // https://stackoverflow.com/questions/16490835/how-to-build-a-compile-time-key-value-store
    std::map<MCell::MCellType_TypeDef, std::vector<std::string>> MCellType_str{
            {MCell::MCellType_TypeDef::TETRAHEDRON, {"t", "Tetrahedron"}},
            {MCell::MCellType_TypeDef::PYRAMID,     {"p", "Pyramid"}},
            {MCell::MCellType_TypeDef::HEXAHEDRON,  {"h", "Hexahedron"}}
    };

    return MCellType_str[this->MCellType];
}

// inline
std::ostream &operator<<(std::ostream &os, const MCell &mCell) {
    os << mCell.getType()[1];

    return os;
}

// inline?
std::ostream &operator<<(std::ostream &os, const MCell &mCell) {

    Material material = *mCell.getMaterial();
    MVector COG = *mCell.getCentreOfGrav();

    os << "MCell type: " << mCell.getType()[1] << "Unique ID: " << mCell.getID() << std::endl
       << "\t Volume: " << mCell.getVolume() << " Weight: " << mCell.getWeight();
    // << " Centre Of Gravity: " << COG << std::endl  //TODO - requires ostream on MVector
    // << " Material: " << material <<std::endl;      //TODO - requires ostream on Material

    // Print vertex IDs
    os << "\t Vertexes: [";
    for (auto &it : mCell.getVertices()) {
        os << it->getID() << ", ";
    }

    // move cursor back and overwrite pending ", "
    os << '\b' << '\b' << "]" << std::endl;

    // Print actual MVectors?

    // Avoid unused variable compiler warnings for now
    (void) material;
    (void) COG;

    return os;
}
