#include "DhtSensor.h"

const Event DhtSensor::TEMPERATURE_READ_EVENT = Event(TEMPERATURE_READ_EVENT_ID);
const Event DhtSensor::HUMIDITY_READ_EVENT = Event(HUMIDITY_READ_EVENT_ID);

DhtSensor::DhtSensor(int pin, uint8_t dhtType, EventHandler* eventHandler)
    : Sensor(pin, eventHandler), dht(pin, dhtType), lastTemperature(NAN), lastHumidity(NAN) {}

void DhtSensor::begin() {
    dht.begin();
    
    // Give the sensor time to stabilize (DHT11 needs at least 1 second)
    Serial.println("DHT11: Initializing sensor, please wait...");
    delay(2000);  // 2 second warm-up period
    
    // Try an initial reading to verify sensor is working
    float testTemp = dht.readTemperature();
    float testHum = dht.readHumidity();
    
    if (!isnan(testTemp) && !isnan(testHum)) {
        Serial.println("DHT11: Sensor initialized successfully");
        Serial.println("Initial readings - T:" + String(testTemp) + "°C, H:" + String(testHum) + "%");
    } else {
        Serial.println("DHT11: Warning - Initial sensor test failed. Check connections.");
    }
}

bool DhtSensor::readSensor() {
    // Add a small delay before reading to ensure sensor stability
    delay(100);
    
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    
    // Check if readings are valid (not NaN)
    if (!isnan(temp) && !isnan(hum)) {
        // Additional validation - reasonable ranges for DHT11
        if (temp >= -40 && temp <= 80 && hum >= 0 && hum <= 100) {
            lastTemperature = temp;
            lastHumidity = hum;
            
            // Trigger events for successful readings
            on(TEMPERATURE_READ_EVENT);
            on(HUMIDITY_READ_EVENT);
            
            return true;
        } else {
            Serial.println("DHT11: Values out of range - T:" + String(temp) + "°C, H:" + String(hum) + "%");
        }
    } else {
        Serial.println("DHT11: Failed to read sensor - check wiring and power");
    }
    
    return false;
}

float DhtSensor::getTemperature() const {
    return lastTemperature;
}

float DhtSensor::getHumidity() const {
    return lastHumidity;
}
