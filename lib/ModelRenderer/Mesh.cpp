//
// Created by oabou on 15/12/2020.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<MVector> vertices) {

    // Generate vertex array object names and bind them
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // Generate buffer object names and bind the position to vertex attributes
    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    // Create and initialize buffer object data store
    // Set data for Vertex Attributes
    // Use as the source for GL drawing commands
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    // Enable Vertex Attribute Array 0
    glEnableVertexAttribArray(0);

    // Modify the attribute array 0, which was just enabled
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);

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
    // Render triangles starting from index 0
    glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
    // Bind the vertex array object
    glBindVertexArray(0);
}
