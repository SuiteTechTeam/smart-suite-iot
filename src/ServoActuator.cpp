#include "ServoActuator.h"

const Command ServoActuator::MOVE_TO_POSITION_COMMAND = Command(MOVE_TO_POSITION_COMMAND_ID);
const Command ServoActuator::MOVE_TO_0_COMMAND = Command(MOVE_TO_0_COMMAND_ID);
const Command ServoActuator::MOVE_TO_90_COMMAND = Command(MOVE_TO_90_COMMAND_ID);
const Command ServoActuator::MOVE_TO_180_COMMAND = Command(MOVE_TO_180_COMMAND_ID);

ServoActuator::ServoActuator(int pin, int initialPosition, CommandHandler* commandHandler)
    : Actuator(pin, commandHandler), currentPosition(initialPosition), targetPosition(initialPosition) {}

void ServoActuator::begin() {
    servo.attach(pin);
    servo.write(currentPosition);
}

void ServoActuator::handle(Command command) {
    if (command == MOVE_TO_POSITION_COMMAND) {
        moveTo(targetPosition);
    } else if (command == MOVE_TO_0_COMMAND) {
        moveTo(0);
    } else if (command == MOVE_TO_90_COMMAND) {
        moveTo(90);
    } else if (command == MOVE_TO_180_COMMAND) {
        moveTo(180);
    }
    Actuator::handle(command); // Propagate to handler if set
}

void ServoActuator::moveTo(int position) {
    if (position >= 0 && position <= 180) {
        currentPosition = position;
        servo.write(position);
    }
}

void ServoActuator::setTargetPosition(int position) {
    if (position >= 0 && position <= 180) {
        targetPosition = position;
    }
}

int ServoActuator::getCurrentPosition() const {
    return currentPosition;
}

int ServoActuator::getTargetPosition() const {
    return targetPosition;
}
