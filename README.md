# ESP8266 IoT Robot Documentation

## Overview

This project implements an ESP8266-based IoT robot that integrates environmental sensing, servo motor control, and cloud connectivity. The robot reads temperature and humidity data from a DHT11 sensor, controls a servo motor based on MQTT commands, and communicates with Alibaba Cloud IoT Platform.

## Features

- **Environmental Monitoring**: Real-time temperature and humidity sensing using DHT11 sensor
- **Remote Control**: Servo motor control via MQTT commands from cloud platform
- **IoT Connectivity**: Secure MQTT communication with Alibaba Cloud IoT Hub
- **Real-time Data**: Periodic sensor data transmission to cloud platform
- **Wireless Communication**: WiFi connectivity for remote operation

## Hardware Requirements

### Components
- ESP8266 development board (NodeMCU, Wemos D1 Mini, etc.)
- DHT11 temperature and humidity sensor
- SG90 servo motor (or compatible)
- Breadboard and jumper wires
- USB cable for programming

### Pin Configuration
| Component | ESP8266 Pin | Physical Pin |
|-----------|-------------|--------------|
| DHT11 Data | GPIO 5 (D1) | Pin 5 |
| Servo Signal | GPIO 4 (D2) | Pin 4 |
| Servo VCC | 3.3V | 3.3V |
| Servo GND | GND | GND |
| DHT11 VCC | 3.3V | 3.3V |
| DHT11 GND | GND | GND |

## Software Dependencies

### Arduino Libraries
```cpp
#include <ArduinoMqttClient.h>  // MQTT communication
#include <DFRobot_DHT11.h>      // DHT11 sensor interface
#include <ArduinoJson.h>        // JSON parsing and serialization
#include <ESP8266WiFi.h>        // WiFi connectivity
#include <Servo.h>              // Servo motor control
```

### Installation
1. Install Arduino IDE
2. Add ESP8266 board support via Board Manager
3. Install required libraries through Library Manager:
   - ArduinoMqttClient by Arduino
   - DFRobot_DHT11 by DFRobot
   - ArduinoJson by Benoit Blanchon
   - ESP8266WiFi (included with ESP8266 core)
   - Servo (Arduino built-in)

## Configuration

### WiFi Settings
```cpp
char ssid[] = "Your_WiFi_SSID";     // Replace with your network name
char pass[] = "Your_WiFi_Password"; // Replace with your network password
```

### MQTT Configuration
```cpp
const char broker[] = "your-iot-hub.mqtt.iothub.aliyuncs.com";
int port = 1883;
const char inTopic[] = "/sys/ProductKey/DeviceName/thing/service/property/set";
const char outTopic[] = "/sys/ProductKey/DeviceName/thing/event/property/post";
```

**Note**: Replace the MQTT credentials and topics with your specific Alibaba Cloud IoT Platform configuration.

## Quick Start

1. **Hardware Setup**: Wire the components according to the pin configuration table
2. **Software Setup**: 
   - Install Arduino IDE and required libraries
   - Update WiFi and MQTT credentials in the code
   - Upload the code to your ESP8266
3. **Testing**: 
   - Open Serial Monitor (9600 baud) to view connection status
   - Send commands through your IoT platform to test servo control
   - Monitor temperature and humidity data transmission

## Project Structure

```
esp8266_robot/
├── esp8266_robot.ino      # Main Arduino sketch
├── README.md              # This documentation
├── API_REFERENCE.md       # Detailed API documentation
├── FUNCTION_REFERENCE.md  # Function documentation
├── HARDWARE_SETUP.md      # Hardware wiring guide
└── EXAMPLES.md           # Usage examples
```

## Data Format

### Outbound Data (Sensor Readings)
```json
{
  "params": {
    "temp": 25.6,
    "humi": 60.2
  },
  "version": "1.0.0"
}
```

### Inbound Data (Servo Control)
```json
{
  "items": {
    "led": {
      "value": 1,
      "time": 1689847086996
    }
  }
}
```

## License

This project is open source. Please refer to the license file for details.

## Support

For issues and questions, please check the documentation files or create an issue in the project repository.