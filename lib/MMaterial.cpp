#include <string>
#include "MMaterial.h"

 unsigned int MMaterial::getId() const {
    return ID;
}

std::string MMaterial::getName() const {
    return name;
}

void MMaterial::setName(const std::string name) {
    this->name = name;
}

std::string MMaterial::getColour() const {
    return colour;
}

void MMaterial::setColour(const std::string colour) {
    this->colour = colour;
}

double MMaterial::getDensity() const {
    return density;
}

void MMaterial::setDensity(double density) {
    this->density = density;
}

MMaterial::MMaterial() : ID(-1), name("undefined"), colour("undefined"), density(-1){}

MMaterial::MMaterial(std::string name, std::string colour, double density, unsigned int id) : ID(id),
                                                                                             name(name),
                                                                                             colour(colour),
                                                                                             density(density) {
}

std::ofstream &operator<<(std::ofstream &os, MMaterial &obj) {
    os << "m " << obj.getId()
       << " " << obj.getDensity()
       << " " << obj.getColour()
       << " " << obj.getName() << std::endl;

    return os;
}
