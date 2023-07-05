#pragma once

#include <Arduino.h>

class Button
{
private:
    /**--------------------------------------------------------------------------------------
     * State Machine
     *-------------------------------------------------------------------------------------*/
    enum ButtonStateEnum
    {
        IDLE,
        DEBOUNCING,
        PRESSED,
        CLICKED,
        RELEASED,
        LONG_PRESSED,
    };

    class ButtonState
    {
    private:
        ButtonStateEnum currentState = IDLE;
        bool enteredState = true;

    public:
        bool is(ButtonStateEnum state) { return (currentState == state); }
        ButtonStateEnum get() { return currentState; }
        void set(ButtonStateEnum newState)
        {
            if (currentState != newState)
            {
                enteredState = true;
                currentState = newState;
            }
        }

        bool justEntered()
        {
            if (enteredState)
            {
                enteredState = false;
                return true;
            }
            return false;
        }
    };

private:
    /**--------------------------------------------------------------------------------------
     * Private Variables
     *-------------------------------------------------------------------------------------*/
    ButtonState state;

    bool activeState;
    uint8_t pin;

    uint16_t lastDebounceTime = 0;
    uint16_t lastPressTime = 0;
    uint16_t lastClickTime = 0;
    uint16_t clickCount = 0;

    uint16_t debounceDelay = 20;
    uint16_t clickDelay = 400;
    uint16_t longPressDelay = 1000;

public:
    /**--------------------------------------------------------------------------------------
     * Public Functions
     *-------------------------------------------------------------------------------------*/
    Button();
    Button(uint8_t pinNumber, uint8_t inputMode = INPUT_PULLUP, uint8_t activeState = LOW);

    void init(uint8_t pinNumber, uint8_t inputMode = INPUT_PULLUP, uint8_t activeState = LOW);

    void setDebounceDelay(uint16_t debounceDelayMs) { debounceDelay = debounceDelayMs; }
    void setClickDelay(uint16_t clickDelayMs) { clickDelay = clickDelayMs; }
    void setLongPressDelay(uint16_t longPressDelayMs) { longPressDelay = longPressDelayMs; }

    bool clicked();
    bool doubleClicked();
    bool tripleClicked();
    bool pressed();
    bool released();
    bool longPressed();
    bool longPressReleased();
    bool held();

    void check();
};