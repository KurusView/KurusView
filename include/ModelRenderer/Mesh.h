//
// Created by oabou on 15/12/2020.
//

#ifndef INC_3DLIB_MESH_H
#define INC_3DLIB_MESH_H

#include <glm.hpp>
#include <glew.h>
#include <string>
#include <MVector.h>
#include <Model.h>

class Mesh {
private:
    enum {
        POSITION_VB,
        INDEX_VB,
        NUM_BUFFERS
    };

    GLuint m_vertexArrayObject{};
    GLuint m_vertexArrayBuffers[NUM_BUFFERS]{};
    unsigned int m_drawCount;

public:
    Mesh(Model &model);

    void draw() const;

    virtual ~Mesh();

};


#endif //INC_3DLIB_MESH_H
