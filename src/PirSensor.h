#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

#include "Sensor.h"

class PirSensor : public Sensor {
private:
    bool lastMotionState;

public:
    static const int MOTION_DETECTED_EVENT_ID = 200;
    static const int MOTION_STOPPED_EVENT_ID = 201;
    static const Event MOTION_DETECTED_EVENT;
    static const Event MOTION_STOPPED_EVENT;

    /**
     * @brief Constructs a PirSensor.
     * @param pin The GPIO pin for the PIR sensor.
     * @param eventHandler Optional handler to receive sensor events (default: nullptr).
     */
    PirSensor(int pin, EventHandler* eventHandler = nullptr);

    /**
     * @brief Initializes the PIR sensor.
     */
    void begin();

    /**
     * @brief Reads the current motion state from the sensor.
     * @return True if motion is detected, false otherwise.
     */
    bool readMotion();

    /**
     * @brief Gets the last motion state.
     * @return True if motion was last detected, false otherwise.
     */
    bool getMotionState() const;
};

#endif // PIR_SENSOR_H
