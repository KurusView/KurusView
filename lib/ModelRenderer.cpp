#include <Display.h>
#include <Shader.h>
#include <Mesh.h>
#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(const Model &model) {
    this->model = model;
    this->width = 800;
    this->height = 600;
    this->title = "Model Renderer";
}

void ModelRenderer::run() {
//    Create an empty, new window with the provided size and title
    Display display(width, height, title);
//    TODO: Load shader
    Shader shader("../../res/basicShader");
//    TODO: Load camera
//    TODO: Load mesh
    std::vector<MVector> testMVects = {MVector(-0.5, 0.5, 0),
                                       MVector(0, -0.5, 0),
                                       MVector(0.5, 0.5, 0)};
    Mesh mesh(model);
//    Constantly update the window until the user closes it
    while (!display.isClosed()) {
//        Fill the screen with a dark-blue background. RGBA floats between 0-1.
        display.clear(0.0f, 0.15f, 0.3f, 0.9f);
//        TODO: Transform the model
//        TODO: Use shader to draw and transform the mesh
        shader.bind();
        mesh.draw();
//        Update the window with all the newly drawn pixels
        display.update();
    }
}
