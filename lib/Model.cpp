#include <fstream>
#include <iostream>
#include <sstream>
#include "Model.h"
#include "MVector.h"

Model::Model(const std::string &filePath) {
    std::cout << "Creating new model from file" << std::endl;
    loadModel(filePath);
}

void Model::loadModel(const std::string &filePath) {
//    Open the file with an input file stream
    std::ifstream inputFile(filePath);

//    Ensure file is open
    if (!inputFile.is_open()) {
        std::cout << "Could not open file." << std::endl;
        return;
    }

//    Strings to store the current line and the current token within the line
    std::string line, buf;

//    Read each line in the file into the line string
    while (std::getline(inputFile, line)) {
        if (line[0] == '#' || line.empty()) // This line is a comment and should be ignored or an empty line
            continue; // Skip to the next line

//        Read line into an input string stream, which allows extracting the space separated values
        std::istringstream ss(line);
//        String vector of the space separated values
        std::vector<std::string> tokens;

//        Read all the values into the tokens string vector
        while (ss >> buf) {
            tokens.push_back(buf);
        }

        if (line[0] == 'm') {
//            This line is a material definition
//            Temporary, will be replaced by object instantiation of Material
//          ---------------------------------------------------------------------------------------------------------
            int mindex = std::stoi(tokens[1]);
            int mdensity = std::stoi(tokens[2]);
            int mred = std::stoi(tokens[3].substr(0, 2), nullptr, 16);
            int mgreen = std::stoi(tokens[3].substr(2, 2), nullptr, 16);
            int mblue = std::stoi(tokens[3].substr(4, 2), nullptr, 16);
            std::string mname = tokens[4];
//          ---------------------------------------------------------------------------------------------------------

            std::cout << "Material " << mindex << "-" << mname << "| D:" << mdensity << " | R:" << mred << " G:"
                      << mgreen << " B:" << mblue << std::endl;
        } else if (line[0] == 'v') {
            // Instantiate an object with the provided x, y and z values
            MVector mVector(std::stod(tokens[2]), std::stod(tokens[3]), std::stod(tokens[4]));
            // Insert it into the vectors list
            vectors.insert(vectors.begin() + std::stoi(tokens[1]), mVector);
            // Display added vector
            std::cout << "Vector " << std::stoi(tokens[1])
                      << "| X:" << mVector.getX() << " Y:" << mVector.getY() << " Z:" << mVector.getZ() << std::endl;
        } else if (line[0] == 'c') {
//            This line is a cell definition
//            Temporary, will be replaced by object instantiation of Cell
//          ---------------------------------------------------------------------------------------------------------
            int cindex = std::stoi(tokens[1]);
            char cellType = *tokens[2].c_str();
            int materialIndex = std::stoi(tokens[3]);
            std::string cellVectors;

            switch (cellType) {
                case 'q':
                    cellVectors += tokens[6] + " " + tokens[5] + " " + tokens[4];
                    break;
                case 'h': // Hexahedron
                    cellVectors += tokens[11] + " " + tokens[10] + " " + tokens[9] + " ";
                case 'p': // Pyramid
                    cellVectors += tokens[8] + " ";
                case 't': // Tetrahedron
                    cellVectors += tokens[7] + " " + tokens[6] + " " + tokens[5] + " " + tokens[4];
                    break;
                default:
                    break;
            }
//          ---------------------------------------------------------------------------------------------------------

            std::cout << "Cell " << cindex << "| Type:" << cellType << " Material:" << materialIndex << " Vectors "
                      << cellVectors << std::endl;
        } else {
//            The file has invalid lines or something went wrong
            std::cout << "Typo in file, please verify the file has correct formatting.";
            break;
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

//    Output materials
    outputFile << "# Materials" << std::endl;
    for (unsigned int i = 0; i < materials.size(); ++i) {
        // TODO Get density from material
        int density = 2700;

        // TODO Get colour from material
        int red = 100, green = 100, blue = 100;
        // Combine int color values into a single hexvalue
        char hexCol[16];
        snprintf(hexCol, sizeof hexCol, "%02x%02x%02x", red, green, blue);

        // TODO Get material name
        std::string name = "copper";
        outputFile << 'm' << ' ' << i << ' ' << density << ' ' << hexCol << ' '
                   << name << std::endl << std::endl;
    }

//    Output all vectors
    outputFile << "# Vectors" << std::endl;
    for (unsigned int i = 0; i < vectors.size(); ++i) {
        std::cout << "Print vector" << std::endl;

        outputFile << 'v' << ' ' << i << ' ' << vectors[i].getX() << ' ' << vectors[i].getY() << ' '
                   << vectors[i].getZ() << std::endl << std::endl;
    }

//    Output all cells
    outputFile << "# Cells" << std::endl;
    for (unsigned int i = 0; i < cells.size(); ++i) {
        // TODO Get cell type
        char cellType = 'h';

        // TODO Get material index
        int materialIndex = 1;

        // TODO Get all vector indices
        std::vector<int> vectorIndices = {0, 1, 2, 3, 4, 5, 6, 7};

        // Output cell indicator, index and type
        outputFile << 'c' << ' ' << i << ' ' << cellType << ' ' << materialIndex;
        for (int vectorIndex : vectorIndices) {
            // Print all vectors in the cell
            outputFile << ' ' << vectorIndex;
        }
    }

    outputFile.close();
}

const std::vector<MVector> &Model::getVectors() const {
    return vectors;
}

const std::vector<MCell *> &Model::getCells() const {
    return cells;
}
