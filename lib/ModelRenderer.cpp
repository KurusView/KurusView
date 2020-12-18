#include <Display.h>
#include <Shader.h>
#include <Mesh.h>
#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(Model *model) {
    this->model = model;
    this->width = 800;
    this->height = 600;
    this->title = "Model Renderer";
}

void ModelRenderer::run() {
//    Create an empty, new window with the provided size and title
    Display display(width, height, title);
    Shader shader("../../res/basicShader");
//    TODO: Load camera

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

        //std::cout << "Mouse X: " << display.mouseState.GetWheelX() <<
        //              ", Y: "    << display.mouseState.GetWheelY() << std::endl;
    }
}
