#ifndef MQ2_SENSOR_H
#define MQ2_SENSOR_H

#include "Sensor.h"

class Mq2Sensor : public Sensor {
private:
    float lastPpmValue;
    float mediumThreshold;
    float highThreshold;

public:
    static const int GAS_DETECTED_EVENT_ID = 300;
    static const int GAS_MEDIUM_EVENT_ID = 301;
    static const int GAS_HIGH_EVENT_ID = 302;
    static const int GAS_CLEAR_EVENT_ID = 303;
    static const Event GAS_DETECTED_EVENT;
    static const Event GAS_MEDIUM_EVENT;
    static const Event GAS_HIGH_EVENT;
    static const Event GAS_CLEAR_EVENT;

    /**
     * @brief Constructs an Mq2Sensor.
     * @param pin The analog GPIO pin for the MQ2 sensor.
     * @param mediumThreshold PPM threshold for medium gas level (default: 300).
     * @param highThreshold PPM threshold for high gas level (default: 600).
     * @param eventHandler Optional handler to receive sensor events (default: nullptr).
     */
    Mq2Sensor(int pin, float mediumThreshold = 300.0, float highThreshold = 600.0, EventHandler* eventHandler = nullptr);

    /**
     * @brief Initializes the MQ2 sensor.
     */
    void begin();

    /**
     * @brief Reads the gas level from the sensor.
     * @return Gas level in estimated PPM.
     */
    float readGasLevel();

    /**
     * @brief Gets the last gas level reading.
     * @return Gas level in PPM.
     */
    float getGasLevel() const;

    /**
     * @brief Sets the thresholds for gas level detection.
     * @param medium Medium threshold in PPM.
     * @param high High threshold in PPM.
     */
    void setThresholds(float medium, float high);
};

#endif // MQ2_SENSOR_H
