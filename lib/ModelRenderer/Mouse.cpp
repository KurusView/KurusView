#include <iostream>
#include "Mouse.h"

bool InputMouse::Initialize() {
    ///--- Last frame information to 0
    m_iPreviousCoordX = 0;
    m_iPreviousCoordY = 0;
    m_uPreviousMouseState = 0;

    ///--- Get the current state of the mouse
    m_uCurrentMouseState = SDL_GetMouseState(&m_iCurrentCoordX, &m_iCurrentCoordY);

    ///--- initialize the wheel to 0 to avoid problems
    m_iWheelX = 0;
    m_iWheelY = 0;
    return true;
}

void InputMouse::Update() {
    ///--- Store the current information to the previous
    m_iPreviousCoordX = m_iCurrentCoordX;
    m_iPreviousCoordY = m_iCurrentCoordY;
    m_uPreviousMouseState = m_uCurrentMouseState;

    ///--- Update the current state of the mouse
    m_uCurrentMouseState = SDL_GetMouseState(&m_iCurrentCoordX, &m_iCurrentCoordY);

    ///--- Set the wheel back to 0
    m_iWheelX = 0;
    m_iWheelY = 0;
}

void InputMouse::ReceiveEvent(SDL_Event &oEvent) {
    switch (oEvent.type) {
        case SDL_MOUSEBUTTONDOWN:
            SDL_SetRelativeMouseMode(SDL_TRUE);
            break;
        case SDL_MOUSEBUTTONUP:
            SDL_SetRelativeMouseMode(SDL_FALSE);
            break;
        case SDL_MOUSEMOTION:
            if (SDL_GetRelativeMouseMode()) {
                m_angleX += oEvent.motion.yrel * 0.01f;
                m_angleY += oEvent.motion.xrel * 0.01f;
                std::cout << "DBG: Catch SDL_MOUSEMOTION event: " << oEvent.motion.xrel << " - " << oEvent.motion.x
                          << std::endl;
            }
            break;
        case SDL_MOUSEWHEEL: {
            m_iWheelX = oEvent.wheel.x;
            m_iWheelX = oEvent.wheel.y;
            m_scale += oEvent.wheel.y * 0.1;

            std::cout << "DBG: Catch SDL_MOUSEWHEEL event: " << oEvent.wheel.y << std::endl;
            break;
        }
    }
}

void InputMouse::GetMousePosition(int &iCoordX, int &iCoordY) const {
    iCoordX = m_iCurrentCoordX;
    iCoordY = m_iCurrentCoordY;
}

void InputMouse::GetMouseDiff(int &iDiffX, int &iDiffY) const {
    iDiffX = m_iCurrentCoordX - m_iPreviousCoordX;
    iDiffY = m_iCurrentCoordY - m_iPreviousCoordY;
}

bool InputMouse::IsButtonTrigered(const Uint32 uButton) const {
    return ((SDL_BUTTON(uButton) & m_uCurrentMouseState) != 0) && ((SDL_BUTTON(uButton) & m_uPreviousMouseState) == 0);
}

bool InputMouse::IsButtonPressed(const Uint32 uButton) const {
    return (SDL_BUTTON(uButton) & m_uCurrentMouseState) != 0;
}

bool InputMouse::IsButtonReleased(const Uint32 uButton) const {
    return ((SDL_BUTTON(uButton) & m_uCurrentMouseState) == 0) && ((SDL_BUTTON(uButton) & m_uPreviousMouseState) != 0);
}

int InputMouse::GetWheelX() const {
    return m_iWheelX;
}

int InputMouse::GetWheelY() const {
    return m_iWheelY;
}

InputMouse::InputMouse(float &mAngleX, float &mAngleY, glm::vec3 &mScale)
        : m_angleX(mAngleX), m_angleY(mAngleY), m_scale(mScale) {

}


