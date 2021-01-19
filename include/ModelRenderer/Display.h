//
// Created by oabou on 12/12/2020.
//

#ifndef INC_KURUSVIEW_DISPLAY_H
#define INC_KURUSVIEW_DISPLAY_H


#include <string>
#include <SDL.h>
#include "Mouse.h"

/**
 * @brief Display - Responsible for creating an SDL Window with an OpenGL context
 */
class Display {
private:
    SDL_Window *m_window; /**< Main SDL Window */
    SDL_GLContext m_glContext; /**< OpenGL Rendering Context */
    bool m_isClosed; /**< Stores whether the window is currently open or closed */

public:
    /**
     * @brief Display - Creates a window using SDL2
     * @param [in] width - Window width in pixels
     * @param [in] height - Window height in pixels
     * @param [in] title - Window title
     */
    Display(int width, int height, const std::string &title, float &angleX, float &angleY, glm::vec3 &scale);

    /**
     * @brief update - Swaps between the two window buffers
     */
    void update();

    /**
     * @brief clear - Clears the window by filling it with the specified RGBA color
     * @param [in] red - Value of red pixel between 0 and 1
     * @param [in] green - Value of green pixel between 0 and 1
     * @param [in] blue - Value of blue pixel between 0 and 1
     * @param [in] alpha - Value of alpha(transparency) between 0 and 1
     */
    void clear(float red, float green, float blue, float alpha);

    /**
     * @brief isClosed - Getter function that determines whether the window has been closed
     * @return - false if the window is open, true if the window is closed
     */
    bool isClosed() const;

    virtual ~Display();

    InputMouse mouseState;
};


#endif //INC_KURUSVIEW_DISPLAY_H
