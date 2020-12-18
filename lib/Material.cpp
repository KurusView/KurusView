#include <string>
#include "Material.h"

 unsigned int Material::getId() const {
    return ID;
}

std::string Material::getName() const {
    return name;
}

void Material::setName(const std::string name) {
    this->name = name;
}

std::string Material::getColour() const {
    return colour;
}

void Material::setColour(const std::string colour) {
    this->colour = colour;
}

double Material::getDensity() const {
    return density;
}

void Material::setDensity(double density) {
    this->density = density;
}

Material::Material() : ID(-1), name("undefined"), colour("undefined"), density(-1){}

Material::Material(std::string name, std::string colour, double density, unsigned int id) : ID(id),
                                                                                            name(name),
                                                                                            colour(colour),
                                                                                            density(density) {
}

std::ofstream &operator<<(std::ofstream &os, Material &obj) {
    os << "m " << obj.getId()
       << " " << obj.getDensity()
       << " " << obj.getColour()
       << " " << obj.getName() << std::endl;

    return os;
}