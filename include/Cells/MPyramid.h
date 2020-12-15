#ifndef INC_3DLIB_MPYRAMID_H
#define INC_3DLIB_MPYRAMID_H


#include "MCell.h"

class MPyramid : public MCell {
public:

    MPyramid (std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material, long int id);
    MPyramid() = delete;

    MPyramid(std::vector<std::shared_ptr<const MVector>> vertices, std::shared_ptr<const Material> material, long id);

    ~MPyramid() = default; // this should call the base destructor regardless

private:
    double calcVolume() const override;

    double calcWeight() const override;

    std::shared_ptr<MVector> calcCentreOfGrav() const override;
};


#endif //INC_3DLIB_MPYRAMID_H
