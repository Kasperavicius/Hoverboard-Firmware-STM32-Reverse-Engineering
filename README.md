> Custom firmware for generic hoverboard mainboards, extended with Arduino Uno motor control and ESP32 Bluetooth for mobile app communication.
 
---
 
## Project Gallery

 | | |
|---|---|
| ![Robot build](Images/image1%20Background%20Removed.png) | ![Electronics](Images/image3%20Background%20Removed.png) |
| ![Robot action](Images/image4%20Background%20Removed.png) | |

---
 
## What This Project Does
 
This project repurposes the mainboard, brushless hub motors, and battery from a cheap off-the-shelf hoverboard into a fully controllable **robot platform**.
 
The hoverboard hardware is surprisingly capable - the motors are powerful, the battery is large, and the mainboard already handles low-level motor driving. This firmware unlocks that hardware by replacing the original software with something you fully control.
 
On top of the base firmware, this build adds two extra layers:
 
1. **Arduino Uno + Motor Controller** — The Arduino acts as the brain, sending speed and direction commands to the hoverboard mainboard over UART. A separate motor controller board sits between the Arduino and the hoverboard, translating high-level commands into the PWM/UART signals the mainboard expects. This makes it easy to swap out control logic or add sensors without touching the hoverboard firmware itself.
2. **ESP32 Bluetooth Bridge** — An ESP32 module connects to the Arduino and exposes a Bluetooth interface. A companion mobile app connects to the ESP32 wirelessly and sends drive commands (forward, backward, turn, speed) in real time. The ESP32 receives these commands and forwards them to the Arduino over its serial connection.
 
---
 
## How the Firmware Hack Works
 
The original hoverboard firmware only knows how to balance a person standing on the board — not useful for a robot. This custom firmware replaces that logic entirely.
 
**What the original firmware does:**
- Reads gyroscope/accelerometer data from two sensor boards (one per wheel)
- Drives both motors to keep the board balanced
- Shuts down if it detects a fall or over-current
**What this firmware does instead:**
- Ignores the balancing sensors
- Exposes a UART interface (on the mainboard's sensor cable connectors) that accepts speed and steering commands directly
- Drives the left and right motors independently, allowing differential steering (like a tank)
- Maintains overcurrent protection and battery safety from the original hardware
The mainboard's STM32F103 microcontroller runs the custom firmware and listens for packets in the format:
 
```
[ Speed Left ]  [ Speed Right ]  (signed 16-bit integers, -1000 to +1000)
```
 
Positive values drive forward, negative values drive backward. Sending different values to left and right produces turns.
 
---
 
## Hardware Overview
 
### Hoverboard Mainboard
The mainboard breaks out two 4-pin sensor connectors, each carrying:
 
| Pin | Signal |
|-----|--------|
| 1 | GND |
| 2 | 12V / 15V |
| 3 | USART TX |
| 4 | USART RX |
 
Both USART2 and USART3 are available. PA2 and PA3 can also be used as 12-bit ADC inputs if needed.
 
Mainboard schematics (reverse engineered): [docs/20150722_hoverboard_sch.pdf](https://github.com/EFeru/hoverboard-firmware-hack-FOC/blob/main/docs/20150722_hoverboard_sch.pdf)
 
### Arduino Uno + Motor Controller
 
The Arduino Uno connects to the hoverboard mainboard via one of the USART ports (USART2). It sends speed packets at regular intervals (~20ms) calculated from the input it receives from the ESP32.
 
The motor controller board level-shifts signals and provides any additional buffering needed between the 5V Arduino and the 3.3V hoverboard mainboard logic.
 
**Wiring summary:**
 
| Arduino | Hoverboard Mainboard |
|---------|----------------------|
| TX (pin 1) | USART2 RX |
| RX (pin 0) | USART2 TX |
| GND | GND |
| *(do not connect 5V to mainboard)* | — |
 
### ESP32 Bluetooth Module
 
The ESP32 connects to the Arduino via a second hardware serial port (or SoftwareSerial). It runs as a **Bluetooth Classic SPP (Serial Port Profile)** server. The mobile app pairs with the ESP32 and sends simple text commands which the ESP32 relays to the Arduino.
 
**Command protocol (ESP32 → Arduino):**
 
```
F<speed>    Forward at speed (0–100)
B<speed>    Backward at speed (0–100)
L<speed>    Turn left
R<speed>    Turn right
S           Stop
```
 
The Arduino parses these commands and maps them to the left/right motor speed values sent to the hoverboard mainboard.
 
---
 
## Mobile App
 
The companion app **Serial Bluetooth Terminal** connects to the ESP32 over Bluetooth and provides a simple interface for driving the robot.
 
![Serial Bluetooth Terminal app](Images/Serial%20Bluetooth%20Terminal.png)
---
 
## Flashing the Firmware
 
### Build
 
```bash
make
```
 
Make sure `PREFIX` in the Makefile points to your `gcc-arm-none-eabi` installation. Version 7 is known to work — use the [official ARM toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) if the Linux repo version fails.
 
### Connect the Programmer
 
There is a 4-pin SWD debug header next to the STM32 on the mainboard:
 
| Pin | Signal |
|-----|--------|
| 1 | GND |
| 2 | 3.3V *(do not power from here)* |
| 3 | SWDIO |
| 4 | SWCLK |
 
Connect GND, SWDIO, and SWCLK to your ST-Link programmer. **Do not power the mainboard from the programmer's 3.3V — this will kill the board.** The battery (>36V) must be connected during flashing.
 
Hold the power button (or jumper the power pins) while flashing, otherwise the STM32 may cut power to itself mid-flash.
 
### Unlock the Flash
 
```bash
openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg \
  -c init -c "reset halt" -c "stm32f1x unlock 0"
```
 
If that fails, try the extended unlock sequence:
 
```bash
openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg \
  -c init -c "reset halt" \
  -c "mww 0x40022004 0x45670123" -c "mww 0x40022004 0xCDEF89AB" \
  -c "mww 0x40022008 0x45670123" -c "mww 0x40022008 0xCDEF89AB" \
  -c targets -c "halt" -c "stm32f1x unlock 0"
```
 
### Flash
 
```bash
st-flash --reset write build/hover.bin 0x8000000
```
 
Or with OpenOCD:
 
```bash
openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg \
  -c "flash write_image erase build/hover.bin 0x8000000"
```
 
---
 
 
## Configuration
 
Edit `Inc/config.h` to match your build:
 
- Motor direction (invert left/right if your robot drives backward)
- UART baud rate
- Maximum speed limits
- Input deadzone for the joystick
---
 
## Related Projects & Resources
 
- [Original hoverboard-firmware-hack](https://github.com/NiklasFauth/hoverboard-firmware-hack) — the base this project builds on
- [UART control example (Arduino)](https://github.com/p-h-a-i-l/hoverboard-firmware-hack) — Arduino UART communication reference
- [BiPropellant fork](https://github.com/bipropellant) — focuses on reliable machine control
- [Gen2 split mainboard hack](https://github.com/flo199213/Hoverboard-Firmware-Hack-Gen2) — for boards with separate left/right mainboards
- [Project talk video (~40 min)](https://media.ccc.de/v/gpn18-95-howto-moving-objects) — overview of hoverboard hacking
---
 
## License
 
Open source. See original project for license details.