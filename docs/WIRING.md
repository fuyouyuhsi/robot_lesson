# Wiring Guide

## Pins

- DHT11 data → GPIO 5 (D1 on many ESP8266 dev boards)
- Servo signal → GPIO 4 (D2)
- All grounds common

## Power

- Power the servo from a stable 5V supply capable of delivering peak current (≥1A recommended for typical small servos)
- Do not power the servo from the ESP8266 3.3V pin
- Connect the external 5V ground to ESP8266 ground

## DHT11

- VCC → 3.3V (or 5V per module specs)
- GND → GND
- DATA → GPIO 5 (D1)
- Use a pull-up (often included on modules)

## Servo

- Signal → GPIO 4 (D2)
- VCC → 5V external
- GND → Common ground

## Notes

- Keep leads short to reduce noise
- Add decoupling capacitors near the servo supply (e.g., 100 µF + 100 nF)
- Consider a level-safe connection; most hobby servos accept 3.3V signal