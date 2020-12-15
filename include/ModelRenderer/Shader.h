//
// Created by oabou on 15/12/2020.
//

#ifndef INC_3DLIB_SHADER_H
#define INC_3DLIB_SHADER_H

#include <string>
#include <glew.h>

class Shader {
private:
    // Using Fragment Shader and Vertex Shader and not Geometry Shader
    // Can use enumeration to set which one refers to which
    static const unsigned int NUM_SHADERS = 2;

    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS]{};

    static GLuint CreateShader(const std::string &text, GLenum shaderType);

    static std::string LoadShader(const std::string &fileName);

    static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string &errorMessage);

public:
    explicit Shader(const std::string &fileName);

    /**
     * @brief bind - Binds this shader class to the GPU
     */
    void bind() const;

    virtual ~Shader();
};


#endif //INC_3DLIB_SHADER_H
