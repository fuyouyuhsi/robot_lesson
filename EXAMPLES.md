# Usage Examples

This document provides comprehensive usage examples, code snippets, and practical scenarios for the ESP8266 IoT Robot project.

## Table of Contents

- [Basic Setup Examples](#basic-setup-examples)
- [Sensor Reading Examples](#sensor-reading-examples)
- [Servo Control Examples](#servo-control-examples)
- [MQTT Communication Examples](#mqtt-communication-examples)
- [Testing and Debugging](#testing-and-debugging)
- [Advanced Configurations](#advanced-configurations)
- [Troubleshooting Scenarios](#troubleshooting-scenarios)
- [Integration Examples](#integration-examples)

## Basic Setup Examples

### Complete Hardware Setup

```cpp
// Complete initialization example
#include <ArduinoMqttClient.h>
#include <DFRobot_DHT11.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

// Hardware configuration
#define DHT11_PIN 5
#define SERVO_PIN 4

// Network configuration
char ssid[] = "YourWiFiNetwork";
char pass[] = "YourPassword";

// Initialize objects
Servo myServo;
DFRobot_DHT11 DHT;
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void setup() {
    Serial.begin(9600);
    
    // Configure hardware
    pinMode(SERVO_PIN, OUTPUT);
    myServo.attach(SERVO_PIN);
    myServo.write(90);  // Center position
    
    // Connect to WiFi
    connectToWiFi();
    
    // Connect to MQTT
    connectToMQTT();
    
    Serial.println("Setup complete!");
}

void connectToWiFi() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, pass);
    while (!WiFi.isConnected()) {
        Serial.print(".");
        delay(1000);
    }
    
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void connectToMQTT() {
    const char broker[] = "your-broker.mqtt.com";
    int port = 1883;
    
    mqttClient.setId("your-device-id");
    mqttClient.setUsernamePassword("username", "password");
    
    Serial.print("Connecting to MQTT broker: ");
    Serial.println(broker);
    
    if (!mqttClient.connect(broker, port)) {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());
        while (1);
    }
    
    Serial.println("Connected to MQTT broker!");
}
```

### Minimal Working Example

```cpp
// Minimal example for quick testing
void setup() {
    Serial.begin(9600);
    Serial.println("ESP8266 Robot Starting...");
    
    // Test servo
    myServo.attach(4);
    myServo.write(0);
    delay(1000);
    myServo.write(90);
    delay(1000);
    myServo.write(180);
    delay(1000);
    myServo.write(90);
    
    Serial.println("Servo test complete!");
}

void loop() {
    // Read sensor every 5 seconds
    DHT.read(5);
    Serial.print("Temperature: ");
    Serial.print(DHT.temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(DHT.humidity);
    Serial.println("%");
    
    delay(5000);
}
```

## Sensor Reading Examples

### Basic DHT11 Reading

```cpp
void readSensor() {
    // Read DHT11 sensor
    DHT.read(DHT11_PIN);
    
    float temperature = DHT.temperature;
    float humidity = DHT.humidity;
    
    // Validate readings
    if (temperature > -40 && temperature < 80) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
    } else {
        Serial.println("Temperature reading error!");
    }
    
    if (humidity >= 0 && humidity <= 100) {
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println("%");
    } else {
        Serial.println("Humidity reading error!");
    }
}
```

### Advanced Sensor Reading with Averaging

```cpp
float getAverageTemperature(int samples = 5) {
    float sum = 0;
    int validReadings = 0;
    
    for (int i = 0; i < samples; i++) {
        DHT.read(DHT11_PIN);
        delay(2000);  // DHT11 requires 2-second intervals
        
        if (DHT.temperature > -40 && DHT.temperature < 80) {
            sum += DHT.temperature;
            validReadings++;
        }
    }
    
    if (validReadings > 0) {
        return sum / validReadings;
    } else {
        return -999;  // Error value
    }
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    float avgTemp = getAverageTemperature(3);
    
    if (avgTemp != -999) {
        Serial.print("Average Temperature: ");
        Serial.print(avgTemp);
        Serial.println("°C");
    } else {
        Serial.println("Failed to read temperature!");
    }
    
    delay(30000);  // Wait 30 seconds between readings
}
```

### Sensor Data with Timestamps

```cpp
struct SensorReading {
    float temperature;
    float humidity;
    unsigned long timestamp;
    bool valid;
};

SensorReading readSensorWithTimestamp() {
    SensorReading reading;
    reading.timestamp = millis();
    
    DHT.read(DHT11_PIN);
    reading.temperature = DHT.temperature;
    reading.humidity = DHT.humidity;
    
    // Validate readings
    reading.valid = (reading.temperature > -40 && reading.temperature < 80) &&
                   (reading.humidity >= 0 && reading.humidity <= 100);
    
    return reading;
}

void logSensorData() {
    SensorReading data = readSensorWithTimestamp();
    
    if (data.valid) {
        Serial.print("[");
        Serial.print(data.timestamp);
        Serial.print("] Temp: ");
        Serial.print(data.temperature);
        Serial.print("°C, Humidity: ");
        Serial.print(data.humidity);
        Serial.println("%");
    } else {
        Serial.println("Invalid sensor reading!");
    }
}
```

## Servo Control Examples

### Basic Servo Control

```cpp
void controlServo(int angle) {
    // Validate angle range
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    
    myServo.write(angle);
    Serial.print("Servo moved to: ");
    Serial.print(angle);
    Serial.println(" degrees");
}

void servoSweep() {
    Serial.println("Starting servo sweep...");
    
    // Sweep from 0 to 180
    for (int angle = 0; angle <= 180; angle += 10) {
        controlServo(angle);
        delay(500);
    }
    
    // Sweep back from 180 to 0
    for (int angle = 180; angle >= 0; angle -= 10) {
        controlServo(angle);
        delay(500);
    }
    
    Serial.println("Servo sweep complete!");
}
```

### Smooth Servo Movement

```cpp
void smoothMove(int startAngle, int endAngle, int stepDelay = 20) {
    int currentAngle = startAngle;
    int step = (endAngle > startAngle) ? 1 : -1;
    
    while (currentAngle != endAngle) {
        myServo.write(currentAngle);
        delay(stepDelay);
        currentAngle += step;
    }
    
    myServo.write(endAngle);  // Ensure we reach the exact target
}

void demonstrateSmootMovement() {
    Serial.println("Demonstrating smooth servo movement...");
    
    smoothMove(90, 0, 15);    // Slow movement to 0°
    delay(1000);
    smoothMove(0, 180, 10);   // Medium speed to 180°
    delay(1000);
    smoothMove(180, 90, 5);   // Fast movement to center
    
    Serial.println("Smooth movement demo complete!");
}
```

### Servo with Position Feedback

```cpp
class ServoController {
private:
    Servo servo;
    int currentPosition;
    int targetPosition;
    int pin;

public:
    ServoController(int servoPin) : pin(servoPin), currentPosition(90), targetPosition(90) {}
    
    void begin() {
        servo.attach(pin);
        servo.write(currentPosition);
        Serial.print("Servo initialized at position: ");
        Serial.println(currentPosition);
    }
    
    void setTarget(int angle) {
        if (angle >= 0 && angle <= 180) {
            targetPosition = angle;
            Serial.print("Target position set to: ");
            Serial.println(targetPosition);
        }
    }
    
    void update() {
        if (currentPosition != targetPosition) {
            int step = (targetPosition > currentPosition) ? 1 : -1;
            currentPosition += step;
            servo.write(currentPosition);
            
            if (currentPosition == targetPosition) {
                Serial.print("Reached target position: ");
                Serial.println(currentPosition);
            }
        }
    }
    
    int getPosition() {
        return currentPosition;
    }
    
    bool isMoving() {
        return currentPosition != targetPosition;
    }
};

ServoController servoCtrl(4);

void setup() {
    Serial.begin(9600);
    servoCtrl.begin();
}

void loop() {
    // Example usage
    static unsigned long lastUpdate = 0;
    static int targetAngles[] = {0, 90, 180, 90};
    static int targetIndex = 0;
    
    // Update servo position every 20ms for smooth movement
    if (millis() - lastUpdate > 20) {
        servoCtrl.update();
        lastUpdate = millis();
    }
    
    // Change target every 3 seconds
    if (!servoCtrl.isMoving()) {
        delay(3000);
        servoCtrl.setTarget(targetAngles[targetIndex]);
        targetIndex = (targetIndex + 1) % 4;
    }
}
```

## MQTT Communication Examples

### Basic MQTT Publishing

```cpp
void publishSensorData() {
    // Read sensor
    DHT.read(DHT11_PIN);
    
    // Create JSON payload
    DynamicJsonDocument json_msg(512);
    DynamicJsonDocument json_data(512);
    
    json_data["temp"] = DHT.temperature;
    json_data["humi"] = DHT.humidity;
    json_data["timestamp"] = millis();
    
    json_msg["params"] = json_data;
    json_msg["version"] = "1.0.0";
    
    String payload;
    serializeJson(json_msg, payload);
    
    // Publish to MQTT
    const char* topic = "/sensor/data";
    bool retained = false;
    int qos = 1;
    
    mqttClient.beginMessage(topic, payload.length(), retained, qos);
    mqttClient.print(payload);
    mqttClient.endMessage();
    
    Serial.print("Published: ");
    Serial.println(payload);
}
```

### MQTT with Error Handling

```cpp
bool publishWithRetry(const char* topic, String payload, int maxRetries = 3) {
    for (int attempt = 0; attempt < maxRetries; attempt++) {
        if (mqttClient.connected()) {
            mqttClient.beginMessage(topic);
            mqttClient.print(payload);
            mqttClient.endMessage();
            
            Serial.print("Message published successfully on attempt ");
            Serial.println(attempt + 1);
            return true;
        } else {
            Serial.print("MQTT not connected, attempt ");
            Serial.print(attempt + 1);
            Serial.println(" failed");
            
            // Try to reconnect
            if (attempt < maxRetries - 1) {
                reconnectMQTT();
                delay(1000);
            }
        }
    }
    
    Serial.println("Failed to publish after all retries");
    return false;
}

void reconnectMQTT() {
    Serial.print("Attempting MQTT reconnection...");
    
    if (mqttClient.connect("your-broker.com", 1883)) {
        Serial.println("Reconnected!");
        mqttClient.subscribe("/commands/servo");
    } else {
        Serial.print("Failed, error code: ");
        Serial.println(mqttClient.connectError());
    }
}
```

### Advanced MQTT Message Processing

```cpp
struct Command {
    String device;
    String action;
    int value;
    unsigned long timestamp;
    bool valid;
};

Command parseCommand(String jsonString) {
    Command cmd;
    cmd.valid = false;
    
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonString);
    
    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return cmd;
    }
    
    if (doc.containsKey("device") && doc.containsKey("action") && doc.containsKey("value")) {
        cmd.device = doc["device"].as<String>();
        cmd.action = doc["action"].as<String>();
        cmd.value = doc["value"];
        cmd.timestamp = doc["timestamp"] | millis();
        cmd.valid = true;
    }
    
    return cmd;
}

void processCommand(const Command& cmd) {
    if (!cmd.valid) {
        Serial.println("Invalid command received");
        return;
    }
    
    Serial.print("Processing command - Device: ");
    Serial.print(cmd.device);
    Serial.print(", Action: ");
    Serial.print(cmd.action);
    Serial.print(", Value: ");
    Serial.println(cmd.value);
    
    if (cmd.device == "servo") {
        if (cmd.action == "move") {
            controlServo(cmd.value);
        } else if (cmd.action == "sweep") {
            servoSweep();
        }
    } else if (cmd.device == "sensor") {
        if (cmd.action == "read") {
            publishSensorData();
        }
    }
}

void onMqttMessage(int messageSize) {
    String message = "";
    
    while (mqttClient.available()) {
        message += (char)mqttClient.read();
    }
    
    Serial.print("Received: ");
    Serial.println(message);
    
    Command cmd = parseCommand(message);
    processCommand(cmd);
}
```

## Testing and Debugging

### Serial Monitor Output Formatting

```cpp
void printSystemStatus() {
    Serial.println("=== ESP8266 Robot Status ===");
    
    // WiFi status
    Serial.print("WiFi Status: ");
    if (WiFi.isConnected()) {
        Serial.print("Connected to ");
        Serial.println(WiFi.SSID());
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Signal Strength: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    } else {
        Serial.println("Disconnected");
    }
    
    // MQTT status
    Serial.print("MQTT Status: ");
    Serial.println(mqttClient.connected() ? "Connected" : "Disconnected");
    
    // Sensor readings
    DHT.read(DHT11_PIN);
    Serial.print("Temperature: ");
    Serial.print(DHT.temperature);
    Serial.println("°C");
    Serial.print("Humidity: ");
    Serial.print(DHT.humidity);
    Serial.println("%");
    
    // Memory info
    Serial.print("Free Heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
    
    Serial.print("Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println(" seconds");
    
    Serial.println("============================");
}

void setup() {
    Serial.begin(9600);
    // ... other setup code ...
    
    // Print status every 30 seconds
    Serial.println("Starting ESP8266 Robot...");
}

void loop() {
    static unsigned long lastStatusPrint = 0;
    
    // ... main loop code ...
    
    if (millis() - lastStatusPrint > 30000) {
        printSystemStatus();
        lastStatusPrint = millis();
    }
}
```

### Diagnostic Functions

```cpp
void runDiagnostics() {
    Serial.println("Starting system diagnostics...");
    
    // Test 1: Servo movement
    Serial.println("Test 1: Servo movement");
    myServo.write(0);
    delay(1000);
    myServo.write(90);
    delay(1000);
    myServo.write(180);
    delay(1000);
    myServo.write(90);
    Serial.println("✓ Servo test completed");
    
    // Test 2: Sensor reading
    Serial.println("Test 2: Sensor reading");
    for (int i = 0; i < 3; i++) {
        DHT.read(DHT11_PIN);
        if (DHT.temperature > -40 && DHT.temperature < 80) {
            Serial.println("✓ Sensor reading OK");
            break;
        } else {
            Serial.print("✗ Sensor reading failed, attempt ");
            Serial.println(i + 1);
        }
        delay(2000);
    }
    
    // Test 3: WiFi connection
    Serial.println("Test 3: WiFi connection");
    if (WiFi.isConnected()) {
        Serial.println("✓ WiFi connected");
        
        // Test internet connectivity
        WiFiClient testClient;
        if (testClient.connect("google.com", 80)) {
            Serial.println("✓ Internet connectivity OK");
            testClient.stop();
        } else {
            Serial.println("✗ Internet connectivity failed");
        }
    } else {
        Serial.println("✗ WiFi not connected");
    }
    
    // Test 4: MQTT connection
    Serial.println("Test 4: MQTT connection");
    if (mqttClient.connected()) {
        Serial.println("✓ MQTT connected");
    } else {
        Serial.println("✗ MQTT not connected");
    }
    
    Serial.println("Diagnostics completed!\n");
}
```

### Debug Mode Implementation

```cpp
#define DEBUG_MODE true  // Set to false for production

void debugPrint(String message) {
    if (DEBUG_MODE) {
        Serial.print("[DEBUG] ");
        Serial.println(message);
    }
}

void debugPrintF(String format, float value) {
    if (DEBUG_MODE) {
        Serial.print("[DEBUG] ");
        Serial.print(format);
        Serial.println(value);
    }
}

void debugJSON(DynamicJsonDocument& doc) {
    if (DEBUG_MODE) {
        Serial.println("[DEBUG] JSON Document:");
        serializeJsonPretty(doc, Serial);
        Serial.println();
    }
}

// Usage examples:
void loop() {
    debugPrint("Starting main loop iteration");
    
    // Read sensor
    DHT.read(DHT11_PIN);
    debugPrintF("Temperature reading: ", DHT.temperature);
    debugPrintF("Humidity reading: ", DHT.humidity);
    
    // Create JSON
    DynamicJsonDocument json_msg(512);
    json_msg["temp"] = DHT.temperature;
    json_msg["humi"] = DHT.humidity;
    
    debugJSON(json_msg);
    
    debugPrint("Main loop iteration completed");
}
```

## Advanced Configurations

### WiFi Manager for Easy Configuration

```cpp
#include <WiFiManager.h>

WiFiManager wifiManager;

void setupWiFiManager() {
    // Set custom parameters
    WiFiManagerParameter custom_mqtt_server("server", "MQTT Server", "mqtt.server.com", 40);
    WiFiManagerParameter custom_mqtt_port("port", "MQTT Port", "1883", 6);
    WiFiManagerParameter custom_device_id("device", "Device ID", "esp8266_robot", 32);
    
    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_device_id);
    
    // Set config portal timeout
    wifiManager.setConfigPortalTimeout(300);  // 5 minutes
    
    // Try to connect, if it fails start config portal
    if (!wifiManager.autoConnect("ESP8266-Robot-Setup")) {
        Serial.println("Failed to connect and timeout occurred");
        delay(3000);
        ESP.restart();
    }
    
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Get custom parameters
    String mqttServer = custom_mqtt_server.getValue();
    String mqttPort = custom_mqtt_port.getValue();
    String deviceId = custom_device_id.getValue();
    
    Serial.println("Custom parameters:");
    Serial.println("MQTT Server: " + mqttServer);
    Serial.println("MQTT Port: " + mqttPort);
    Serial.println("Device ID: " + deviceId);
}
```

### OTA (Over-The-Air) Updates

```cpp
#include <ArduinoOTA.h>

void setupOTA() {
    ArduinoOTA.setHostname("esp8266-robot");
    ArduinoOTA.setPassword("your-ota-password");
    
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        Serial.println("Start updating " + type);
    });
    
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });
    
    ArduinoOTA.begin();
    Serial.println("OTA Ready");
}

void loop() {
    ArduinoOTA.handle();  // Handle OTA updates
    
    // ... rest of main loop ...
}
```

### Configuration Storage in EEPROM

```cpp
#include <EEPROM.h>

struct Config {
    char ssid[32];
    char password[64];
    char mqttServer[64];
    int mqttPort;
    char deviceId[32];
    bool valid;
};

void saveConfig(const Config& config) {
    EEPROM.begin(sizeof(Config));
    EEPROM.put(0, config);
    EEPROM.commit();
    EEPROM.end();
    Serial.println("Configuration saved to EEPROM");
}

Config loadConfig() {
    Config config;
    EEPROM.begin(sizeof(Config));
    EEPROM.get(0, config);
    EEPROM.end();
    
    if (!config.valid) {
        Serial.println("No valid configuration found, using defaults");
        // Set default values
        strcpy(config.ssid, "DefaultSSID");
        strcpy(config.password, "DefaultPassword");
        strcpy(config.mqttServer, "mqtt.server.com");
        config.mqttPort = 1883;
        strcpy(config.deviceId, "esp8266_robot");
        config.valid = true;
        saveConfig(config);
    }
    
    return config;
}

void setup() {
    Serial.begin(9600);
    
    Config config = loadConfig();
    
    Serial.println("Loaded configuration:");
    Serial.println("SSID: " + String(config.ssid));
    Serial.println("MQTT Server: " + String(config.mqttServer));
    Serial.println("MQTT Port: " + String(config.mqttPort));
    Serial.println("Device ID: " + String(config.deviceId));
    
    // Use configuration for WiFi and MQTT setup
    WiFi.begin(config.ssid, config.password);
    // ... rest of setup ...
}
```

## Troubleshooting Scenarios

### WiFi Connection Issues

```cpp
void troubleshootWiFi() {
    Serial.println("WiFi Troubleshooting:");
    
    // Check WiFi status
    wl_status_t status = WiFi.status();
    Serial.print("WiFi Status Code: ");
    Serial.println(status);
    
    switch (status) {
        case WL_CONNECTED:
            Serial.println("✓ WiFi is connected");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
            break;
        case WL_NO_SSID_AVAIL:
            Serial.println("✗ SSID not available");
            Serial.println("Solution: Check network name");
            break;
        case WL_CONNECT_FAILED:
            Serial.println("✗ Connection failed");
            Serial.println("Solution: Check password");
            break;
        case WL_CONNECTION_LOST:
            Serial.println("✗ Connection lost");
            Serial.println("Solution: Check signal strength");
            break;
        case WL_DISCONNECTED:
            Serial.println("✗ Disconnected");
            Serial.println("Solution: Attempting reconnection...");
            WiFi.reconnect();
            break;
        default:
            Serial.println("✗ Unknown WiFi status");
            break;
    }
    
    // Scan for available networks
    Serial.println("\nScanning for networks...");
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("No networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found:");
        for (int i = 0; i < n; ++i) {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
        }
    }
}
```

### MQTT Connection Debugging

```cpp
void troubleshootMQTT() {
    Serial.println("MQTT Troubleshooting:");
    
    if (!WiFi.isConnected()) {
        Serial.println("✗ WiFi not connected - fix WiFi first");
        return;
    }
    
    Serial.println("✓ WiFi is connected");
    
    // Test broker connectivity
    WiFiClient testClient;
    Serial.print("Testing connection to broker: ");
    Serial.println("your-broker.com");
    
    if (testClient.connect("your-broker.com", 1883)) {
        Serial.println("✓ Broker is reachable");
        testClient.stop();
        
        // Try MQTT connection
        if (mqttClient.connect("your-broker.com", 1883)) {
            Serial.println("✓ MQTT connected successfully");
        } else {
            Serial.print("✗ MQTT connection failed, error: ");
            Serial.println(mqttClient.connectError());
            
            switch (mqttClient.connectError()) {
                case -2:
                    Serial.println("Solution: Check client ID");
                    break;
                case -4:
                    Serial.println("Solution: Check username/password");
                    break;
                case -5:
                    Serial.println("Solution: Check authorization");
                    break;
                default:
                    Serial.println("Solution: Check broker settings");
                    break;
            }
        }
    } else {
        Serial.println("✗ Broker is not reachable");
        Serial.println("Solution: Check broker address and port");
    }
}
```

### Sensor Reading Issues

```cpp
void troubleshootSensor() {
    Serial.println("DHT11 Sensor Troubleshooting:");
    
    // Test multiple readings
    int successCount = 0;
    int attempts = 5;
    
    for (int i = 0; i < attempts; i++) {
        DHT.read(DHT11_PIN);
        
        Serial.print("Attempt ");
        Serial.print(i + 1);
        Serial.print(": ");
        
        if (DHT.temperature > -40 && DHT.temperature < 80) {
            Serial.print("✓ Temp: ");
            Serial.print(DHT.temperature);
            Serial.print("°C, Humi: ");
            Serial.print(DHT.humidity);
            Serial.println("%");
            successCount++;
        } else {
            Serial.println("✗ Invalid reading");
        }
        
        delay(2500);  // DHT11 requires 2+ second intervals
    }
    
    float successRate = (float)successCount / attempts * 100;
    Serial.print("Success rate: ");
    Serial.print(successRate);
    Serial.println("%");
    
    if (successRate < 50) {
        Serial.println("Solutions:");
        Serial.println("- Check wiring (VCC, GND, Data)");
        Serial.println("- Verify pin number in code");
        Serial.println("- Check power supply stability");
        Serial.println("- Try different sensor");
    }
}
```

## Integration Examples

### Web Server Interface

```cpp
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void setupWebServer() {
    server.on("/", handleRoot);
    server.on("/sensor", handleSensor);
    server.on("/servo", handleServo);
    server.on("/status", handleStatus);
    
    server.begin();
    Serial.println("Web server started");
    Serial.print("Access at: http://");
    Serial.println(WiFi.localIP());
}

void handleRoot() {
    String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>ESP8266 Robot Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        .card { background: #f0f0f0; padding: 20px; margin: 10px; border-radius: 5px; }
        button { padding: 10px 20px; margin: 5px; cursor: pointer; }
        .servo-controls button { background: #4CAF50; color: white; border: none; }
        .status { background: #e7f3ff; }
    </style>
</head>
<body>
    <h1>ESP8266 Robot Control Panel</h1>
    
    <div class="card">
        <h3>Servo Control</h3>
        <div class="servo-controls">
            <button onclick="moveServo(0)">0°</button>
            <button onclick="moveServo(45)">45°</button>
            <button onclick="moveServo(90)">90°</button>
            <button onclick="moveServo(135)">135°</button>
            <button onclick="moveServo(180)">180°</button>
        </div>
        <br>
        <input type="range" id="servoSlider" min="0" max="180" value="90" onchange="moveServo(this.value)">
        <span id="servoValue">90°</span>
    </div>
    
    <div class="card status">
        <h3>Current Status</h3>
        <div id="status">Loading...</div>
        <button onclick="updateStatus()">Refresh</button>
    </div>
    
    <script>
        function moveServo(angle) {
            fetch('/servo?angle=' + angle)
                .then(response => response.text())
                .then(data => {
                    document.getElementById('servoValue').innerText = angle + '°';
                    console.log('Servo moved to ' + angle + '°');
                });
        }
        
        function updateStatus() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('status').innerHTML = 
                        'Temperature: ' + data.temperature + '°C<br>' +
                        'Humidity: ' + data.humidity + '%<br>' +
                        'Servo Position: ' + data.servoAngle + '°<br>' +
                        'WiFi RSSI: ' + data.rssi + ' dBm<br>' +
                        'Uptime: ' + data.uptime + ' seconds';
                });
        }
        
        // Auto-update status every 10 seconds
        setInterval(updateStatus, 10000);
        updateStatus();
    </script>
</body>
</html>
)";
    
    server.send(200, "text/html", html);
}

void handleServo() {
    if (server.hasArg("angle")) {
        int angle = server.arg("angle").toInt();
        if (angle >= 0 && angle <= 180) {
            myServo.write(angle);
            server.send(200, "text/plain", "Servo moved to " + String(angle) + " degrees");
        } else {
            server.send(400, "text/plain", "Invalid angle. Use 0-180.");
        }
    } else {
        server.send(400, "text/plain", "Missing angle parameter");
    }
}

void handleSensor() {
    DHT.read(DHT11_PIN);
    
    DynamicJsonDocument json(256);
    json["temperature"] = DHT.temperature;
    json["humidity"] = DHT.humidity;
    json["timestamp"] = millis();
    
    String response;
    serializeJson(json, response);
    
    server.send(200, "application/json", response);
}

void handleStatus() {
    DynamicJsonDocument json(512);
    
    // Read current sensor values
    DHT.read(DHT11_PIN);
    
    json["temperature"] = DHT.temperature;
    json["humidity"] = DHT.humidity;
    json["servoAngle"] = 90;  // You'd track this in your servo control
    json["wifiConnected"] = WiFi.isConnected();
    json["rssi"] = WiFi.RSSI();
    json["ipAddress"] = WiFi.localIP().toString();
    json["uptime"] = millis() / 1000;
    json["freeHeap"] = ESP.getFreeHeap();
    
    String response;
    serializeJson(json, response);
    
    server.send(200, "application/json", response);
}

void loop() {
    server.handleClient();
    // ... rest of main loop ...
}
```

This comprehensive documentation provides detailed examples for every aspect of the ESP8266 IoT Robot project, from basic setup to advanced configurations and troubleshooting. Users can reference these examples to understand how to implement, test, and extend the functionality of their IoT robot system.