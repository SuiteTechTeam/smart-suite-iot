#ifndef SERVO_ACTUATOR_H
#define SERVO_ACTUATOR_H

#include "Actuator.h"
#include <ESP32Servo.h>

class ServoActuator : public Actuator {
private:
    Servo servo;
    int currentPosition;
    int targetPosition;

public:
    static const int MOVE_TO_POSITION_COMMAND_ID = 10;
    static const int MOVE_TO_0_COMMAND_ID = 11;
    static const int MOVE_TO_90_COMMAND_ID = 12;
    static const int MOVE_TO_180_COMMAND_ID = 13;
    static const Command MOVE_TO_POSITION_COMMAND;
    static const Command MOVE_TO_0_COMMAND;
    static const Command MOVE_TO_90_COMMAND;
    static const Command MOVE_TO_180_COMMAND;

    /**
     * @brief Constructs a ServoActuator.
     * @param pin The GPIO pin for the servo (PWM capable).
     * @param initialPosition Initial position of the servo (0-180 degrees, default: 0).
     * @param commandHandler Optional handler to receive commands (default: nullptr).
     */
    ServoActuator(int pin, int initialPosition = 0, CommandHandler* commandHandler = nullptr);

    /**
     * @brief Initializes the servo actuator.
     */
    void begin();

    /**
     * @brief Handles commands to control the servo position.
     * @param command The command to execute.
     */
    void handle(Command command) override;

    /**
     * @brief Moves the servo to a specific position.
     * @param position Target position (0-180 degrees).
     */
    void moveTo(int position);

    /**
     * @brief Sets the target position for the servo (used with MOVE_TO_POSITION_COMMAND).
     * @param position Target position (0-180 degrees).
     */
    void setTargetPosition(int position);

    /**
     * @brief Gets the current position of the servo.
     * @return Current position in degrees.
     */
    int getCurrentPosition() const;

    /**
     * @brief Gets the target position of the servo.
     * @return Target position in degrees.
     */
    int getTargetPosition() const;
};

#endif // SERVO_ACTUATOR_H
