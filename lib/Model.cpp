#include <fstream>
#include <iostream>
#include <sstream>
#include <MHexahedron.h>
#include <MPyramid.h>
#include <MTetrahedron.h>
#include "Model.h"
#include "MVector.h"

Model::Model(const std::string &filePath) {
    std::cout << "Creating new model from file" << std::endl;
    loadModel(filePath);
}

void Model::loadModel(const std::string &filePath) {
    // Open the file with an input file stream
    std::ifstream inputFile(filePath);
    // Ensure file is open
    if (!inputFile.is_open()) {
        std::cout << "Could not open file." << std::endl;
        return;
    }

    materials.clear();
    vectors.clear();
    cells.clear();

    // Strings to store the current line and the current token within the line
    std::string line, buf;

    for (int i = 0; i < 2; ++i) {
        // Reset ifstream flags and scroll back to beginning of file
        inputFile.clear();
        inputFile.seekg(0);

        // Loop through all lines in file
        while (std::getline(inputFile, line)) {

            // Only parse 'm' and 'v' in the first pass, then only 'c'
            if (!i && line[0] == 'c' || i && line[0] != 'c' ||
                line.empty()) // This line is a comment and should be ignored or an empty line
                continue; // Skip to the next line


            // Read line into an input string stream, which allows extracting the space separated values
            std::istringstream ss(line);
            // String vector of the space separated values
            std::vector<std::string> tokens;

            // Read all the values into the tokens string vector
            while (ss >> buf) {
                tokens.push_back(buf);
            }

            if (line[0] == 'm') {
                // This line is a material line formatted as follows:
                // 0      1           2             3               4
                /* m int(index) int(density) string(hexColor) string(name) */

                // Insert a shared pointer to a new Material into the provided index
                materials.insert(materials.begin() + std::stoul(tokens[1]),
                                 std::make_shared<Material>(tokens[4], tokens[3],
                                                            std::stoi(tokens[2]),
                                                            std::stoul(tokens[1])));
            } else if (line[0] == 'v') {
                // This line is a vector line formatted as follows:
                // 0      1         2         3         4
                /* v int(index) double(x) double(y) double(z) */

                // Insert a shared pointer to a new MVector into the provided index
                vectors.insert(vectors.begin() + std::stoul(tokens[1]),
                               MVector(std::stod(tokens[2]), std::stod(tokens[3]),
                                       std::stod(tokens[4]), std::stoul(tokens[1])));
            } else if (line[0] == 'c') {
                // This line is a cell line formatted as follows:
                // 0      1            2                3                 4               5         ...
                // c int(index) char(cellType) int(materialIndex) int(vectorIndex) int(vectorIndex) ...

                // Stores shared pointers to all the vectors that the cell is based on
                std::vector<std::shared_ptr<MVector>> cellVectors;

                // Store the cellType as it will be used a lot
                const char cellType = *tokens[2].c_str();

                // Initialize cellVectors to the correct size and pre-populate it with nullptr
                if (cellType == 'h')
                    // Hexahedrons have 8 vertices
                    cellVectors = std::vector<std::shared_ptr<MVector>>(8, nullptr);
                else if (cellType == 'p')
                    // Pyramids have 5 vertices
                    cellVectors = std::vector<std::shared_ptr<MVector>>(5, nullptr);
                else if (cellType == 't')
                    // Tetrahedrons have 4 vertices
                    cellVectors = std::vector<std::shared_ptr<MVector>>(4, nullptr);

                // Populate cellVectors with the vectors this cell is based on
                switch (cellType) {
                    case 'h': // Hexahedron
                        cellVectors[7] = std::make_shared<MVector>(vectors[stoi(tokens[11])]);
                        cellVectors[6] = std::make_shared<MVector>(vectors[stoi(tokens[10])]);
                        cellVectors[5] = std::make_shared<MVector>(vectors[stoi(tokens[9])]);
                    case 'p': // Pyramid
                        cellVectors[4] = std::make_shared<MVector>(vectors[stoi(tokens[8])]);
                    case 't': // Tetrahedron
                        cellVectors[3] = std::make_shared<MVector>(vectors[stoi(tokens[7])]);
                        cellVectors[2] = std::make_shared<MVector>(vectors[stoi(tokens[6])]);
                        cellVectors[1] = std::make_shared<MVector>(vectors[stoi(tokens[5])]);
                        cellVectors[0] = std::make_shared<MVector>(vectors[stoi(tokens[4])]);
                        break;
                    default:
                        break;
                }

                // Insert a shared pointer to a new Cell of the provided type into the provided index
                std::shared_ptr<MCell> tempMCell;
                if (cellType == 'h')
                    tempMCell = std::make_shared<MHexahedron>(cellVectors, materials[stoi(tokens[3])],
                                                              stoi(tokens[1]));
                else if (cellType == 'p')
                    tempMCell = std::make_shared<MPyramid>(cellVectors, materials[stoi(tokens[3])],
                                                           stoi(tokens[1]));
                else if (cellType == 't')
                    tempMCell = std::make_shared<MTetrahedron>(cellVectors, materials[stoi(tokens[3])],
                                                               stoi(tokens[1]));

                cells.insert(cells.begin() + std::stoul(tokens[1]), tempMCell);
            }
        }
    }

    inputFile.close();
}

void Model::saveModel(const std::string &filePath) {
//    Open the file with an output file stream
    std::ofstream outputFile;
    outputFile.open(filePath, std::ofstream::out);

//    Ensure file is open
    if (!outputFile.is_open()) {
        std::cout << "Could not open save file." << std::endl;
        return;
    }

    outputFile << "# Model" << std::endl;

//    Output materials
    outputFile << std::endl << "# Materials" << std::endl;
    for (const auto &material: materials)
        outputFile << *material;

//    Output all vectors
    outputFile << std::endl << "# Vectors" << std::endl;
    for (MVector &vector : vectors)
        outputFile << vector;

//    Output all cells
    outputFile << std::endl << "# Cells" << std::endl;
    for (const auto &cell: cells)
        outputFile << *cell;

    outputFile.close();
}

void Model::displayVertexCount() {
    std::cout << "Number of Vertices: " << vectors.size() << std::endl;
}

void Model::displayCells() {
//    MCell::getCount(); // For global count
    std::cout << "Number of Cells: " << cells.size() << std::endl;
    for (const auto &cell:cells) {
        std::cout << "Cell " << cell->getID() << ": " << cell->getType()[1] << std::endl;
    }
}

MVector Model::calcCentre() {
    MVector centreOfGravity;
    double totalMass = 0;

    for (const auto &cell : cells) {
        totalMass += cell->getWeight();
        centreOfGravity = centreOfGravity + (*(cell->getCentreOfGrav()) * cell->getWeight());
    }

    centreOfGravity = centreOfGravity / totalMass;
    return centreOfGravity;
}

const std::vector<MVector> &Model::getVectors() const {
    return vectors;
}

const std::vector<std::shared_ptr<MCell>> &Model::getCells() const {
    return cells;
}

const std::vector<unsigned int> &Model::getIndices() const {
    return indices;
}
