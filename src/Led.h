#ifndef LED_H
#define LED_H

#include "Actuator.h"

class Led : public Actuator {
private:
    bool state; ///< Current state of the LED (true = ON, false = OFF).

public:
    static const int TOGGLE_LED_COMMAND_ID = 0; ///< Unique ID for toggle command.
    static const int TURN_ON_COMMAND_ID = 1; ///< Unique ID for turn-on command.
    static const int TURN_OFF_COMMAND_ID = 2; ///< Unique ID for turn-off command.
    static const Command TOGGLE_LED_COMMAND; ///< Predefined command to toggle the LED.
    static const Command TURN_ON_COMMAND; ///< Predefined command to turn the LED ON.
    static const Command TURN_OFF_COMMAND; ///< Predefined command to turn the LED OFF.

    /**
     * @brief Constructs an Led actuator.
     * @param pin The GPIO pin for the LED (configured as OUTPUT).
     * @param initialState Initial state of the LED (default: false/OFF).
     * @param commandHandler Optional handler to receive commands (default: nullptr).
     */
    Led(int pin, bool initialState = false, CommandHandler* commandHandler = nullptr);

    /**
     * @brief Handles commands to control the LED state.
     * @param command The command to execute (e.g., TOGGLE_LED_COMMAND).
     */
    void handle(Command command) override;

    /**
     * @brief Gets the current state of the LED.
     * @return True if the LED is ON, false if OFF.
     */
    bool getState() const;

    /**
     * @brief Sets the LED state directly.
     * @param newState The new state (true = ON, false = OFF).
     */
    void setState(bool newState);
};

#endif // LED_H
