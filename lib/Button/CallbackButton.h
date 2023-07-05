#pragma once

#include <Button.h>

typedef void (*func_t)(void);

class CallbackButton : public Button
{
    using Button::Button; // inherit constructors

private:
    func_t clickedCallback = nullptr;
    func_t doubleClickedCallback = nullptr;
    func_t tripleClickedCallback = nullptr;
    func_t pressedCallback = nullptr;
    func_t releasedCallback = nullptr;
    func_t longPressedCallback = nullptr;
    func_t longPressReleasedCallback = nullptr;
    func_t heldCallback = nullptr;

public:
    void setClickedCallback(func_t funcCallback) { clickedCallback = funcCallback; }
    void setDoubleClickedCallback(func_t funcCallback) { doubleClickedCallback = funcCallback; }
    void setTripleClickedCallback(func_t funcCallback) { tripleClickedCallback = funcCallback; }
    void setPressedCallback(func_t funcCallback) { pressedCallback = funcCallback; }
    void setReleasedCallback(func_t funcCallback) { releasedCallback = funcCallback; }
    void setLongPressedCallback(func_t funcCallback) { longPressedCallback = funcCallback; }
    void setLongPressReleasedCallback(func_t funcCallback) { longPressReleasedCallback = funcCallback; }
    void setHeldCallback(func_t funcCallback) { heldCallback = funcCallback; }

    void run()
    {
        check();

        if (clickedCallback != nullptr && clicked())
            clickedCallback();
        if (doubleClickedCallback != nullptr && doubleClicked())
            doubleClickedCallback();
        if (tripleClickedCallback != nullptr && tripleClicked())
            tripleClickedCallback();
        if (pressedCallback != nullptr && pressed())
            pressedCallback();
        if (releasedCallback != nullptr && released())
            releasedCallback();
        if (longPressedCallback != nullptr && longPressed())
            longPressedCallback();
        if (longPressReleasedCallback != nullptr && longPressReleased())
            longPressReleasedCallback();
        if (heldCallback != nullptr && held())
            heldCallback();
    }
};