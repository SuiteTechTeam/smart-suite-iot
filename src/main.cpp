#include "ModestIoT.h"

// Create the main device instance
SmartSuiteDevice smartSuite;

void setup() {
    // Initialize the SmartSuite device
    smartSuite.begin();
    
    // WiFi and MQTT configuration (these are the same defaults but shown for customization)
    smartSuite.setWiFiCredentials("Las4as.pe", "L@s4as.pe");
    smartSuite.setMQTTConfig("test.mosquitto.org", 1883, 
                            "smartsuite/sensors/data", 
                            "smartsuite/servo/command", 
                            "smartsuite/alerts");
    
    // HTTP endpoint configuration
    smartSuite.setHTTPEndpoint("https://smart-suite-iot.free.beeceptor.com");
    
    Serial.println("=== SmartSuite ESP32 Ready ===");
    Serial.println("Using ModestIoT Nano-framework");
    Serial.println("Sensors: DHT11, PIR, MQ2");
    Serial.println("Actuators: 5 LEDs, 2 Servos");
    Serial.println("Connectivity: WiFi + MQTT + HTTP");
    Serial.println("===============================");
}

void loop() {
    // Main device update loop - handles all sensors, actuators, and connectivity
    smartSuite.update();
}