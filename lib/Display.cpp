//
// Created by oabou on 12/12/2020.
//

#include "Display.h"
#include <glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string &title) {
//    Initialize the video subsystem in the SDL Library, this also automatically initializes the events subsystem
    SDL_Init(SDL_INIT_VIDEO);

//    2^8=256 shades of each color
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
//    Set the minimum number of bits for the frame buffer size to the sum of RGBA
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
//    Enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

//    Create a new centered window that can be used with an OpenGL context
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                                SDL_WINDOW_OPENGL);
//    Create an OpenGL rendering context to use with the window
    m_glContext = SDL_GL_CreateContext(m_window);

//    After creating an OpenGL context, glew can be initialized
    GLenum status = glewInit();

//    Ensure glew initialization was successful
    if (status != GLEW_OK)
        std::cerr << "Glew failed to initialize!" << std::endl;
//    Now that the window is open and OpenGL is running, indicate that in m_isClosed
    m_isClosed = false;

//    Only renders faces that are front-facing towards the viewer
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

Display::~Display() {
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Display::update() {
//    Update the window with the rendered pixels
    SDL_GL_SwapWindow(m_window);

    SDL_Event event;
//    Polls are currently pending events
    while (SDL_PollEvent(&event))
//        If the window was closed, set m_isClosed to indicate that
        if (event.type == SDL_QUIT)
            m_isClosed = true;
}

void Display::clear(float red, float green, float blue, float alpha) {
//    Specify the clear values to be used when the color buffers are cleared
    glClearColor(red, green, blue, alpha);
//    Clear the buffers currently enabled for color writing
    glClear(GL_COLOR_BUFFER_BIT);
}

bool Display::isClosed() const {
    return m_isClosed;
}
