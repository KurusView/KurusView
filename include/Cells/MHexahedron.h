#ifndef INC_3DLIB_MHEXAHEDRON_H
#define INC_3DLIB_MHEXAHEDRON_H


#include "MCell.h"

class MHexahedron : public MCell {
public:

    MHexahedron (std::vector<std::shared_ptr<MVector>> vertices, std::shared_ptr<Material> material, long int id) ;
    MHexahedron() = delete;

    MHexahedron(std::vector<std::shared_ptr<const MVector>> vertices, std::shared_ptr<const Material> material,
                long id);

    ~MHexahedron() = default; // this should call the base destructor regardless

private:
    double calcVolume() const override;

    double calcWeight() const override;

    std::shared_ptr<MVector> calcCentreOfGrav() const override;
};


#endif //INC_3DLIB_MHEXAHEDRON_H
