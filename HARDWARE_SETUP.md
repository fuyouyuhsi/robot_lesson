# Hardware Setup Guide

This document provides comprehensive hardware setup instructions, wiring diagrams, component specifications, and assembly guidelines for the ESP8266 IoT Robot project.

## Table of Contents

- [Component List](#component-list)
- [Pin Configuration](#pin-configuration)
- [Wiring Diagrams](#wiring-diagrams)
- [Assembly Instructions](#assembly-instructions)
- [Power Requirements](#power-requirements)
- [Component Specifications](#component-specifications)
- [Hardware Troubleshooting](#hardware-troubleshooting)
- [PCB Design Considerations](#pcb-design-considerations)
- [Enclosure and Mounting](#enclosure-and-mounting)

## Component List

### Required Components

| Component | Quantity | Purpose | Notes |
|-----------|----------|---------|-------|
| ESP8266 Development Board | 1 | Main controller | NodeMCU v3, Wemos D1 Mini, or similar |
| DHT11 Sensor Module | 1 | Temperature/Humidity sensing | 3-pin module preferred |
| SG90 Servo Motor | 1 | Actuator control | 180° rotation servo |
| Breadboard (Half-size) | 1 | Prototyping connections | 400 tie points minimum |
| Jumper Wires (M-M) | 10+ | Connections | Various colors recommended |
| Jumper Wires (M-F) | 5+ | Sensor connections | For module connections |
| USB Cable (Micro/Type-C) | 1 | Programming and power | Match ESP8266 board |

### Optional Components

| Component | Quantity | Purpose | Notes |
|-----------|----------|---------|-------|
| 10kΩ Resistor | 1 | DHT11 pull-up | Built-in on most modules |
| 0.1µF Capacitor | 1 | Power filtering | Ceramic capacitor |
| LED | 1 | Status indicator | 3mm or 5mm |
| 220Ω Resistor | 1 | LED current limiting | For status LED |
| Push Button | 1 | Reset/Test button | Momentary switch |
| External Power Supply | 1 | Servo power | 5V, 1A minimum |

## Pin Configuration

### ESP8266 Pin Assignments

| Function | ESP8266 Pin | GPIO Number | Physical Pin | Wire Color |
|----------|-------------|-------------|--------------|------------|
| DHT11 Data | D1 | GPIO 5 | Pin 5 | Yellow |
| Servo Signal | D2 | GPIO 4 | Pin 4 | Orange |
| VCC (3.3V) | 3V3 | - | 3.3V | Red |
| Ground | GND | - | GND | Black |
| USB Power | VIN | - | VIN | Red (5V) |

### DHT11 Sensor Pinout

```
DHT11 Module (3-pin)
┌─────────────┐
│ VCC DATA GND│
│  │   │    │ │
│  1   2    3 │
└─────────────┘

Pin 1 (VCC):  3.3V or 5V power
Pin 2 (DATA): Digital data signal
Pin 3 (GND):  Ground connection
```

### SG90 Servo Pinout

```
SG90 Servo
┌─────────────┐
│    Brown    │ ── GND (Ground)
│   Red       │ ── VCC (5V Power)
│  Orange     │ ── Signal (PWM Control)
└─────────────┘
```

## Wiring Diagrams

### Complete Circuit Diagram

```
ESP8266 NodeMCU                    DHT11 Module
┌─────────────┐                   ┌─────────────┐
│           D1├───────────────────┤DATA      VCC├──┐
│           D2├──┐                │            GND├──┼──┐
│          3V3├──┼────────────────┴─────────────────┘  │  │
│          GND├──┼───────────────────────────────────── ┴──┘
│          VIN├──┼──┐                                       │
└─────────────┘  │  │              SG90 Servo              │
                 │  │             ┌─────────────┐           │
                 │  └─────────────┤Orange    Red├───────────┘
                 └────────────────┤Brown        │
                                  └─────────────┘
```

### Breadboard Layout

```
Breadboard Connection Layout:

    Power Rails           Main Area
   + - - - - - -         1  2  3  4  5
   │ │                   ┊  ┊  ┊  ┊  ┊
   │ └─── GND            a  b  c  d  e
   └───── 3.3V           ┊  ┊  ┊  ┊  ┊
                         f  g  h  i  j
                         ┊  ┊  ┊  ┊  ┊
   Component Placement:
   
   Row 1-10:  ESP8266 NodeMCU (straddling center)
   Row 15:    DHT11 Module
   Row 20:    Servo connector
   
   Power Distribution:
   - Red rail: 3.3V from ESP8266
   - Black rail: GND from ESP8266
```

### Step-by-Step Wiring

#### Step 1: Power Connections
```
1. Connect ESP8266 3.3V to breadboard positive rail (red)
2. Connect ESP8266 GND to breadboard negative rail (black)
3. Connect DHT11 VCC to positive rail
4. Connect DHT11 GND to negative rail
5. Connect Servo Red wire to ESP8266 VIN (5V)
6. Connect Servo Brown wire to negative rail
```

#### Step 2: Signal Connections
```
1. Connect DHT11 DATA pin to ESP8266 D1 (GPIO 5)
2. Connect Servo Orange wire to ESP8266 D2 (GPIO 4)
```

#### Step 3: Verification
```
1. Check all power connections for correct polarity
2. Verify signal wire connections match pin definitions
3. Ensure no short circuits between power rails
4. Test continuity with multimeter if available
```

## Assembly Instructions

### Basic Assembly Process

#### Phase 1: Component Preparation
1. **Inspect Components**
   - Check ESP8266 board for damage
   - Verify DHT11 sensor functionality
   - Test servo motor movement manually
   - Sort jumper wires by color

2. **Prepare Workspace**
   - Clean, static-free work surface
   - Good lighting and magnification
   - Anti-static wrist strap (recommended)
   - Component organizer tray

#### Phase 2: Initial Connections
1. **Mount ESP8266 on Breadboard**
   ```
   - Place NodeMCU across center divide
   - Ensure all pins are properly seated
   - Verify board sits flat and stable
   - Label pin functions with tape if needed
   ```

2. **Install DHT11 Sensor**
   ```
   - Insert DHT11 module in designated row
   - Ensure proper orientation (check pinout)
   - Make connections as per wiring diagram
   - Test fit before final insertion
   ```

3. **Connect Servo Motor**
   ```
   - Prepare servo wire connections
   - Use male-to-female jumpers if needed
   - Secure connections to prevent disconnection
   - Route wires neatly to avoid interference
   ```

#### Phase 3: Power and Signal Routing
1. **Power Distribution**
   ```
   - Install power rail connections first
   - Use red wire for positive connections
   - Use black wire for ground connections
   - Double-check polarity before powering on
   ```

2. **Signal Wire Routing**
   ```
   - Keep signal wires away from power wires
   - Use appropriate wire colors for identification
   - Maintain neat, organized layout
   - Leave slack for adjustments
   ```

### Advanced Assembly Options

#### Permanent Installation
For permanent installations, consider:

1. **Perfboard Construction**
   ```
   - Transfer breadboard layout to perfboard
   - Use solder connections for reliability
   - Add connector blocks for external components
   - Include test points for debugging
   ```

2. **Custom PCB Design**
   ```
   - Create schematic in EDA software
   - Design PCB layout with proper spacing
   - Include mounting holes and connectors
   - Add power indicators and test points
   ```

## Power Requirements

### Power Analysis

#### ESP8266 Power Consumption
- **Operating Voltage**: 3.0V - 3.6V
- **Peak Current (WiFi TX)**: 170mA
- **Average Current (Active)**: 80mA
- **Sleep Mode Current**: 20µA
- **Deep Sleep Current**: 10µA

#### DHT11 Sensor Power
- **Operating Voltage**: 3.0V - 5.5V
- **Active Current**: 0.3mA
- **Standby Current**: 60µA
- **Reading Duration**: 2 seconds max

#### SG90 Servo Power
- **Operating Voltage**: 4.8V - 6.0V
- **Stall Current**: 1.6A (maximum)
- **No-load Current**: 220mA
- **Holding Current**: 4-7mA

### Power Supply Options

#### Option 1: USB Power Only
```
Suitable for: Development and testing
Limitations: Limited servo torque
Configuration:
- ESP8266 powered from USB (5V → 3.3V regulator)
- DHT11 powered from ESP8266 3.3V
- Servo powered from ESP8266 VIN (USB 5V)
```

#### Option 2: External 5V Supply
```
Suitable for: Production use with full servo power
Components: 5V, 2A power adapter
Configuration:
- Main 5V supply for servo and ESP8266
- ESP8266 internal regulator provides 3.3V
- DHT11 powered from 3.3V rail
```

#### Option 3: Dual Supply System
```
Suitable for: Maximum performance and reliability
Components: 3.3V and 5V regulated supplies
Configuration:
- Dedicated 3.3V supply for ESP8266 and DHT11
- Separate 5V supply for servo motor
- Isolated grounds with common reference
```

### Power Distribution Schematic

```
Power Distribution Network:

USB/External 5V ──┬── ESP8266 VIN
                  │
                  ├── Servo VCC (Red)
                  │
                  └── Optional: External 5V rail

ESP8266 3.3V ────┬── DHT11 VCC
                 │
                 └── Optional: 3.3V devices

Common GND ──────┴── All component grounds
```

## Component Specifications

### ESP8266 Development Boards

#### NodeMCU v3 Specifications
- **Microcontroller**: ESP8266-12E
- **Flash Memory**: 4MB
- **RAM**: 96KB
- **GPIO Pins**: 13 available
- **ADC**: 1 channel, 10-bit
- **PWM**: Software PWM on any GPIO
- **I2C/SPI**: Software implementation
- **Dimensions**: 58mm × 31mm × 13mm
- **Weight**: 10g

#### Wemos D1 Mini Specifications
- **Microcontroller**: ESP8266-12F
- **Flash Memory**: 4MB
- **RAM**: 96KB
- **GPIO Pins**: 11 available
- **Dimensions**: 34.2mm × 25.6mm × 4mm
- **Weight**: 3g
- **Advantages**: Compact size, breadboard-friendly

### DHT11 Sensor Specifications

#### Technical Specifications
- **Humidity Range**: 20-80% RH
- **Humidity Accuracy**: ±5% RH
- **Temperature Range**: 0-50°C
- **Temperature Accuracy**: ±2°C
- **Resolution**: 1% RH, 1°C
- **Response Time**: <5 seconds
- **Sampling Rate**: 1Hz (1 reading per second)
- **Power Supply**: 3.0-5.5V DC
- **Current Consumption**: 0.3mA (measuring), 60µA (standby)

#### Physical Specifications
- **Dimensions**: 15.5mm × 12mm × 5.5mm
- **Pin Spacing**: 2.54mm (0.1")
- **Operating Temperature**: 0-50°C
- **Storage Temperature**: -20-60°C
- **Humidity Range**: 5-95% RH (non-condensing)

### SG90 Servo Specifications

#### Technical Specifications
- **Operating Voltage**: 4.8V - 6.0V
- **Stall Torque**: 1.8 kg⋅cm (25 oz⋅in) at 4.8V
- **Operating Speed**: 0.1 sec/60° at 4.8V
- **Rotation Range**: 180° (±90°)
- **PWM Period**: 20ms (50Hz)
- **Pulse Width**: 1-2ms (1ms=0°, 1.5ms=90°, 2ms=180°)
- **Dead Band**: 10µs
- **Weight**: 9g

#### Physical Specifications
- **Dimensions**: 22mm × 11.5mm × 22.5mm
- **Shaft Diameter**: 4mm splined
- **Mount Holes**: 2.5mm diameter, 28mm spacing
- **Wire Length**: 150mm
- **Connector**: Standard 3-pin servo connector

### Connection Specifications

#### Servo PWM Signal Requirements
```cpp
// PWM characteristics for SG90:
// Period: 20ms (50Hz)
// Pulse width range: 1-2ms
// Resolution: Depends on PWM timer resolution

Position Control:
  0°   →  1.0ms pulse width
  45°  →  1.25ms pulse width  
  90°  →  1.5ms pulse width
  135° →  1.75ms pulse width
  180° →  2.0ms pulse width
```

#### DHT11 Communication Protocol
```
Data Format: 40-bit data stream
- 8-bit humidity integer
- 8-bit humidity decimal  
- 8-bit temperature integer
- 8-bit temperature decimal
- 8-bit checksum

Timing Requirements:
- Start signal: Pull low for >18ms
- Response: 80µs low, 80µs high
- Data bit 0: 50µs low, 26-28µs high
- Data bit 1: 50µs low, 70µs high
```

## Hardware Troubleshooting

### Common Issues and Solutions

#### Power Issues

**Problem**: ESP8266 doesn't power on
```
Symptoms:
- No LED indicators
- No serial output
- Device not recognized by computer

Solutions:
1. Check USB cable connectivity
2. Verify power supply voltage (should be 5V for USB)
3. Test different USB port/computer
4. Check for short circuits on breadboard
5. Measure voltage at ESP8266 VIN pin
```

**Problem**: Servo doesn't move or moves erratically
```
Symptoms:
- Servo jitters or doesn't respond
- ESP8266 resets when servo moves
- Inconsistent servo positioning

Solutions:
1. Check servo power supply (needs 5V, adequate current)
2. Add bulk capacitor (470µF) across servo power
3. Use external 5V supply for servo
4. Verify PWM signal with oscilloscope
5. Check for loose connections
```

#### Communication Issues

**Problem**: DHT11 returns invalid readings
```
Symptoms:
- Temperature readings of -999 or extreme values
- Humidity readings outside 0-100% range
- Intermittent sensor failures

Solutions:
1. Check wiring connections (VCC, GND, DATA)
2. Verify pin assignment matches code
3. Add 10kΩ pull-up resistor to data line
4. Ensure 2+ second intervals between readings
5. Test with different DHT11 module
6. Check for electromagnetic interference
```

**Problem**: WiFi connection fails
```
Symptoms:
- Cannot connect to network
- Frequent disconnections
- Weak signal strength

Solutions:
1. Verify SSID and password
2. Check network compatibility (2.4GHz only)
3. Move closer to WiFi router
4. Add external antenna if available
5. Check for power supply issues
6. Test with different network
```

#### Signal Integrity Issues

**Problem**: Servo position inaccuracy
```
Symptoms:
- Servo doesn't reach commanded position
- Position drift over time
- Non-linear response

Solutions:
1. Calibrate servo endpoints in software
2. Check PWM signal timing with oscilloscope
3. Verify servo mechanical condition
4. Add external pull-up/pull-down resistors
5. Use dedicated servo driver IC for precision
```

**Problem**: DHT11 communication errors
```
Symptoms:
- Checksum errors
- Timeout errors
- Partial data reception

Solutions:
1. Reduce wire length between ESP8266 and DHT11
2. Add decoupling capacitor near DHT11
3. Shield data wire from interference
4. Use twisted pair for long connections
5. Implement software error handling/retry
```

### Diagnostic Procedures

#### Basic Connectivity Test
```cpp
// Test sequence for hardware verification
void hardwareDiagnostic() {
    Serial.println("=== Hardware Diagnostic ===");
    
    // Test 1: Power levels
    Serial.print("VCC Level: ");
    Serial.println(analogRead(A0) * 3.3 / 1024.0);
    
    // Test 2: DHT11 presence
    Serial.print("DHT11 Test: ");
    DHT.read(DHT11_PIN);
    if (DHT.temperature > -40 && DHT.temperature < 80) {
        Serial.println("PASS");
    } else {
        Serial.println("FAIL - Check wiring");
    }
    
    // Test 3: Servo response
    Serial.print("Servo Test: ");
    myServo.write(90);
    delay(500);
    myServo.write(0);
    delay(500);
    myServo.write(180);
    delay(500);
    myServo.write(90);
    Serial.println("COMPLETE - Observe movement");
    
    Serial.println("=== Diagnostic Complete ===");
}
```

#### Signal Quality Measurement
```cpp
// Measure signal timing and quality
void signalQualityTest() {
    // Test PWM output stability
    unsigned long startTime = micros();
    for (int i = 0; i < 100; i++) {
        digitalWrite(4, HIGH);
        delayMicroseconds(1500); // 90° position
        digitalWrite(4, LOW);
        delayMicroseconds(18500); // Complete 20ms period
    }
    unsigned long duration = micros() - startTime;
    
    Serial.print("PWM Timing Test: ");
    Serial.print(duration / 100.0);
    Serial.println("µs average period");
    
    // Expected: ~20000µs (20ms)
    // Tolerance: ±50µs for stable servo operation
}
```

## PCB Design Considerations

### Layout Guidelines

#### Component Placement
```
Recommended PCB Layout:

┌─────────────────────────────┐
│  [PWR]     [ESP8266]   [DHT]│
│           │         │      │
│  [SERVO]  │  [USB]  │ [LED]│
│           │         │      │
│  [GND]    [BUTTONS] [TEST] │
└─────────────────────────────┘

Zones:
- Power section (top-left)
- MCU section (center)
- Sensor section (top-right)
- I/O section (bottom)
```

#### Trace Routing
- **Power traces**: Minimum 0.5mm width
- **Signal traces**: 0.2-0.3mm width
- **Ground plane**: Use pour for EMI reduction
- **Via size**: 0.3mm drill, 0.6mm pad
- **Keep-out zones**: 5mm around antenna area

#### Layer Stack-up (2-layer PCB)
```
Layer 1 (Top):
- Component placement
- Signal routing
- Power distribution

Layer 2 (Bottom):
- Ground plane
- Return paths
- Additional routing if needed
```

### Manufacturing Specifications

#### Standard PCB Parameters
- **Board Thickness**: 1.6mm
- **Copper Weight**: 1oz (35µm)
- **Solder Mask**: Green (LPI)
- **Silkscreen**: White
- **Finish**: HASL or ENIG
- **Minimum Track/Space**: 0.1mm/0.1mm
- **Minimum Via**: 0.15mm drill

#### Design Rules
```
Electrical:
- Maximum trace resistance: 1Ω
- Minimum clearance: 0.15mm
- Via current capacity: 1A per via

Mechanical:
- Board outline tolerance: ±0.1mm
- Hole position tolerance: ±0.05mm
- Component placement accuracy: ±0.1mm
```

## Enclosure and Mounting

### Enclosure Requirements

#### Environmental Protection
- **IP Rating**: IP20 minimum (finger protection)
- **Material**: ABS plastic or aluminum
- **Ventilation**: Required for heat dissipation
- **Access**: USB port and status indicators
- **Mounting**: Standard threaded inserts

#### Mechanical Specifications
```
Minimum Internal Dimensions:
- Length: 100mm
- Width: 70mm
- Height: 40mm
- PCB mounting: M3 standoffs, 5mm height

External Features:
- USB access port: 15mm × 8mm
- Servo cable gland: 8mm diameter
- Sensor access: 20mm × 15mm ventilation
- Status LED light pipe: 3mm diameter
```

### Mounting Options

#### Desktop/Lab Use
```
Configuration: Weighted base with rubber feet
Materials:
- Base plate: 150mm × 100mm × 10mm aluminum
- Rubber feet: 4× 15mm diameter
- PCB standoffs: M3 × 15mm nylon
- Cover: Clear acrylic for visibility
```

#### Wall Mount
```
Configuration: Wall bracket with adjustable angle
Materials:
- Bracket: Laser-cut steel, powder coated
- Mounting hardware: M6 × 50mm screws + anchors
- Adjustment range: ±30° vertical tilt
- Cable management: Integrated strain relief
```

#### Portable/Field Use
```
Configuration: Rugged case with battery pack
Materials:
- Case: IP65 rated plastic enclosure
- Battery: 18650 Li-ion, 3.7V, 3000mAh
- Power management: TP4056 charging circuit
- Interface: Magnetic charging connector
```

### Assembly Hardware

#### Fasteners and Hardware
| Item | Specification | Quantity | Purpose |
|------|---------------|----------|---------|
| M3 × 8mm Screws | Phillips head, stainless steel | 4 | PCB mounting |
| M3 × 15mm Standoffs | Nylon, female-female | 4 | PCB spacing |
| M3 Washers | Nylon, 6mm OD | 4 | Load distribution |
| Cable Glands | PG7, IP65 rated | 2 | Cable sealing |
| Rubber Feet | 15mm diameter, adhesive | 4 | Base isolation |

#### Tools Required
- **Screwdrivers**: Phillips #1, #2
- **Hex keys**: 2.5mm, 3mm
- **Wire strippers**: 20-26 AWG
- **Crimping tool**: For connector pins
- **Drill bits**: 3mm, 7mm, 15mm
- **Files**: Metal and plastic

This hardware documentation provides comprehensive guidance for building, testing, and deploying the ESP8266 IoT Robot hardware system with professional-grade reliability and maintainability.