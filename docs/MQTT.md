# MQTT Topics and Payloads

## Topics

- Subscribe (commands): `inTopic`
- Publish (telemetry): `outTopic`

These identifiers are configured in `esp8266_robot.ino`.

## Telemetry Payload (Publish → `outTopic`)

Example:

```json
{
  "params": {
    "temp": 27,
    "humi": 62
  },
  "vision": "1.0.0"
}
```

- `temp`: temperature from DHT11 (°C)
- `humi`: relative humidity (%)
- `vision`: string as implemented in the sketch

## Command Payload (Subscribe ← `inTopic`)

Command schema controls the servo using `items.led.value`:

```json
{
  "items": {
    "led": {
      "value": 0
    }
  }
}
```

- `value == 0` → servo angle 0°
- `value != 0` → servo angle 90°

### Example using mosquitto_pub

```bash
mosquitto_pub -h <broker> -t "/sys/<productKey>/<deviceName>/thing/service/property/set" -m '{"items":{"led":{"value":1}}}'
```

### Example using Python (paho-mqtt)

```python
import json
import paho.mqtt.client as mqtt

client = mqtt.Client()
client.username_pw_set("<username>", "<password>")
client.connect("<broker>", 1883)

payload = {"items": {"led": {"value": 1}}}
client.publish("/sys/<productKey>/<deviceName>/thing/service/property/set", json.dumps(payload), qos=1)
client.loop()
```