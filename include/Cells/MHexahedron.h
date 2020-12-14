#ifndef INC_3DLIB_MHEXAHEDRON_H
#define INC_3DLIB_MHEXAHEDRON_H


#include "MCell.h"

class MHexahedron : public MCell {
public:
    MHexahedron (std::vector<MVector> vertices, Material material, const long int id) ;
    ~MHexahedron() = default; // this should call the base destructor regardless

    double calcVolume() const override;

    double calcWeight() const override;

    MVector calcCentreOfGrav() const override;
};


#endif //INC_3DLIB_MHEXAHEDRON_H
