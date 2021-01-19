#include <Display.h>
#include <Shader.h>
#include <Mesh.h>
#include "ModelRenderer.h"
#include <glm.hpp>
#include <gtx/transform.hpp>

ModelRenderer::ModelRenderer(Model *model) {
    this->model = model;
    this->width = 800;
    this->height = 800;
    this->title = "Model Renderer";
}

void ModelRenderer::run() {
//    Create an empty, new window with the provided size and title
    Display display(width, height, title);
    Shader shader("../../../res/basicShader");
//    TODO: Load camera

    Mesh mesh(model);

    glm::vec3 m_scale = {30, 30, 30};
    float m_rotate = 1.55;

    glm::mat4 transform;

    float counter = 0.0f;
//    Constantly update the window until the user closes it
    while (!display.isClosed()) {
        float sinCounter = sinf(counter);
        float cosCounter = cosf(counter);

//        Fill the screen with a dark-blue background. RGBA floats between 0-1.
        display.clear(0.0f, 0.15f, 0.3f, 0.9f);
//        TODO: Transform the model
//        TODO: Use shader to draw and transform the mesh
        transform = glm::scale(m_scale) * glm::rotate(cosCounter, glm::vec3(0, 1, 0)) *
                    glm::rotate(sinCounter, glm::vec3(1, 0, 0));
        shader.bind();
        mesh.draw();
        shader.update(transform);
//        Update the window with all the newly drawn pixels
        display.update();
        counter += 0.005f;
    }
}
