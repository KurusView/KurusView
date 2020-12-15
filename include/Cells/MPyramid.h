#ifndef INC_3DLIB_MPYRAMID_H
#define INC_3DLIB_MPYRAMID_H


#include "MCell.h"

class MPyramid : public MCell {
public:

    MPyramid (std::vector<MVector> vertices, Material material, const long int id) ;
    MPyramid() = delete;

    ~MPyramid() = default; // this should call the base destructor regardless

    double calcVolume() const override;

    double calcWeight() const override;

    MVector calcCentreOfGrav() const override;
};


#endif //INC_3DLIB_MPYRAMID_H
