# Function Reference

This document provides detailed documentation for all functions in the ESP8266 IoT Robot project.

## Table of Contents

- [Core Functions](#core-functions)
  - [setup()](#setup)
  - [loop()](#loop)
  - [onMqttMessage()](#onmqttmessage)

## Core Functions

### `setup()`

**Description**: Initializes the ESP8266 robot system, including serial communication, servo motor, WiFi connection, and MQTT client setup.

**Signature**:
```cpp
void setup()
```

**Parameters**: None

**Return Value**: `void` - No return value

**Functionality**:
1. Initializes serial communication at 9600 baud rate
2. Configures GPIO pin 4 as output for servo control
3. Attaches servo to pin 4 and initializes position to 90 degrees
4. Establishes WiFi connection using provided credentials
5. Configures MQTT client with Alibaba Cloud IoT credentials
6. Connects to MQTT broker
7. Sets up message callback handler
8. Subscribes to incoming command topic

**Hardware Dependencies**:
- Servo motor connected to GPIO pin 4
- WiFi network availability
- Internet connectivity for MQTT broker access

**Example Usage**:
```cpp
// Called automatically by Arduino framework
// No manual invocation required
void setup() {
  // Initialization code is executed once at startup
}
```

**Error Handling**:
- Infinite loop if WiFi connection fails
- Infinite loop if MQTT broker connection fails
- Serial output for debugging connection status

**Pin Configuration**:
```cpp
pinMode(4, OUTPUT);        // Configure pin 4 for servo control
myServo.attach(servoPin);  // Attach servo to pin 4
```

**Network Configuration**:
```cpp
WiFi.begin(ssid, pass);                    // Connect to WiFi
mqttClient.connect(broker, port);          // Connect to MQTT broker
mqttClient.subscribe(inTopic, subscribeQos); // Subscribe to commands
```

---

### `loop()`

**Description**: Main execution loop that handles MQTT communication, sensor data reading, and periodic data transmission to the cloud platform.

**Signature**:
```cpp
void loop()
```

**Parameters**: None

**Return Value**: `void` - No return value

**Functionality**:
1. Polls MQTT client for incoming messages
2. Checks if transmission interval has elapsed (10 seconds)
3. Reads temperature and humidity from DHT11 sensor
4. Constructs JSON payload with sensor data
5. Publishes data to MQTT outbound topic
6. Increments message counter

**Timing**:
- Executes continuously
- Data transmission every 10 seconds (`interval = 10000` milliseconds)
- 1-second delay before sensor reading for stability

**Data Format**:
```json
{
  "params": {
    "temp": 25.6,
    "humi": 60.2
  },
  "version": "1.0.0"
}
```

**Example Usage**:
```cpp
// Called automatically by Arduino framework in continuous loop
// Handles real-time operations
void loop() {
  // Continuous execution of sensor reading and MQTT communication
}
```

**Sensor Reading Process**:
```cpp
DHT.read(DHT11_PIN);              // Read sensor data
json_data["temp"] = DHT.temperature; // Extract temperature
json_data["humi"] = DHT.humidity;    // Extract humidity
```

**MQTT Publishing**:
```cpp
mqttClient.beginMessage(outTopic, payload.length(), retained, qos, dup);
mqttClient.print(payload);
mqttClient.endMessage();
```

**Performance Considerations**:
- Non-blocking execution pattern
- Minimal delay operations (1 second for sensor stability)
- Efficient JSON serialization

---

### `onMqttMessage(int messageSize)`

**Description**: Callback function that processes incoming MQTT messages for servo motor control. Parses JSON commands and adjusts servo position based on received values.

**Signature**:
```cpp
void onMqttMessage(int messageSize)
```

**Parameters**:
- `messageSize` (int): Size of the incoming MQTT message in bytes

**Return Value**: `void` - No return value

**Functionality**:
1. Receives and logs incoming MQTT message metadata
2. Reads message content character by character
3. Parses JSON message structure
4. Extracts servo control command from "led" field
5. Sets servo position based on command value (0° or 90°)
6. Provides serial output for debugging

**Message Structure**:
```json
{
  "deviceType": "CustomCategory",
  "iotId": "device_id_string",
  "requestId": "timestamp_string",
  "items": {
    "led": {
      "value": 0,  // 0 = servo to 0°, 1 = servo to 90°
      "time": 1689847086996
    }
  }
}
```

**Servo Control Logic**:
- `value = 0`: Moves servo to 0 degrees (OFF position)
- `value = 1`: Moves servo to 90 degrees (ON position)

**Example Usage**:
```cpp
// Automatically called when MQTT message is received
// Manual invocation not required

// Example incoming message:
// {"items":{"led":{"value":1,"time":1689847086996}}}
// Result: Servo moves to 90 degrees
```

**JSON Parsing Process**:
```cpp
DynamicJsonDocument json_msg(1024);     // Main message container
DynamicJsonDocument json_item(1024);    // Items section
DynamicJsonDocument json_value(1024);   // LED value section

deserializeJson(json_msg, inputString);  // Parse main message
String items = json_msg["items"];        // Extract items
deserializeJson(json_item, items);       // Parse items section
String led = json_item["led"];           // Extract LED command
deserializeJson(json_value, led);        // Parse LED value
bool value = json_value["value"];        // Get control value
```

**Servo Control Implementation**:
```cpp
if(value == 0) {
    angle = 0;  // Set to OFF position
    Serial.println("off");
} else {
    angle = 90; // Set to ON position  
    Serial.println("on");
}

if (angle >= 0 && angle <= 180) {
    myServo.write(angle);  // Move servo to position
    Serial.print("Servo moved to: ");
    Serial.println(angle);
}
```

**Error Handling**:
- Angle bounds checking (0-180 degrees)
- JSON parsing error resilience
- Serial debug output for troubleshooting

**Performance Features**:
- Efficient character-by-character reading
- Dynamic JSON document allocation
- Immediate servo response to commands

## Global Variables and Objects

### Hardware Objects
```cpp
Servo myServo;           // Servo motor control object
DFRobot_DHT11 DHT;      // DHT11 sensor interface object
WiFiClient wifiClient;   // WiFi client for network communication
MqttClient mqttClient;   // MQTT client for IoT communication
```

### Configuration Constants
```cpp
#define DHT11_PIN 5      // DHT11 sensor data pin
#define servoPin 4       // Servo motor control pin
const long interval = 10000;  // Data transmission interval (10 seconds)
```

### Network Configuration
```cpp
char ssid[] = "Redmi";              // WiFi network name
char pass[] = "abc123456";          // WiFi password
const char broker[] = "...";        // MQTT broker address
int port = 1883;                    // MQTT broker port
```

### State Variables
```cpp
int angle = 0;                      // Current servo angle
int count = 0;                      // Message counter
String inputString = "";            // MQTT message buffer
unsigned long previousMillis = 0;   // Last transmission timestamp
```

## Dependencies and Libraries

### Required Libraries
- **ArduinoMqttClient**: MQTT protocol implementation
- **DFRobot_DHT11**: DHT11 sensor interface
- **ArduinoJson**: JSON parsing and serialization
- **ESP8266WiFi**: WiFi connectivity for ESP8266
- **Servo**: Servo motor control library

### Hardware Dependencies
- ESP8266 microcontroller
- DHT11 temperature/humidity sensor
- SG90 servo motor (or compatible)
- Stable power supply (3.3V/5V)
- WiFi network connectivity