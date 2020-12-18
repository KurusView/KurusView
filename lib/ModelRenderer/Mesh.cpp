//
// Created by oabou on 15/12/2020.
//

#include "Mesh.h"

Mesh::Mesh(Model &model) {
    m_drawCount = model.getIndices().size();

    // Generate vertex array object names and bind them
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // Generate buffer object names and bind the position to vertex attributes
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    // Create and initialize buffer object data store
    // Set data for Vertex Attributes
    // Use as the source for GL drawing commands
    glBufferData(GL_ARRAY_BUFFER, model.getVectors().size() * sizeof(model.getVectors()[0]), model.getVectors().data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
    // Set data for Vertex Array Indices
    // Use as the source for GL drawing commands
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.getIndices().size() * sizeof(model.getIndices()[0]),
                 model.getIndices().data(), GL_STATIC_DRAW);

    // Enable Vertex Attribute Array 0
    glEnableVertexAttribArray(0);

    /**
     * @brief: Modify the attribute array 0, which was just enabled
     *
     * @internal: define an array of generic vertex attribute data - http://docs.gl/gl4/glVertexAttribPointer
     * @note: stride 0 (packed) was used - this was the cause of bug 65ccba1. Our vertex (MVector) object contains more
     *        than position data (the ID), and possibly shader data in the future.
     *
     * @TODO: Should normalized be false?
     */
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(MVector), nullptr);

    // Bind the vertex array object
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    // Delete 1 vector array object
    glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::draw() const {
    // Bind the vertex array object
    glBindVertexArray(m_vertexArrayObject);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLE_STRIP, m_drawCount, GL_UNSIGNED_INT, nullptr);
    // Bind the vertex array object
    glBindVertexArray(0);
}
