#include "Mq2Sensor.h"
#include <Arduino.h>

const Event Mq2Sensor::GAS_DETECTED_EVENT = Event(GAS_DETECTED_EVENT_ID);
const Event Mq2Sensor::GAS_MEDIUM_EVENT = Event(GAS_MEDIUM_EVENT_ID);
const Event Mq2Sensor::GAS_HIGH_EVENT = Event(GAS_HIGH_EVENT_ID);
const Event Mq2Sensor::GAS_CLEAR_EVENT = Event(GAS_CLEAR_EVENT_ID);

Mq2Sensor::Mq2Sensor(int pin, float mediumThreshold, float highThreshold, EventHandler* eventHandler)
    : Sensor(pin, eventHandler), lastPpmValue(0.0), mediumThreshold(mediumThreshold), highThreshold(highThreshold) {}

void Mq2Sensor::begin() {
    // Analog pins don't need pinMode setup
}

float Mq2Sensor::readGasLevel() {
    int analogValue = analogRead(pin);
    float ppm = (analogValue / 4095.0) * 1000.0; // Convert to estimated PPM
    
    // Store the last reading
    float previousPpm = lastPpmValue;
    lastPpmValue = ppm;
    
    // Generate events based on thresholds
    if (ppm >= highThreshold && previousPpm < highThreshold) {
        on(GAS_HIGH_EVENT);
    } else if (ppm >= mediumThreshold && previousPpm < mediumThreshold) {
        on(GAS_MEDIUM_EVENT);
    } else if (ppm > 0 && previousPpm == 0) {
        on(GAS_DETECTED_EVENT);
    } else if (ppm < mediumThreshold && previousPpm >= mediumThreshold) {
        on(GAS_CLEAR_EVENT);
    }
    
    return ppm;
}

float Mq2Sensor::getGasLevel() const {
    return lastPpmValue;
}

void Mq2Sensor::setThresholds(float medium, float high) {
    mediumThreshold = medium;
    highThreshold = high;
}
