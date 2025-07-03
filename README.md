# SmartSuite IoT ESP32 Project

![ESP32](https://img.shields.io/badge/ESP32-blue?style=flat&logo=espressif)
![PlatformIO](https://img.shields.io/badge/PlatformIO-orange?style=flat&logo=platformio)
![Arduino](https://img.shields.io/badge/Arduino-Framework-blue?style=flat&logo=arduino)
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B)

Un sistema IoT completo basado en ESP32 que utiliza el **ModestIoT Nano-framework** para monitorear múltiples sensores y controlar actuadores de forma remota mediante MQTT y HTTP.

## 📋 Características Principales

- **Framework Custom**: ModestIoT - Un nano-framework orientado a objetos para ESP32
- **Múltiples Sensores**: DHT11 (temperatura/humedad), PIR (movimiento), MQ2 (gas/humo)
- **Actuadores Diversos**: 5 LEDs de estado, 2 servomotores
- **Conectividad Dual**: WiFi + MQTT + HTTP
- **Arquitectura Event-Driven**: Sistema de eventos y comandos
- **Configuración Flexible**: WiFi, MQTT y HTTP endpoints configurables

## 🔧 Componentes de Hardware

### Sensores
| Sensor | Pin | Descripción |
|--------|-----|-------------|
| DHT11 | GPIO 4 | Sensor de temperatura y humedad |
| PIR | GPIO 13 | Sensor de movimiento por infrarrojos |
| MQ2 | GPIO 34 (ADC) | Sensor de gas y humo |

### Actuadores
| Actuador | Pin | Descripción |
|----------|-----|-------------|
| LED Rojo | GPIO 25 | Indicador de temperatura alta |
| LED Verde | GPIO 26 | Indicador de estado normal |
| LED Naranja | GPIO 27 | Indicador de alerta media |
| LED Azul | GPIO 33 | Indicador de humedad |
| LED Alerta | GPIO 32 | Indicador de emergencia |
| Servo 1 | GPIO 14 (PWM) | Servomotor principal |
| Servo 2 | GPIO 16 (PWM) | Servomotor secundario |

## 🛠️ Instalación y Configuración

### Requisitos Previos

1. **Visual Studio Code** con extensión **PlatformIO IDE**
2. **ESP32 Development Board**
3. **Cable USB** para programación
4. **Componentes electrónicos** (sensores y actuadores listados arriba)

### Instalación de PlatformIO

1. Instala Visual Studio Code desde [aquí](https://code.visualstudio.com/)
2. Abre VS Code y ve a Extensions (Ctrl+Shift+X)
3. Busca e instala "PlatformIO IDE"
4. Reinicia VS Code

### Configuración del Proyecto

1. **Clonar el proyecto:**
   ```bash
   git clone <tu-repositorio>
   cd smart-suite-iot-esp32x32
   ```

2. **Abrir en PlatformIO:**
   - Abre VS Code
   - Ve a PlatformIO Home (icono de casa en la barra lateral)
   - Click en "Open Project"
   - Selecciona la carpeta del proyecto

3. **Conectar el ESP32:**
   - Conecta tu ESP32 al puerto USB
   - Verifica que aparezca en el Device Manager (Windows)

## 🚀 Compilar y Ejecutar

### Usando PlatformIO en VS Code

1. **Compilar el proyecto:**
   - Presiona `Ctrl+Alt+B` o
   - Click en el ícono ✓ en la barra inferior (PlatformIO: Build)

2. **Subir al ESP32:**
   - Presiona `Ctrl+Alt+U` o
   - Click en el ícono → en la barra inferior (PlatformIO: Upload)

3. **Monitor Serie:**
   - Presiona `Ctrl+Alt+S` o
   - Click en el ícono 🔌 en la barra inferior (PlatformIO: Serial Monitor)

### Comandos de Terminal (Alternativos)

```bash
# Compilar
pio run

# Compilar y subir
pio run --target upload

# Monitor serie
pio device monitor

# Limpiar proyecto
pio run --target clean
```

## ⚙️ Configuración del Sistema

### WiFi y Conectividad

Edita en `main.cpp` las credenciales de tu red:

```cpp
// Configuración WiFi
smartSuite.setWiFiCredentials("TU_RED_WIFI", "TU_PASSWORD");

// Configuración MQTT
smartSuite.setMQTTConfig("test.mosquitto.org", 1883, 
                        "smartsuite/sensors/data", 
                        "smartsuite/servo/command", 
                        "smartsuite/alerts");

// Endpoint HTTP
smartSuite.setHTTPEndpoint("https://tu-endpoint.com");
```

### Valores por Defecto

| Configuración | Valor por Defecto |
|---------------|-------------------|
| WiFi SSID | "Las4as.pe" |
| WiFi Password | "L@s4as.pe" |
| MQTT Broker | "test.mosquitto.org" |
| MQTT Port | 1883 |
| Sensor Interval | 2000ms |
| MQTT Interval | 5000ms |

## 📡 Protocolos de Comunicación

### MQTT Topics

- **Datos de Sensores**: `smartsuite/sensors/data`
- **Comandos de Servo**: `smartsuite/servo/command`
- **Alertas**: `smartsuite/alerts`

### Formato de Datos JSON

```json
{
  "temperature": 25.4,
  "humidity": 60.2,
  "motionDetected": false,
  "smokeLevel": 150.0,
  "servoPosition": 90,
  "servo2Position": 45,
  "timestamp": 12345678
}
```

### Comandos MQTT para Servos

```json
{
  "servo": 1,
  "position": 90
}
```

## 🏗️ Arquitectura del Sistema

### ModestIoT Framework

El proyecto utiliza un nano-framework custom con:

- **Event-Driven Architecture**: Sistema de eventos para sensores
- **Command Pattern**: Manejo de comandos para actuadores
- **Inheritance-Based**: Clases base para Device, Sensor, Actuator
- **Modular Design**: Cada componente es independiente y reutilizable

### Jerarquía de Clases

```
Device (Abstract)
├── SmartSuiteDevice (Main)
│
Sensor (Abstract)
├── DhtSensor
├── PirSensor
└── Mq2Sensor

Actuator (Abstract)
├── Led
└── ServoActuator
```

### Interfaces

- **EventHandler**: Para procesar eventos de sensores
- **CommandHandler**: Para ejecutar comandos en actuadores

## 📊 Umbrales y Alertas

### Sensor MQ2 (Gas)
- **Nivel Medio**: 300 PPM
- **Nivel Alto**: 600 PPM

### Eventos del Sistema
- **DHT11**: Temperatura/Humedad leída
- **PIR**: Movimiento detectado/detenido
- **MQ2**: Gas detectado (bajo/medio/alto/despejado)

## 🔍 Debugging y Troubleshooting

### Monitor Serie
El sistema imprime información de debug a 115200 baudios:

```
=== SmartSuite ESP32 Ready ===
Using ModestIoT Nano-framework
Sensors: DHT11, PIR, MQ2
Actuators: 5 LEDs, 2 Servos
Connectivity: WiFi + MQTT + HTTP
===============================
```

### Problemas Comunes

1. **Error de compilación**: Verifica que todas las librerías estén instaladas
2. **No conecta WiFi**: Revisa credenciales en el código
3. **Sensores no responden**: Verifica conexiones de hardware
4. **MQTT no funciona**: Confirma broker y topics

## 📚 Dependencias

Las siguientes librerías se instalan automáticamente via PlatformIO:

```ini
lib_deps = 
    adafruit/DHT sensor library@^1.4.4
    madhephaestus/ESP32Servo@^1.2.1
    knolleary/PubSubClient@^2.8
    bblanchon/ArduinoJson@^6.21.3
```

## 🤝 Contribuir

1. Fork el proyecto
2. Crea una branch para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la branch (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## 📄 Licencia

Este proyecto está bajo la Licencia MIT. Ver el archivo `LICENSE` para más detalles.

## 👨‍💻 Autor

**Diego** - Desarrollo del SmartSuite IoT ESP32 Project

---

### 📞 Soporte

Si encuentras algún problema o tienes preguntas:

1. Revisa la sección de [Troubleshooting](#-debugging-y-troubleshooting)
2. Abre un Issue en el repositorio
3. Consulta la documentación de PlatformIO

**¡Happy Coding! 🚀**
