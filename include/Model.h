#ifndef INC_3DLIB_MODEL_H
#define INC_3DLIB_MODEL_H

#include <vector>
#include <string>
#include "MCell.h"
#include "Material.h"
#include "MVector.h"

/**
 * @brief Model - This object is responsible for reading and writing model entities to VTK style data files
 */
class Model
{
private:
    /** @brief vertices - List of all the vectors in the model */
    std::vector<MVector> vertices;
    /** @brief cells - List of all the cells in the model */
    std::vector<MCell> cells;
    /** @brief materials - List of all the materials in the model */
    std::vector<Material> materials;

public:
    /**
     * @brief Model - Default Constructor
     */
    Model() = default;

    /**
     * @brief Model - Constructor that loads a model from a file using loadModel
     * @param [in] filePath - Path to the data file
     */
    explicit Model(std::string &filePath);

    /**
     * @brief ~Model - Default Destructor
     */
    ~Model() = default;

    /**
     * @brief loadModel - Loads a model from a VTK style data file
     * @param [in] filePath - Path to the data file
     */
    void loadModel(std::string &filePath);

    /**
     * @brief saveModel - Saves the currently loaded model to a VTK style data file
     * @param [in] filePath - Path to save the file in
     */
    void saveModel(std::string &filePath);

    /**
     * @brief displayVertexCount - Calculates the number of vertices and displays them
     * @return - Vertex Count
     */
    int displayVertexCount();

    /**
     * @brief displayCells - Displays the number of cells and the types of each cell
     */
    void displayCells();

    /**
     * @brief calcCentre - Calculates the centre of the model
     * @return - MVector that points to the centre of the model
     */
    MVector calcCentre(void);
};


#endif //INC_3DLIB_MODEL_H
