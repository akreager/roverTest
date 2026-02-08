# Rover Test Platform
Engineering test platform for developing RC rover control and telemetry. Pairs with the [RC Transmitter](https://github.com/akreager/rc-transmitter).

![Front](docs/images/MVIMG_20200410_220730.jpg?raw=true)

## Overview
A small-scale test rover used to develop and validate control, telemetry, and safety features before deploying them on full-size vehicles (e.g. RC snowblower). Receives wireless commands from a custom pistol-grip transmitter and sends back real-time sensor data via nRF24L01+ acknowledge payloads.

### Features
- **Wireless control** via nRF24L01+ with bidirectional telemetry
- **Mixed-mode drive** — throttle and steering via RoboClaw motor controller
- **Telemetry reporting** — battery voltage, board temperatures, motor currents sent back to transmitter each cycle
- **Connection failsafe** — motors stop automatically if radio link is lost for >1 second
- **Motor stop** (M-Stop) — responds to transmitter's M-Stop flag, controls motor enable/disable via digital pin

### Planned
- Joystick-aimed chute control (additional servo/actuator channels)
- Per-vehicle configuration via SD card profiles on transmitter side

## Hardware
- Arduino Mega
- nRF24L01+ radio module (SPI, CE=D49, CSN=D53)
- [Basicmicro RoboClaw](https://www.basicmicro.com/) motor controller (Serial1, address 0x80, 38400 baud)
- Motor enable/disable on D7

## Project Structure
```
RoverTest/
├── main/                 # Receiver firmware (Arduino IDE)
│   └── main.ino          # Radio receive, RoboClaw control, telemetry, failsafe
├── test_sketches/        # Standalone test/reference sketches
│   ├── simpleJoystick/   # Raw joystick → serial output
│   ├── roverJoystick/    # Direct joystick → RoboClaw
│   ├── roverJoyRead/     # Joystick + RoboClaw telemetry → LCD
│   ├── roverRX/          # nRF24 receiver + RoboClaw (precursor to main)
│   ├── roverReceiver/    # Joystick → RoboClaw variant
│   ├── roverRead/        # RoboClaw telemetry → serial LCD
│   ├── roverReadVersion/ # RoboClaw firmware version check
│   ├── PacketSerialMixedPWM/ # RoboClaw mixed mode basic test
│   ├── LCD_Test/         # Serial LCD test
│   └── while/            # LCD + LED loop test
├── stl/                  # 3D printed chassis files
├── docs/                 # Documentation and images
├── LICENSE               # GPLv3
└── README.md
```

## Radio Protocol
The rover receives a `dataPackage` and responds with an `ackPackage` via nRF24L01+ acknowledge payloads. These structs must match the transmitter exactly.

```c
// Transmitter → Rover
struct dataPackage {
  byte throttle;        // 0-127, 64 = stop (RoboClaw mixed mode)
  byte steering;        // 0-127, 64 = center
  bool data_motor_stop; // true = motors disabled
};

// Rover → Transmitter (acknowledge payload)
struct ackPackage {
  byte highBatt, lowBatt;     // Battery voltage, tenths of a volt
  byte highTemp1, lowTemp1;   // Board temp 1, tenths of a degree C
  byte highTemp2, lowTemp2;   // Board temp 2, tenths of a degree C
  byte highIM1, lowIM1;       // Motor 1 current, hundredths of an amp
  byte highIM2, lowIM2;       // Motor 2 current, hundredths of an amp
  bool ack_motor_stop;        // Current motor stop state
};
```

## Dependencies
Install via Arduino IDE Library Manager:
- [RF24](https://github.com/tmrh20/RF24/) (TMRh20 fork)
- [RoboClaw Arduino Library](https://github.com/basicmicro/roboclaw_arduino_library)

## Development
This project is developed with assistance from Claude (Anthropic), used for code architecture, refactoring, and documentation.

## License
This project is licensed under the GNU General Public License v3.0 — see [LICENSE](LICENSE) for details.