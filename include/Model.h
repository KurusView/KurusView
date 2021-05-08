#ifndef INC_KURUSVIEW_MODEL_H
#define INC_KURUSVIEW_MODEL_H

#include <vector>
#include <string>
#include "MCell.h"
#include "Material.h"
#include "MVector.h"
#include <vtkSmartPointer.h>
#include <vtkAlgorithm.h>

/**
 * @brief Model - This object is responsible for reading and writing model entities to VTK style data files
 */
class Model {
private:
    /** @brief vectors - List of all the vectors in the model */
    std::vector<MVector> vectors;
    /** @brief cells - List of all the cells in the model */
    std::vector<std::shared_ptr<MCell>> cells;
    /** @brief materials - List of all the materials in the model */
    std::vector<std::shared_ptr<Material>> materials;
    /** @brief vtkModel - Unstructured grid containing all the cells of the model */
    vtkSmartPointer<vtkAlgorithm> vtkModel;

public:
    /**
     * @brief Model - Default Constructor
     */
    Model() = default;

    /**
     * @brief Model - Constructor that loads a model from a file using loadModel
     * @param [in] filePath - Path to the data file
     */
    explicit Model(const std::string &filePath);

    /**
     * @brief ~Model - Default Destructor
     */
    ~Model() = default;

    /**
     * @brief loadModel - Loads a model from a VTK style data file
     * @param [in] filePath - Path to the data file
     */
    void loadModel(const std::string &filePath);

    /**
     * @brief saveModel - Saves the currently loaded model to a VTK style data file
     * @param [in] filePath - Path to save the file in
     */
    void saveModel(const std::string &filePath);


    /**
     * @brief displayVertexCount - Calculates the number of vertices and displays them
     */
    void displayVertexCount();

    /**
     * @brief displayCells - Displays the number of cells and the type of each cell
     */
    void displayCells();

    /**
     * @brief calcCentre - Calculates the centre of the model
     * @return - MVector that points to the centre of the model
     */
    MVector calcCentre();

    /**
     * @brief buildVTKModel - Builds model from cells and vectors loaded from a proprietary model file
     */
    void buildVTKModel();

    /**
     * @brief getVTKModel - Gets the private built vtk model
     * @return - vtkUnstructuredGrid containing the model
     */
    vtkSmartPointer<vtkAlgorithm> getVTKModel();

    void loadSTLModel(const std::string &filePath);
};


#endif //INC_KURUSVIEW_MODEL_H
