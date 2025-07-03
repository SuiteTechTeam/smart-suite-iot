#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include "Sensor.h"
#include <DHT.h>

class DhtSensor : public Sensor {
private:
    DHT dht;
    float lastTemperature;
    float lastHumidity;

public:
    static const int TEMPERATURE_READ_EVENT_ID = 100;
    static const int HUMIDITY_READ_EVENT_ID = 101;
    static const Event TEMPERATURE_READ_EVENT;
    static const Event HUMIDITY_READ_EVENT;

    /**
     * @brief Constructs a DhtSensor.
     * @param pin The GPIO pin for the DHT sensor.
     * @param dhtType The type of DHT sensor (DHT11, DHT22, etc.).
     * @param eventHandler Optional handler to receive sensor events (default: nullptr).
     */
    DhtSensor(int pin, uint8_t dhtType = DHT11, EventHandler* eventHandler = nullptr);

    /**
     * @brief Initializes the DHT sensor.
     */
    void begin();

    /**
     * @brief Reads temperature and humidity from the sensor.
     * @return True if reading was successful, false otherwise.
     */
    bool readSensor();

    /**
     * @brief Gets the last temperature reading.
     * @return Temperature in Celsius.
     */
    float getTemperature() const;

    /**
     * @brief Gets the last humidity reading.
     * @return Humidity percentage.
     */
    float getHumidity() const;
};

#endif // DHT_SENSOR_H
