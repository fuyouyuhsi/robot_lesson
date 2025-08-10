# API Reference

This document covers the Arduino sketch `esp8266_robot.ino` public surface: functions, global configuration, MQTT topics, and data contracts.

## Functions

### void setup()
Initializes serial, pins, Wi‑Fi, MQTT client, subscriptions, and servo.

- Serial at 9600 baud
- Attaches servo to `servoPin`, initializes to 90°
- Connects to Wi‑Fi (`ssid`, `pass`)
- Configures MQTT client ID, username/password
- Connects to MQTT `broker`:`port`
- Binds message callback `onMqttMessage`
- Subscribes to `inTopic`

### void loop()
Main loop that:

- Calls `mqttClient.poll()` to maintain the MQTT connection and receive messages
- Every `interval` milliseconds:
  - Reads DHT11 (`DHT.temperature`, `DHT.humidity`)
  - Publishes telemetry to `outTopic` as JSON

### void onMqttMessage(int messageSize)
Message handler for subscribed topic.

- Logs metadata (topic, dup, QoS, retain, length)
- Reads the full message into `inputString`
- Deserializes the JSON payload and extracts `items.led.value`
- If `value == 0`: set servo angle to 0°
- Else: set servo angle to 90°
- Writes the angle to the servo and logs the result

## Globals and Constants

- Wi‑Fi
  - `char ssid[]` Wi‑Fi SSID
  - `char pass[]` Wi‑Fi password
- Sensors/Actuators
  - `DFRobot_DHT11 DHT` DHT11 driver instance
  - `#define DHT11_PIN 5` DHT11 data pin (GPIO 5 / D1)
  - `Servo myServo` Servo instance
  - `#define servoPin 4` Servo signal pin (GPIO 4 / D2)
  - `int angle` Current servo angle (0..180)
- MQTT
  - `WiFiClient wifiClient` or `WiFiSSLClient` (if TLS)
  - `MqttClient mqttClient(wifiClient)` MQTT client
  - `const char broker[]` MQTT broker hostname
  - `int port` 1883 (or 8883 with TLS)
  - `const char inTopic[]` Subscribed command topic
  - `const char outTopic[]` Published telemetry topic
- Scheduling
  - `const long interval = 10000` Telemetry period (ms)
  - `unsigned long previousMillis` Last publish timestamp
- Misc
  - `int count` Publish counter
  - `String inputString` Buffer for incoming message

## MQTT Data Contracts

### Telemetry (publish → `outTopic`)

Schema:

```json
{
  "params": {
    "temp": <number>,
    "humi": <number>
  },
  "vision": "<string>"
}
```

Notes:
- `vision` is the literal key used in code. If you intended `version`, rename in the sketch.

### Command (subscribe ← `inTopic`)

Schema:

```json
{
  "items": {
    "led": {
      "value": <0|1>
    }
  }
}
```

Behavior:
- `value == 0` → servo to 0°
- `value != 0` → servo to 90°

## External Dependencies

- `ArduinoMqttClient`
- `ArduinoJson`
- `ESP8266WiFi`
- `DFRobot_DHT11`
- `Servo`

## Error Handling

- Wi‑Fi connection loops until connected
- MQTT connection: on failure, prints error and halts (`while(1);`)
- DHT11 read is not explicitly checked; values may be stale on sensor error

## Extensibility Notes

- Use `WiFiSSLClient` and port 8883 for TLS
- Consider extracting secrets to `arduino_secrets.h`
- Adjust `interval` for telemetry rate
- Replace command parsing to accept angles directly if needed (e.g., `{ "items": { "servo": { "angle": 45 } } }`) and update handler accordingly