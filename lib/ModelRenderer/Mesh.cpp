//
// Created by oabou on 15/12/2020.
//

#include "Mesh.h"

Mesh::Mesh(Model *model) {
    this->model = model;
    m_IndexArrayBuffers = new GLuint[model->getCells().size()];
    indexBuffers = std::vector < std::vector < unsigned int >> (model->getCells().size());

    for (unsigned int i = 0; i < model->getCells().size(); ++i) {
        MCell &cell = *model->getCells()[i];
        char cellType = cell.getType()[0][0];
        const std::vector<unsigned int> &ids = cell.getVertexIndices();
        if (cellType == 't')
            // 0 1 2 3 0 1
            indexBuffers[i] = {ids[0], ids[1], ids[2], ids[3], ids[0], ids[1]};
        else if (cellType == 'p')
            // 0 1 2  0 2 3  0 1 4  0 3 4  2 3 4  2 1 4
            indexBuffers[i] = {ids[0], ids[1], ids[2],
                               ids[0], ids[2], ids[3],
                               ids[0], ids[1], ids[4],
                               ids[0], ids[3], ids[4],
                               ids[2], ids[3], ids[4],
                               ids[2], ids[1], ids[4]};
        else if (cellType == 'h')
            // 0 1 2 3  0 1 5 4  1 2 6 5  2 3 7 6  3 0 4 7  4 7 6 5
            indexBuffers[i] = {
                    ids[0], ids[1], ids[2], ids[3],
                    ids[0], ids[1], ids[5], ids[4],
                    ids[1], ids[2], ids[6], ids[5],
                    ids[2], ids[3], ids[7], ids[6],
                    ids[3], ids[0], ids[4], ids[7],
                    ids[4], ids[7], ids[6], ids[5]
            };
    }

    // Generate vertex array object names and bind them
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // Generate buffer object names and bind the position to vertex attributes
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);

    // Create and initialize buffer object data store
    // Set data for Vertex Attributes
    // Use as the source for GL drawing commands
    glBufferData(GL_ARRAY_BUFFER, model->getVectors().size() * sizeof(model->getVectors()[0]),
                 model->getVectors().data(),
                 GL_STATIC_DRAW);

    glGenBuffers(model->getCells().size(), m_IndexArrayBuffers);
    for (unsigned int i = 0; i < model->getCells().size(); ++i) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexArrayBuffers[i]);
        // Set data for Vertex Array Indices
        // Use as the source for GL drawing commands
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffers[i].size() * sizeof(indexBuffers[i][0]),
                     indexBuffers[i].data(), GL_STATIC_DRAW);
    }

    // Enable Vertex Attribute Array 0
    glEnableVertexAttribArray(0);

    /**
     * @brief: Modify the attribute array 0, which was just enabled
     *
     * @internal: define an array of generic vertex attribute data - http://docs.gl/gl4/glVertexAttribPointer
     * @note: stride 0 (packed) was used - this was the cause of bug 65ccba1. Our vertex (MVector) object contains more
     *        than position data (the ID), and possibly shader data in the future.
     *
     */
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(MVector), nullptr);

    // Bind the vertex array object
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    // Delete 1 vector array object
    glDeleteVertexArrays(1, &m_vertexArrayObject);
    free(m_IndexArrayBuffers);
}

void Mesh::draw() const {
    // Bind the vertex array object
    glBindVertexArray(m_vertexArrayObject);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (unsigned int i = 0; i < indexBuffers.size(); ++i) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexArrayBuffers[i]);
        char cellType = model->getCells()[i]->getType()[0][0];
        if (cellType == 't')
            // 0 1 2 3 0 1
            glDrawElements(GL_TRIANGLE_STRIP, indexBuffers[i].size(), GL_UNSIGNED_INT, nullptr);
        else if (cellType == 'p')
            // 0 1 2  0 2 3  0 1 4  0 3 4  2 3 4  2 1 4
            glDrawElements(GL_TRIANGLES, indexBuffers[i].size(), GL_UNSIGNED_INT, nullptr);
        else if (cellType == 'h')
            // 0 1 2 3  1 2 6 5  2 3 7 6  3 0 4 7  4 7 6 5
            glDrawElements(GL_QUADS, indexBuffers[i].size(), GL_UNSIGNED_INT, nullptr);

    }
    // Bind the vertex array object
    glBindVertexArray(0);
}
