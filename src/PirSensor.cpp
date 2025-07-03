#include "PirSensor.h"
#include <Arduino.h>

const Event PirSensor::MOTION_DETECTED_EVENT = Event(MOTION_DETECTED_EVENT_ID);
const Event PirSensor::MOTION_STOPPED_EVENT = Event(MOTION_STOPPED_EVENT_ID);

PirSensor::PirSensor(int pin, EventHandler* eventHandler)
    : Sensor(pin, eventHandler), lastMotionState(false) {}

void PirSensor::begin() {
    pinMode(pin, INPUT);
}

bool PirSensor::readMotion() {
    bool currentMotion = digitalRead(pin);
    
    // Only trigger events on state change
    if (currentMotion != lastMotionState) {
        lastMotionState = currentMotion;
        
        if (currentMotion) {
            on(MOTION_DETECTED_EVENT);
        } else {
            on(MOTION_STOPPED_EVENT);
        }
    }
    
    return currentMotion;
}

bool PirSensor::getMotionState() const {
    return lastMotionState;
}
