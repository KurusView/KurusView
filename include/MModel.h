#ifndef INC_KURUSVIEW_MMODEL_H
#define INC_KURUSVIEW_MMODEL_H

#include <vector>
#include <string>
#include "MCell.h"
#include "MMaterial.h"
#include "MVector.h"

/**
 * @brief MModel - This object is responsible for reading and writing model entities to .mod data files
 */
class MModel {
private:
    /** @brief vectors - List of all the vectors in the model */
    std::vector<MVector> vectors;
    /** @brief materials - List of all the materials in the model */
    std::vector<std::shared_ptr<MMaterial>> materials;


public:
    /**
     * @brief MModel - Default Constructor
     */
    MModel() = default;

    /**
     * @brief MModel - Constructor that loads a model from a file using loadModel
     * @param filePath - Path to the data file
     */
    explicit MModel(const std::string &filePath);

    /**
     * @brief ~MModel - Default Destructor
     */
    ~MModel() = default;

    /**
     * @brief loadModel - Loads a model from a VTK style data file
     * @param filePath - Path to the data file
     */
    void loadModel(const std::string &filePath);

    /**
     * @brief saveModel - Saves the currently loaded model to a VTK style data file
     * @param filePath - Path to save the file in
     */
    void saveModel(const std::string &filePath);


    /**
     * @brief displayVertexCount - Calculates the number of vertices and displays them
     */
    void displayVertexCount();

    /**
     * @brief getCellCount - Displays the number of cells and the type of each cell
     */
    size_t getCellCount();

    /**
     * @brief calcCentre - Calculates the centre of the model
     * @return - MVector that points to the centre of the model
     */
    MVector calcCentre();

    /**
     * @brief calcVolume - Calculates the volume of the model
     * @return totalVolume - The total volume of the model
     */
    double calcVolume();

    /**
     * @brief calcWeight - Calculates the weight of the model
     * @return totalWeight - The total weight of the model
     */
    double calcWeight();

    std::string fileType;

    std::string filePath;

    /** @brief cells - List of all the cells in the model */
    std::vector<std::shared_ptr<MCell>> cells;

    std::vector<std::shared_ptr<MMaterial>> getMaterials();

    std::vector<MVector> getVectors();

    std::vector<std::shared_ptr<MCell>> getCells();

    size_t getVectorCount();

    size_t getMaterialCount();
};


#endif //INC_KURUSVIEW_MMODEL_H
