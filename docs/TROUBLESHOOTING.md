# Troubleshooting

## Wi‑Fi

- Stuck connecting: verify SSID/password and 2.4GHz network
- Weak signal: move closer to AP or add antenna

## MQTT

- Connection failed: check broker host/port and credentials
- TLS required: switch to `WiFiSSLClient` and port 8883; ensure broker has a trusted root CA on the device if needed

## Telemetry

- No messages: confirm `interval` and that DHT11 wiring is correct
- Nonsensical values: DHT11 requires stable power; add delay before first read if needed

## Commands

- Ignored commands: verify publish topic equals `inTopic`
- JSON shape mismatch: ensure payload resembles `{ "items": { "led": { "value": 0 } } }`

## Servo

- Jitter/reset: use external 5V and common ground; add decoupling
- No movement: confirm pin is GPIO 4 (D2) and servo is functional

## Serial

- Use 9600 baud; open monitor after reset
- Check logs for errors from MQTT (`connectError()`)