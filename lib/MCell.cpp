#include "MCell.h"
MCell::MCell(std::vector<MVector> vertices, Material material, const long int id) : MCellID(id),
                                                                                    MCellType(_MCellType_t::NONE) {
    MCellInstanceCount++;
    // when instantiating derived class, call this constructor to update global cell counter and id
    // https://stackoverflow.com/a/12045286
}

MCell::~MCell() {
    MCellInstanceCount--;
}
