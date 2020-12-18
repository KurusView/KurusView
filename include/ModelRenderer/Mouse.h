#ifndef INC_3DLIB_MOUSE_H
#define INC_3DLIB_MOUSE_H

#include "SDL.h"
#include "SDL_mouse.h"

class InputMouse
{
public:
    ///--- Constructor/Destructor
    InputMouse() = default;
    ~InputMouse() = default;

    ///--- Object control
    bool Initialize();
    void Update();
    void Release();

    ///--- To receive SDL_MouseWheelEvent
    void ReceiveEvent(SDL_Event& oEvent);

    ///--- Mouse position fucntions
    void GetMousePosition(int& iCoordX, int& iCoordY) const;
    void GetMouseDiff(int& iDiffX, int& iDiffY) const;

    ///--- Mouse button states
    bool IsButtonTrigered(const Uint32 uButton) const;
    bool IsButtonPressed(const Uint32 uButton) const;
    bool IsButtonReleased(const Uint32 uButton) const;

    ///--- Wheel state
    int GetWheelX() const;
    int GetWheelY() const;
private:
    /// Information about the state of the mouse
    int m_iCurrentCoordX;
    int m_iCurrentCoordY;
    Uint32 m_uCurrentMouseState;

    int m_iPreviousCoordX;
    int m_iPreviousCoordY;
    Uint32 m_uPreviousMouseState;

    ///information about the state of thw wheel
    Sint32 m_iWheelX;
    Sint32 m_iWheelY;
};

#endif //INC_3DLIB_MOUSE_H
