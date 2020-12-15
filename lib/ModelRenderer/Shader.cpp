//
// Created by oabou on 15/12/2020.
//

#include <iostream>
#include <fstream>
#include "Shader.h"

Shader::Shader(const std::string &fileName) {
    // Create the OpenGL Program and Vertex & Fragment Shader
    m_program = glCreateProgram();
    m_shaders[0] = CreateShader(LoadShader(fileName + ".vert"), GL_VERTEX_SHADER);;
    m_shaders[1] = CreateShader(LoadShader(fileName + ".frag"), GL_FRAGMENT_SHADER);

    // Link both shaders to the program
    for (unsigned int m_shader : m_shaders)
        glAttachShader(m_program, m_shader);

    // Associate attribute index 0 with the position attribute
    glBindAttribLocation(m_program, 0, "position");

    // Link the program object and check for errors
    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program linking failed: ");

    // Validate the program object and check for errors
    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");
}

Shader::~Shader() {
    // Detach and delete both shaders
    for (unsigned int m_shader : m_shaders) {
        glDetachShader(m_program, m_shader);
        glDeleteShader(m_shader);
    }

    // Delete the program
    glDeleteProgram(m_program);
}

void Shader::bind() const {
    glUseProgram(m_program);
}

GLuint Shader::CreateShader(const std::string &text, GLenum shaderType) {
    // Create the shader object
    GLuint shader = glCreateShader(shaderType);

    if (shader == 0)
        std::cerr << "Error: Shader creation failed!" << std::endl;

    // Stores the shader text as a char *
    const GLchar *shaderSourceStrings[1];
    GLint shaderSourceStringLengths[1];

    shaderSourceStrings[0] = text.c_str();
    shaderSourceStringLengths[0] = text.length();

    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
    glCompileShader(shader);
    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

    return shader;
}

std::string Shader::LoadShader(const std::string &fileName) {
    std::ifstream file;
    file.open(fileName.c_str());

    std::string output;
    std::string line;

    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    } else {
        std::cerr << "Unable to load shader: " << fileName << std::endl;
    }
    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string &errorMessage) {
    GLint success = 0;
    GLchar error[1024] = {0};

    if (isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if (success == GL_FALSE) {
        if (isProgram)
            glGetProgramInfoLog(shader, sizeof(error), nullptr, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), nullptr, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}
