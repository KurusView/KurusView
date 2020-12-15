#ifndef INC_3DLIB_MTETRAHEDRON_H
#define INC_3DLIB_MTETRAHEDRON_H


#include "MCell.h"

class MTetrahedron : public MCell {
public:

    MTetrahedron (std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material, long int id) ;
    MTetrahedron() = delete;

    MTetrahedron(std::vector<std::shared_ptr<const MVector>> vertices, std::shared_ptr<const Material> material,
                 long id);

    ~MTetrahedron() = default; // this should call the base destructor regardless

private:
    double calcVolume() const override;

    double calcWeight() const override;

    std::shared_ptr<MVector> calcCentreOfGrav() const override;
};


#endif //INC_3DLIB_MTETRAHEDRON_H
