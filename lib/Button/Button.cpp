#include "Button.h"

/**--------------------------------------------------------------------------------------
 * Initialization
 *-------------------------------------------------------------------------------------*/

Button::Button()
{
}

Button::Button(uint8_t pinNumber, uint8_t inputMode /* = INPUT_PULLUP */, uint8_t activeState /* = LOW */)
{
    init(pinNumber, inputMode, activeState);
}

/**
 * Default inputMode: INPUT_PULLUP - Default activeState: LOW
 * \param pinNumber The pin number of the button
 * \param inputMode The type of input - INPUT, INPUT_PULLUP, INPUT_PULLDOWN
 * \param buttonActiveState The state that triggers the button - LOW or HIGH
 */
void Button::init(uint8_t pinNumber, uint8_t inputMode /* = INPUT_PULLUP */, uint8_t buttonActiveState /* = LOW */)
{
    pinMode(pinNumber, inputMode);
    pin = pinNumber;
    activeState = buttonActiveState;
}

/**--------------------------------------------------------------------------------------
 * Public Functions
 *-------------------------------------------------------------------------------------*/

bool Button::clicked()
{
    return (state.is(CLICKED) && clickCount == 1);
}

bool Button::doubleClicked()
{
    return (state.is(CLICKED) && clickCount == 2);
}

bool Button::tripleClicked()
{
    return (state.is(CLICKED) && clickCount == 3);
}

bool Button::pressed()
{
    return (state.is(PRESSED) && state.justEntered());
}

bool Button::released()
{
    return (state.is(RELEASED));
}

bool Button::longPressed()
{
    return (state.is(LONG_PRESSED) && state.justEntered());
}

bool Button::longPressReleased()
{
    return (state.is(RELEASED) && lastPressTime == 0);
}

bool Button::held()
{
    return (state.is(PRESSED));
}

/**
 * Checks the button state and updates the state machine
 * Must only be called once per loop else button states may be missed
 */
void Button::check()
{
    bool pressedState = (digitalRead(pin) == activeState);

    switch (state.get())
    {
    case (IDLE):
        if (pressedState)
        {
            lastDebounceTime = millis();
            state.set(DEBOUNCING);
        }
        break;

    case (DEBOUNCING):
        if (millis() - lastDebounceTime > debounceDelay)
        {
            if (pressedState)
            {
                lastPressTime = millis();
                state.set(PRESSED);
            }
            else
            {
                state.set(IDLE);
            }
        }
        break;

    case (PRESSED):
        if (!pressedState)
        {
            if (millis() - lastClickTime > clickDelay)
            {
                clickCount = 1;
                lastClickTime = millis();
            }
            else
            {
                clickCount++;
            }
            state.set(CLICKED);
        }
        else if (millis() - lastPressTime > longPressDelay)
        {
            state.set(LONG_PRESSED);
        }
        break;

    case (CLICKED):
        state.set(RELEASED);
        break;

    case (RELEASED):
        state.set(IDLE);
        break;

    case (LONG_PRESSED):
        if (!pressedState)
        {
            lastPressTime = 0;
            state.set(RELEASED);
        }
        break;
    }
}
