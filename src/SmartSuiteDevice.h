#ifndef SMART_SUITE_DEVICE_H
#define SMART_SUITE_DEVICE_H

#include "Device.h"
#include "DhtSensor.h"
#include "PirSensor.h"
#include "Mq2Sensor.h"
#include "Led.h"
#include "ServoActuator.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

class SmartSuiteDevice : public Device {
private:
    // Sensors
    DhtSensor dhtSensor;
    PirSensor pirSensor;
    Mq2Sensor mq2Sensor;
    
    // Actuators
    Led ledRed;
    Led ledGreen;
    Led ledOrange;
    Led ledBlue;
    Led ledAlert;
    ServoActuator servo1;
    ServoActuator servo2;
    
    // WiFi and MQTT
    WiFiClient espClient;
    PubSubClient mqttClient;
    HTTPClient httpClient;
    
    // Configuration
    const char* wifiSSID;
    const char* wifiPassword;
    const char* mqttBroker;
    const char* mqttTopicData;
    const char* mqttTopicServoCommand;
    const char* mqttTopicAlerts;
    const char* httpEndpoint;
    const char* clientId;
    int mqttPort;
    
    // Timing
    unsigned long lastSensorRead;
    unsigned long lastDataSent;
    unsigned long sensorInterval;
    unsigned long mqttInterval;

public:
    // Pin definitions
    static const int PIR_PIN = 13;
    static const int DHT_PIN = 4;
    static const int SERVO1_PIN = 14;
    static const int SERVO2_PIN = 16;
    static const int MQ2_PIN = 34;
    static const int LED_RED_PIN = 25;
    static const int LED_GREEN_PIN = 26;
    static const int LED_ORANGE_PIN = 27;
    static const int LED_BLUE_PIN = 33;
    static const int LED_ALERT_PIN = 32;

    /**
     * @brief Constructs a SmartSuiteDevice with default configuration.
     */
    SmartSuiteDevice();

    /**
     * @brief Initializes the device (sensors, actuators, WiFi, MQTT).
     */
    void begin();

    /**
     * @brief Main update loop for the device.
     */
    void update();

    /**
     * @brief Handles events from sensors.
     * @param event The event to process.
     */
    void on(Event event) override;

    /**
     * @brief Handles commands for actuators.
     * @param command The command to execute.
     */
    void handle(Command command) override;

    /**
     * @brief Sets WiFi credentials.
     * @param ssid WiFi network name.
     * @param password WiFi password.
     */
    void setWiFiCredentials(const char* ssid, const char* password);

    /**
     * @brief Sets MQTT broker configuration.
     * @param broker MQTT broker address.
     * @param port MQTT broker port.
     * @param topicData Topic for sensor data.
     * @param topicServoCommand Topic for servo commands.
     * @param topicAlerts Topic for alerts.
     */
    void setMQTTConfig(const char* broker, int port, const char* topicData, 
                       const char* topicServoCommand, const char* topicAlerts);

    /**
     * @brief Sets HTTP endpoint for data transmission.
     * @param endpoint HTTP endpoint URL.
     */
    void setHTTPEndpoint(const char* endpoint);

private:
    void setupWiFi();
    void reconnectMQTT();
    void handleMQTTMessage(char* topic, byte* payload, unsigned int length);
    void sendSensorData();
    void sendSensorDataHTTP();
    void sendAlert(const char* type, const char* severity, const char* message);
    void processTemperatureHumidity();
    void processMotionDetection();
    void processGasDetection();
    
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    static SmartSuiteDevice* instance;
};

#endif // SMART_SUITE_DEVICE_H
