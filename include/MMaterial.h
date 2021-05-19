#ifndef INC_KURUSVIEW_MMATERIAL_H
#define INC_KURUSVIEW_MMATERIAL_H

#include <string>
#include <fstream>

// Class for the Material and its properties.
class MMaterial {
private:
    const unsigned int ID; // Object for the material ID.
    std::string name; // Object of the material name.
    std::string colour; // Object for the material colour.
    double density; // Object for the material density.
public:
    /**
     *
     */
    MMaterial();
    /**
     * @brief Material - Sets the propeties of the material in the model.
     * @param name - The name of the material
     * @param colour - Colour of the material
     * @param density - The density of the material
     * @param id  - The id of the material
     */
    MMaterial(std::string name, std::string colour, double density, unsigned int id);
    /*
     * @breif ~Material - Destructor
     */
    ~MMaterial() = default;

    /**
     * @breif
     * @param os
     * @param obj
     * @return
     */
    friend std::ofstream &operator<<(std::ofstream &os, MMaterial &obj);

    unsigned int getId() const;

    /**
     * @brief getName - Gets the name of the material
     * @return name- The name of the material
     */
    std::string getName() const;

    /**
     * @breif setName - Sets the name of the material
     * @param name - The name of the material set
     */
    void setName(const std::string name);

    /**
     * @brief getColour - Gets the colour of the material
     * @return colour - The colour of the material
     */
    std::string getColour() const;

    /**
     * @brief setColour - Sets the colour of the material
     * @param colour  - The colour set for the material
     */
    void setColour(const std::string colour);

    /**
     * @breif getDensity - Gets the density of the material
     * @return density - The density of the material
     */
    double getDensity() const;

    /**
     * @brief setDensity - Sets the density of the material
     * @param density - The value set for the density of the material
     */
    void setDensity(double density);

};

#endif //INC_KURUSVIEW_MMATERIAL_H
