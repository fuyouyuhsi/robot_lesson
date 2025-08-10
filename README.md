# ESP8266 Robot: MQTT + DHT11 + Servo

A minimal ESP8266 (NodeMCU/Wemos D1 mini) project that:

- Connects to Wi‑Fi
- Connects to an MQTT broker
- Publishes DHT11 temperature/humidity telemetry periodically
- Subscribes to a command topic and moves a servo based on incoming JSON commands

The project consists of a single Arduino sketch: `esp8266_robot.ino`.

---

## Features

- Wi‑Fi connectivity using `ESP8266WiFi`
- MQTT client using `ArduinoMqttClient`
- JSON serialization/deserialization via `ArduinoJson`
- DHT11 sensor support via `DFRobot_DHT11`
- Servo control via `Servo` library
- Periodic telemetry publishing (default every 10 seconds)
- Command handling via MQTT to set servo angle (0° or 90°)

---

## Documentation

- API Reference: `docs/API.md`
- Usage Guide: `docs/USAGE.md`
- MQTT Details: `docs/MQTT.md`
- Wiring Guide: `docs/WIRING.md`
- Troubleshooting: `docs/TROUBLESHOOTING.md`

---

## Quick Start

1. Install prerequisites
   - Arduino IDE or PlatformIO
   - ESP8266 board support (e.g., NodeMCU, Wemos D1 mini)
   - Libraries: `ArduinoMqttClient`, `ArduinoJson`, `DFRobot_DHT11`, `Servo`
2. Wire the hardware (see Wiring below)
3. Open `esp8266_robot.ino`
4. Update Wi‑Fi and MQTT credentials
5. Build and upload to your ESP8266
6. Watch serial monitor at 9600 baud; verify Wi‑Fi and MQTT connections
7. Send a command to the subscribed MQTT topic to move the servo

---

## Hardware and Wiring

- Board: ESP8266 (e.g., NodeMCU, Wemos D1 mini)
- Sensors/Actuators:
  - DHT11 data pin → GPIO 5 (D1)
  - Servo signal → GPIO 4 (D2)
- Power:
  - Power the servo from a stable 5V source (not from the ESP8266 3.3V pin)
  - Common ground between ESP8266, DHT11, and servo

---

## Configuration

Edit these in `esp8266_robot.ino`:

- Wi‑Fi
  - `char ssid[] = "..."`
  - `char pass[] = "..."`
- MQTT
  - `const char broker[] = "..."`
  - `int port = 1883` (use 8883 for TLS)
  - Client ID via `mqttClient.setId(...)`
  - Username/Password via `mqttClient.setUsernamePassword(...)`
- Topics
  - Subscribe: `inTopic` → receives commands
  - Publish: `outTopic` → publishes telemetry
- Telemetry interval: `const long interval = 10000` (milliseconds)

Security note: move real secrets to `arduino_secrets.h` and do not commit them to source control.

---

## Build and Flash

- Arduino IDE
  - Tools → Board → ESP8266 Boards → your board
  - Tools → Upload Speed: 921600 (or stable for your setup)
  - Tools → Flash Size: default for your board
  - Open `esp8266_robot.ino`, adjust settings, Upload
- PlatformIO (optional)
  - Create a new project targeting ESP8266
  - Add the required libraries in `platformio.ini`

---

## MQTT Interface

- Subscribed topic: value of `inTopic`
- Published topic: value of `outTopic`

### Telemetry (published → `outTopic`)

Example payload:

```json
{
  "params": { "temp": 27, "humi": 62 },
  "vision": "1.0.0"
}
```

Note: the key is spelled `vision` in the sketch. If you intended `version`, update the code accordingly.

### Commands (subscribe ← `inTopic`)

Send a JSON message with an `items.led.value` field. The sketch interprets:

- `0` → move servo to 0°
- `1` (or non‑zero) → move servo to 90°

Example command payloads:

```json
{ "items": { "led": { "value": 0 } } }
```

```json
{ "items": { "led": { "value": 1 } } }
```

The sketch parses `items` and `led` internally and updates the servo accordingly.

---

## Serial Output

- 9600 baud
- Shows Wi‑Fi/MQTT connection status
- Prints published telemetry payloads
- Logs received command metadata and resulting servo angle

---

## Troubleshooting

- Wi‑Fi not connecting
  - Check SSID/password
  - Ensure 2.4GHz network; ESP8266 does not support 5GHz
- MQTT connection failed
  - Verify broker, port, client ID, username/password
  - For TLS, use `WiFiSSLClient` and port 8883
- Servo jitter or resets
  - Use separate 5V supply for servo; common ground with ESP8266
  - Keep servo wires short; use decoupling on supply
- No telemetry
  - Confirm `interval` and that DHT11 is wired correctly
  - Check serial logs for JSON serialization
- Commands ignored
  - Confirm you publish to `inTopic`
  - Verify command JSON structure matches examples above

---

## License

MIT (or your preferred license).