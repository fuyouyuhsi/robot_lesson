# Usage Guide

## Prerequisites

- Arduino IDE (or PlatformIO)
- ESP8266 board support installed
- Libraries installed: `ArduinoMqttClient`, `ArduinoJson`, `DFRobot_DHT11`, `Servo`

## Setup

1. Open `esp8266_robot.ino`
2. Update Wi‑Fi credentials:
   - `ssid`, `pass`
3. Update MQTT settings:
   - `broker`, `port`
   - `mqttClient.setId(...)`
   - `mqttClient.setUsernamePassword(...)`
4. Wire hardware:
   - DHT11 → GPIO 5 (D1)
   - Servo → GPIO 4 (D2)
   - Common ground; external 5V for servo recommended

## Build and Upload

- Select your ESP8266 board in the IDE
- Set Serial Monitor to 9600 baud
- Upload the sketch

## Verify

- Watch serial logs for:
  - Wi‑Fi connected
  - MQTT connected
  - Subscribed to topic
  - Periodic telemetry publish

## Send a Command

Publish to `inTopic` a JSON payload like:

```json
{ "items": { "led": { "value": 0 } } }
```

This moves the servo to 0°. To move to 90°:

```json
{ "items": { "led": { "value": 1 } } }
```

## Receive Telemetry

Subscribe to `outTopic` and expect payloads like:

```json
{ "params": { "temp": 27, "humi": 62 }, "vision": "1.0.0" }
```

## Testing with Mosquitto CLI

- Subscribe:
  - `mosquitto_sub -h <broker> -t <outTopic> -v`
- Publish:
  - `mosquitto_pub -h <broker> -t <inTopic> -m '{"items":{"led":{"value":0}}}'`

## Notes

- For TLS, switch to `WiFiSSLClient` and port 8883
- If your broker requires different auth (e.g., tokens), adapt `setId` and `setUsernamePassword` accordingly
- If commands are not processed, double-check topic names and JSON structure