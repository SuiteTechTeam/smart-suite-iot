#include "SmartSuiteDevice.h"

// Static instance for MQTT callback
SmartSuiteDevice* SmartSuiteDevice::instance = nullptr;

SmartSuiteDevice::SmartSuiteDevice()
    : dhtSensor(DHT_PIN, DHT11, this),
      pirSensor(PIR_PIN, this),
      mq2Sensor(MQ2_PIN, 300.0, 600.0, this),
      ledRed(LED_RED_PIN, false, this),
      ledGreen(LED_GREEN_PIN, false, this),
      ledOrange(LED_ORANGE_PIN, false, this),
      ledBlue(LED_BLUE_PIN, false, this),
      ledAlert(LED_ALERT_PIN, false, this),
      servo1(SERVO1_PIN, 0, this),
      servo2(SERVO2_PIN, 0, this),
      mqttClient(espClient),
      wifiSSID("Las4as.pe"),
      wifiPassword("L@s4as.pe"),
      mqttBroker("192.168.0.237"),
      mqttTopicData("smartsuite/sensors/data"),
      mqttTopicServoCommand("smartsuite/servo/command"),
      mqttTopicAlerts("smartsuite/alerts"),
      httpEndpoint("https://jsonplaceholder.typicode.com/posts"),
      clientId("SmartSuite_ESP32"),
      mqttPort(1883),
      lastSensorRead(0),
      lastDataSent(0),
      sensorInterval(2000),  // Increased to 2 seconds for DHT11 stability
      mqttInterval(5000) {
    
    instance = this;
}

void SmartSuiteDevice::begin() {
    Serial.begin(115200);
    
    // Initialize sensors
    dhtSensor.begin();
    pirSensor.begin();
    mq2Sensor.begin();
    
    // Initialize actuators  
    servo1.begin();
    servo2.begin();
    
    // Set initial servo positions
    servo1.handle(ServoActuator::MOVE_TO_0_COMMAND);
    servo2.handle(ServoActuator::MOVE_TO_0_COMMAND);
    
    // Setup WiFi and MQTT
    setupWiFi();
    mqttClient.setServer(mqttBroker, mqttPort);
    mqttClient.setCallback(mqttCallback);
    
    Serial.println("SmartSuite ESP32 initialized using ModestIoT framework");
    Serial.println("Sensors: DHT11, PIR, MQ2");
    Serial.println("Actuators: 5 LEDs, 2 Servos");
    Serial.println("Connectivity: WiFi + MQTT");
}

void SmartSuiteDevice::update() {
    // Maintain MQTT connection
    if (!mqttClient.connected()) {
        reconnectMQTT();
    }
    mqttClient.loop();
    
    unsigned long currentTime = millis();
    
    // Read sensors periodically
    if (currentTime - lastSensorRead >= sensorInterval) {
        lastSensorRead = currentTime;
        
        // Read all sensors with retry mechanism
        bool dhtSuccess = dhtSensor.readSensor();
        if (!dhtSuccess) {
            // Wait a bit and try one more time
            delay(500);
            dhtSuccess = dhtSensor.readSensor();
        }
        
        pirSensor.readMotion();
        mq2Sensor.readGasLevel();
        
        // Process sensor data
        processTemperatureHumidity();
        processMotionDetection();
        processGasDetection();
    }
    
    // Send MQTT data periodically
    if (currentTime - lastDataSent >= mqttInterval) {
        lastDataSent = currentTime;
        sendSensorData();
        sendSensorDataHTTP(); // También enviar por HTTP
    }
    
    // Small delay for system stability
    delay(100);
}

void SmartSuiteDevice::on(Event event) {
    if (event == DhtSensor::TEMPERATURE_READ_EVENT || event == DhtSensor::HUMIDITY_READ_EVENT) {
        processTemperatureHumidity();
    } else if (event == PirSensor::MOTION_DETECTED_EVENT) {
        ledBlue.handle(Led::TURN_ON_COMMAND);
        sendAlert("motion", "medium", "Motion detected in the area");
    } else if (event == PirSensor::MOTION_STOPPED_EVENT) {
        ledBlue.handle(Led::TURN_OFF_COMMAND);
    } else if (event == Mq2Sensor::GAS_MEDIUM_EVENT) {
        processGasDetection();
        sendAlert("smoke", "medium", ("Gas level detected: " + String(mq2Sensor.getGasLevel()) + " ppm").c_str());
    } else if (event == Mq2Sensor::GAS_HIGH_EVENT) {
        processGasDetection();
        sendAlert("smoke", "high", ("High gas level detected: " + String(mq2Sensor.getGasLevel()) + " ppm").c_str());
    } else if (event == Mq2Sensor::GAS_CLEAR_EVENT) {
        ledAlert.handle(Led::TURN_OFF_COMMAND);
        servo2.handle(ServoActuator::MOVE_TO_0_COMMAND);
    }
}

void SmartSuiteDevice::handle(Command command) {
    // Handle actuator feedback or logging
    Serial.print("Command executed: ");
    Serial.println(command.id);
}

void SmartSuiteDevice::setWiFiCredentials(const char* ssid, const char* password) {
    wifiSSID = ssid;
    wifiPassword = password;
}

void SmartSuiteDevice::setMQTTConfig(const char* broker, int port, const char* topicData, 
                                     const char* topicServoCommand, const char* topicAlerts) {
    mqttBroker = broker;
    mqttPort = port;
    mqttTopicData = topicData;
    mqttTopicServoCommand = topicServoCommand;
    mqttTopicAlerts = topicAlerts;
}

void SmartSuiteDevice::setHTTPEndpoint(const char* endpoint) {
    httpEndpoint = endpoint;
}

void SmartSuiteDevice::setupWiFi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifiSSID);

    WiFi.begin(wifiSSID, wifiPassword);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void SmartSuiteDevice::reconnectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        
        if (mqttClient.connect(clientId)) {
            Serial.println("connected");
            mqttClient.subscribe(mqttTopicServoCommand);
            Serial.println("Subscribed to: " + String(mqttTopicServoCommand));
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" trying again in 5 seconds");
            delay(5000);
        }
    }
}

void SmartSuiteDevice::handleMQTTMessage(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);

    // Convert payload to string
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Message: " + message);

    // Process servo commands
    if (String(topic) == mqttTopicServoCommand) {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, message);
        
        if (doc.containsKey("position")) {
            int position = doc["position"];
            int servoNumber = doc.containsKey("servo") ? doc["servo"] : 1;
            
            if (servoNumber == 1 && position >= 0 && position <= 180) {
                servo1.setTargetPosition(position);
                servo1.handle(ServoActuator::MOVE_TO_POSITION_COMMAND);
                Serial.println("Servo 1 moved to position: " + String(position));
            } else if (servoNumber == 2 && position >= 0 && position <= 180) {
                servo2.setTargetPosition(position);
                servo2.handle(ServoActuator::MOVE_TO_POSITION_COMMAND);
                Serial.println("Servo 2 moved to position: " + String(position));
            }
        }
    }
}

void SmartSuiteDevice::sendSensorData() {
    DynamicJsonDocument doc(1024);
    
    float temp = dhtSensor.getTemperature();
    float hum = dhtSensor.getHumidity();
    
    // Handle sensor failures with error values (-999)
    float tempToSend = !isnan(temp) ? temp : -999.0;  // -999 indicates error
    float humToSend = !isnan(hum) ? hum : -999.0;     // -999 indicates error
    
    doc["temperature"] = tempToSend;
    doc["humidity"] = humToSend;
    doc["motionDetected"] = pirSensor.getMotionState();
    doc["smokeLevel"] = mq2Sensor.getGasLevel();
    doc["servoPosition"] = servo1.getCurrentPosition();
    doc["servo2Position"] = servo2.getCurrentPosition();
    doc["timestamp"] = millis();
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    Serial.println("=== SENDING MQTT DATA ===");
    Serial.println("Topic: " + String(mqttTopicData));
    Serial.println("Data: " + jsonString);
    
    if (mqttClient.publish(mqttTopicData, jsonString.c_str())) {
        Serial.println("✅ Data sent successfully");
    } else {
        Serial.println("❌ Error sending data - MQTT state: " + String(mqttClient.state()));
    }
    Serial.println("============================");
}

void SmartSuiteDevice::sendAlert(const char* type, const char* severity, const char* message) {
    DynamicJsonDocument doc(512);
    
    doc["type"] = type;
    doc["severity"] = severity;
    doc["message"] = message;
    doc["timestamp"] = millis();
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    if (mqttClient.publish(mqttTopicAlerts, jsonString.c_str())) {
        Serial.println("Alert sent: " + jsonString);
    } else {
        Serial.println("Error sending alert");
    }
}

void SmartSuiteDevice::processTemperatureHumidity() {
    float temp = dhtSensor.getTemperature();
    float hum = dhtSensor.getHumidity();
    
    if (!isnan(temp) && !isnan(hum)) {
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.print(" °C\tHumidity: ");
        Serial.print(hum);
        Serial.println(" %");
        
        // Control LEDs based on climate conditions
        if (temp < 18 || hum < 40) {
            ledRed.handle(Led::TURN_ON_COMMAND);
            ledGreen.handle(Led::TURN_OFF_COMMAND);
            ledOrange.handle(Led::TURN_OFF_COMMAND);
        } else if (temp > 28 || hum > 70) {
            ledRed.handle(Led::TURN_OFF_COMMAND);
            ledGreen.handle(Led::TURN_OFF_COMMAND);
            ledOrange.handle(Led::TURN_ON_COMMAND);
        } else {
            ledRed.handle(Led::TURN_OFF_COMMAND);
            ledGreen.handle(Led::TURN_ON_COMMAND);
            ledOrange.handle(Led::TURN_OFF_COMMAND);
        }
        
        // Control Servo 1 for high temperature
        if (temp > 32) {
            if (servo1.getCurrentPosition() != 90) {
                servo1.handle(ServoActuator::MOVE_TO_90_COMMAND);
                sendAlert("temperature", "high", ("High temperature detected: " + String(temp) + "°C").c_str());
            }
        } else {
            if (servo1.getCurrentPosition() != 0) {
                servo1.handle(ServoActuator::MOVE_TO_0_COMMAND);
            }
        }
    } else {
        // More detailed error information
        Serial.println("DHT11 sensor error - attempting to retry reading...");
        
        // Try to read the sensor directly one more time
        if (dhtSensor.readSensor()) {
            Serial.println("DHT11 retry successful!");
        } else {
            Serial.println("DHT11 retry failed - check sensor connections, power, and timing");
            Serial.println("Troubleshooting tips:");
            Serial.println("- Ensure DHT11 is connected to pin 4");
            Serial.println("- Check 3.3V/5V power supply");
            Serial.println("- Verify pull-up resistor (10kΩ) on data line");
            Serial.println("- Sensor may need more time between readings");
        }
    }
}

void SmartSuiteDevice::processMotionDetection() {
    if (pirSensor.getMotionState()) {
        sendAlert("motion", "medium", "Movement detected in the area");
    }
}

void SmartSuiteDevice::processGasDetection() {
    static bool gasAlertActive = false;  // Variable estática para controlar el estado
    static unsigned long lastServoAction = 0;  // Tiempo de la última acción del servo
    const unsigned long servoDebounceTime = 5000;  // 5 segundos entre movimientos
    
    float ppm = mq2Sensor.getGasLevel();
    
    Serial.print("MQ2 Estimated smoke level: ");
    Serial.print(ppm);
    Serial.println(" ppm");
    
    unsigned long currentTime = millis();
    
    if (ppm > 300) {
        // Solo activar si no está ya activo o ha pasado suficiente tiempo
        if (!gasAlertActive || (currentTime - lastServoAction > servoDebounceTime)) {
            ledAlert.handle(Led::TURN_ON_COMMAND);
            
            // Solo mover el servo si no está ya en la posición correcta
            if (servo2.getCurrentPosition() != 90) {
                servo2.handle(ServoActuator::MOVE_TO_90_COMMAND);
                lastServoAction = currentTime;
                gasAlertActive = true;
                
                String severity = ppm > 600 ? "high" : "medium";
                sendAlert("smoke", severity.c_str(), ("Smoke level detected: " + String(ppm) + " ppm").c_str());
                
                Serial.println("🚨 Servo2 moved to 90° - Gas detection mode activated");
            }
        }
    } else {
        // Solo desactivar si estaba activo y ha pasado suficiente tiempo
        if (gasAlertActive && (currentTime - lastServoAction > servoDebounceTime)) {
            ledAlert.handle(Led::TURN_OFF_COMMAND);
            
            // Solo mover el servo si no está ya en la posición correcta
            if (servo2.getCurrentPosition() != 0) {
                servo2.handle(ServoActuator::MOVE_TO_0_COMMAND);
                lastServoAction = currentTime;
                gasAlertActive = false;
                
                Serial.println("✅ Servo2 moved to 0° - Gas cleared");
            }
        }
    }
}

void SmartSuiteDevice::sendSensorDataHTTP() {
    // Only send HTTP data if WiFi is connected
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("❌ WiFi not connected - skipping HTTP data send");
        return;
    }
    
    DynamicJsonDocument doc(1024);
    
    float temp = dhtSensor.getTemperature();
    float hum = dhtSensor.getHumidity();
    
    // Handle sensor failures with error values (-999)
    float tempToSend = !isnan(temp) ? temp : -999.0;  // -999 indicates error
    float humToSend = !isnan(hum) ? hum : -999.0;     // -999 indicates error
    
    doc["temperature"] = tempToSend;
    doc["humidity"] = humToSend;
    doc["motionDetected"] = pirSensor.getMotionState();
    doc["smokeLevel"] = mq2Sensor.getGasLevel();
    doc["servoPosition"] = servo1.getCurrentPosition();
    doc["servo2Position"] = servo2.getCurrentPosition();
    doc["timestamp"] = millis();
    doc["deviceId"] = clientId;
    doc["source"] = "smartsuite-esp32";
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    Serial.println("=== SENDING HTTP DATA ===");
    Serial.println("Endpoint: " + String(httpEndpoint));
    Serial.println("Data: " + jsonString);
    
    httpClient.begin(httpEndpoint);
    httpClient.addHeader("Content-Type", "application/json");
    httpClient.addHeader("User-Agent", "SmartSuite-ESP32/1.0");
    
    int httpResponseCode = httpClient.POST(jsonString);
    
    if (httpResponseCode > 0) {
        String response = httpClient.getString();
        Serial.println("✅ HTTP Response code: " + String(httpResponseCode));
        Serial.println("📡 Response: " + response);
    } else {
        Serial.println("❌ HTTP Error code: " + String(httpResponseCode));
        Serial.println("🔧 Check network connection and endpoint URL");
    }
    
    httpClient.end();
    Serial.println("============================");
}

void SmartSuiteDevice::mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (instance != nullptr) {
        instance->handleMQTTMessage(topic, payload, length);
    }
}
