# API Reference

This document provides comprehensive documentation for all APIs, communication protocols, and external interfaces used in the ESP8266 IoT Robot project.

## Table of Contents

- [MQTT API](#mqtt-api)
  - [Connection Configuration](#connection-configuration)
  - [Topics](#topics)
  - [Message Formats](#message-formats)
  - [Quality of Service](#quality-of-service)
- [Alibaba Cloud IoT Platform Integration](#alibaba-cloud-iot-platform-integration)
- [WiFi API](#wifi-api)
- [Sensor APIs](#sensor-apis)
- [Servo Control API](#servo-control-api)
- [Error Codes and Handling](#error-codes-and-handling)

## MQTT API

The ESP8266 robot communicates with Alibaba Cloud IoT Platform using the MQTT (Message Queuing Telemetry Transport) protocol.

### Connection Configuration

#### Broker Connection
```cpp
const char broker[] = "iot-06z00cxx1p87j6t.mqtt.iothub.aliyuncs.com";
int port = 1883;  // Standard MQTT port (non-SSL)
```

#### Authentication
```cpp
// Device credentials for Alibaba Cloud IoT Platform
mqttClient.setId("k0t5suseNtK.esp8266_dev|securemode=2,signmethod=hmacsha256,timestamp=1740661520959|");
mqttClient.setUsernamePassword(
  "esp8266_dev&k0t5suseNtK",  // Username format: DeviceName&ProductKey
  "52fd0e66b4e62198dae622c867189ee85c2b549fb69915b0a44db117ad39a50a"  // Device secret (HMAC-SHA256)
);
```

#### Connection Parameters
- **Client ID**: `k0t5suseNtK.esp8266_dev|securemode=2,signmethod=hmacsha256,timestamp=1740661520959|`
- **Username**: `esp8266_dev&k0t5suseNtK`
- **Password**: HMAC-SHA256 encrypted device secret
- **Keep Alive**: Default (60 seconds)
- **Clean Session**: True (default)

### Topics

#### Inbound Topic (Command Reception)
```cpp
const char inTopic[] = "/sys/k0t5suseNtK/esp8266_dev/thing/service/property/set";
```

**Purpose**: Receives servo control commands from IoT platform
**Direction**: Cloud → Device
**QoS Level**: 1 (At least once delivery)
**Subscription**: Automatic during setup

#### Outbound Topic (Data Transmission)
```cpp
const char outTopic[] = "/sys/k0t5suseNtK/esp8266_dev/thing/event/property/post";
```

**Purpose**: Sends sensor data to IoT platform
**Direction**: Device → Cloud
**QoS Level**: 1 (At least once delivery)
**Publishing Interval**: 10 seconds

### Message Formats

#### Outbound Messages (Sensor Data)

**Topic**: `/sys/k0t5suseNtK/esp8266_dev/thing/event/property/post`

**Payload Structure**:
```json
{
  "params": {
    "temp": 25.6,    // Temperature in Celsius (float)
    "humi": 60.2     // Humidity percentage (float)
  },
  "version": "1.0.0"  // Protocol version
}
```

**Example Payload**:
```json
{
  "params": {
    "temp": 23.8,
    "humi": 65.4
  },
  "version": "1.0.0"
}
```

**Data Types**:
- `temp`: Float, range -40.0 to 80.0°C (DHT11 specification)
- `humi`: Float, range 5.0% to 95.0% RH (DHT11 specification)
- `version`: String, API version identifier

#### Inbound Messages (Servo Control)

**Topic**: `/sys/k0t5suseNtK/esp8266_dev/thing/service/property/set`

**Payload Structure**:
```json
{
  "deviceType": "CustomCategory",
  "iotId": "fwJnX0wS4z4MbrzkyRlt000000",
  "requestId": "1689847083906",
  "checkFailedData": {},
  "productKey": "k0t5suseNtK",
  "gmtCreate": 1689847087015,
  "deviceName": "esp8266_dev",
  "items": {
    "led": {
      "value": 1,                    // Control value: 0 or 1
      "time": 1689847086996         // Timestamp
    }
  }
}
```

**Control Commands**:
- `value: 0` → Servo moves to 0° (OFF position)
- `value: 1` → Servo moves to 90° (ON position)

**Response Behavior**:
```cpp
if(value == 0) {
    angle = 0;    // Move to OFF position
    Serial.println("off");
} else {
    angle = 90;   // Move to ON position
    Serial.println("on");
}
myServo.write(angle);
```

### Quality of Service

#### QoS Levels Used
- **QoS 0**: At most once delivery (not used in this implementation)
- **QoS 1**: At least once delivery (used for all messages)
- **QoS 2**: Exactly once delivery (not used in this implementation)

#### Message Properties
```cpp
bool retained = false;    // Messages are not retained
int qos = 1;             // QoS level 1 for reliability
bool dup = false;        // Duplicate flag
```

#### Subscription QoS
```cpp
int subscribeQos = 1;    // Subscribe with QoS 1
mqttClient.subscribe(inTopic, subscribeQos);
```

## Alibaba Cloud IoT Platform Integration

### Platform Specifications
- **Product Key**: `k0t5suseNtK`
- **Device Name**: `esp8266_dev`
- **Device Type**: `CustomCategory`
- **Security Mode**: 2 (HMAC-SHA256)
- **Sign Method**: `hmacsha256`

### Topic Structure Pattern
```
/sys/{ProductKey}/{DeviceName}/thing/{MessageType}/{ActionType}
```

**Where**:
- `{ProductKey}`: `k0t5suseNtK`
- `{DeviceName}`: `esp8266_dev`
- `{MessageType}`: `service` or `event`
- `{ActionType}`: `property/set` or `property/post`

### Authentication Flow
1. Generate timestamp
2. Create device signature using HMAC-SHA256
3. Format client ID with security parameters
4. Connect using username/password authentication

## WiFi API

### Connection Configuration
```cpp
char ssid[] = "Redmi";        // Network SSID
char pass[] = "abc123456";    // Network password
```

### Connection Process
```cpp
WiFi.begin(ssid, pass);           // Initialize connection
while (!WiFi.isConnected()) {     // Wait for connection
    Serial.print(".");
    delay(5000);                  // Retry every 5 seconds
}
```

### Status Monitoring
```cpp
WiFi.isConnected()               // Returns true if connected
WiFi.status()                   // Returns connection status code
WiFi.localIP()                  // Returns assigned IP address
WiFi.RSSI()                     // Returns signal strength
```

## Sensor APIs

### DHT11 Temperature and Humidity Sensor

#### Library Interface
```cpp
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;              // Sensor object
```

#### Reading Process
```cpp
DHT.read(DHT11_PIN);            // Trigger sensor reading
float temperature = DHT.temperature;  // Get temperature (°C)
float humidity = DHT.humidity;        // Get humidity (% RH)
```

#### Pin Configuration
- **Data Pin**: GPIO 5 (D1 on NodeMCU)
- **Power**: 3.3V or 5V
- **Ground**: GND

#### Specifications
- **Temperature Range**: -40°C to 80°C (±2°C accuracy)
- **Humidity Range**: 5% to 95% RH (±5% accuracy)
- **Reading Interval**: Minimum 2 seconds between readings
- **Response Time**: < 6 seconds

#### Error Handling
```cpp
if (DHT.temperature != -999) {   // Check for valid reading
    // Process temperature data
}
if (DHT.humidity != -999) {      // Check for valid reading
    // Process humidity data
}
```

## Servo Control API

### Servo Motor Interface

#### Library Configuration
```cpp
#include <Servo.h>
Servo myServo;                  // Servo object
```

#### Initialization
```cpp
myServo.attach(servoPin);       // Attach to pin 4
myServo.write(90);              // Set initial position to 90°
```

#### Position Control
```cpp
myServo.write(angle);           // Set servo angle (0-180°)
```

#### Angle Constraints
```cpp
if (angle >= 0 && angle <= 180) {
    myServo.write(angle);       // Safe angle range
}
```

#### Control Commands
- **0°**: OFF position (fully counterclockwise)
- **90°**: Default/ON position (center)
- **180°**: Maximum clockwise position

#### Hardware Specifications
- **Control Signal**: PWM (20ms period)
- **Pulse Width**: 1-2ms (1ms = 0°, 2ms = 180°)
- **Operating Voltage**: 4.8V - 6V
- **Operating Current**: 100-300mA

## Error Codes and Handling

### MQTT Connection Errors
```cpp
mqttClient.connectError()       // Returns error code
```

**Common Error Codes**:
- `-2`: Connection refused - identifier rejected
- `-3`: Connection refused - server unavailable
- `-4`: Connection refused - bad username or password
- `-5`: Connection refused - not authorized

### WiFi Connection Status
```cpp
WiFi.status()                   // Returns WiFi status
```

**Status Codes**:
- `WL_CONNECTED`: Successfully connected
- `WL_NO_SHIELD`: WiFi shield not present
- `WL_IDLE_STATUS`: Temporary status during connection
- `WL_NO_SSID_AVAIL`: SSID cannot be reached
- `WL_SCAN_COMPLETED`: Scan networks completed
- `WL_CONNECT_FAILED`: Connection failed
- `WL_CONNECTION_LOST`: Connection lost
- `WL_DISCONNECTED`: Disconnected from network

### Serial Debug Output
```cpp
Serial.begin(9600);             // Initialize serial communication
Serial.println("Debug message"); // Debug output
Serial.print("Value: ");       // Formatted output
Serial.println(variable);      // Variable output
```

### Error Recovery Strategies

#### WiFi Reconnection
```cpp
if (!WiFi.isConnected()) {
    WiFi.disconnect();
    WiFi.begin(ssid, pass);
}
```

#### MQTT Reconnection
```cpp
if (!mqttClient.connected()) {
    mqttClient.connect(broker, port);
    mqttClient.subscribe(inTopic, subscribeQos);
}
```

#### Sensor Reading Validation
```cpp
if (DHT.temperature > -40 && DHT.temperature < 80) {
    // Valid temperature reading
}
```

## Performance Considerations

### Memory Management
- **JSON Document Size**: 512-1024 bytes for sensor data
- **Input Buffer**: Dynamic allocation for MQTT messages
- **Stack Usage**: Minimal due to simple function structure

### Timing Constraints
- **Sensor Reading**: 1-second delay for stability
- **MQTT Publishing**: Every 10 seconds
- **Message Processing**: Real-time callback handling

### Network Optimization
- **QoS Level 1**: Balance between reliability and performance
- **No Message Retention**: Reduces broker load
- **Efficient JSON**: Minimal payload structure

## Security Considerations

### Authentication
- **HMAC-SHA256**: Secure device authentication
- **Timestamp Validation**: Prevents replay attacks
- **Device Certificates**: Unique device identity

### Network Security
- **WPA/WPA2**: Secure WiFi connection
- **TLS/SSL**: Optional encrypted MQTT (port 8883)
- **Credential Management**: Secure storage of sensitive data

### Best Practices
- Change default WiFi credentials
- Use unique device certificates
- Implement secure credential storage
- Regular security updates