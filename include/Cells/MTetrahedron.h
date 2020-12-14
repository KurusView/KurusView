#ifndef INC_3DLIB_MTETRAHEDRON_H
#define INC_3DLIB_MTETRAHEDRON_H


#include "MCell.h"

class MTetrahedron : MCell {
public:
    MTetrahedron (std::vector<MVector> vertices, Material material, const long int id) ;
    ~MTetrahedron() = default; // this should call the base destructor regardless

    double calcVolume() const override;

    double calcWeight() const override;

    MVector calcCentreOfGrav() const override;
};


#endif //INC_3DLIB_MTETRAHEDRON_H
