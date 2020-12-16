#ifndef INC_3DLIB_MODELRENDERER_H
#define INC_3DLIB_MODELRENDERER_H


#include "Model.h"

/**
 * @brief ModelRenderer - Visualizes a model using OpenGL
 */
class ModelRenderer {
private:
    Model * model; /**< Points to the model that was passed to ModelRenderer */
    int width, height; /**< Width and Height of window */
    std::string title; /**< Window Title */
public:
    /**
     * @brief ModelRenderer - Constructs a ModelRenderer instance
     * @param [in] model - A loaded model to be displayed
     */
    explicit ModelRenderer(Model *model);

    /**
     * @brief run - Creates a window and visualises the model
     */
    void run();
};


#endif //INC_3DLIB_MODELRENDERER_H
