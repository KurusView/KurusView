#ifndef INC_3DLIB_MATERIAL_H
#define INC_3DLIB_MATERIAL_H

#include <string>

// Class for the Material and its properties.
class Material {
private:
    const unsigned int ID; // Object for the material ID.
    std::string name; // Object of the material name.
    std::string colour; // Object for the material colour.
    double density; // Object for the material density.
public:
    Material();
    Material(std::string name, std::string colour, double density, unsigned int id);

    ~Material() = default;

    unsigned int getId() const;

    std::string getName() const;

    void setName(const std::string name);

    std::string getColour() const;

    void setColour(const std::string colour);

    double getDensity() const;

    void setDensity(double density);

};

#endif //INC_3DLIB_MATERIAL_H
