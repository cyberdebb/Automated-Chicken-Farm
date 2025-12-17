# Automated Chicken Farm

This repository contains the development and simulation of an **automated and sustainable chicken farm control system**, designed to manage environmental conditions, feeding, and lighting automatically.

The project was developed using **Proteus 8 Professional** for simulation and **MPLAB X IDE v5.35** for firmware development in **C**, targeting a microcontroller-based embedded system.

---

## Project Overview

The Automated Chicken Farm aims to ensure **animal welfare and sustainability** by automating essential farm processes such as:

- Lighting cycles
- Water and food distribution
- Temperature monitoring and control
- Safety alerts for critical temperature conditions
- System reliability using a watchdog timer

The repository includes:
- Proteus simulation files (`.pdsprj`)
- C source code (`.X` project folder)
- PCB schematic design files

---

## System Features

### Lighting Control
- 16 hours of continuous light
- 8 hours of darkness
- Automatic switching based on night sensor input

### Feeding and Water Supply
- Water and food available continuously
- Automatic replenishment every hour
- Motors controlled with position sensors (open/closed)

### Temperature Control
- Ideal operating range: **18°C to 28°C**
- Cooling system activated at high temperatures
- Heating system activated at low temperatures
- Audible alarm triggered when:
  - Temperature > **45°C**
  - Temperature < **5°C**

### Reliability
- Use of **Watchdog Timer** to ensure system stability and continuous operation

---

## Inputs

| Signal | Description |
|------|------------|
| RB0 | Night sensor (interrupt) |
| RB1 | Water motor – open sensor |
| RB2 | Water motor – closed sensor |
| RB3 | Food motor – open sensor |
| RB4 | Food motor – closed sensor |
| RB5 | Water supply sensor |
| RB6 | Food supply sensor |
| RA0 | Temperature sensor (analog) |
| RA1 | Ambient light sensor (analog) |

---

## Outputs

| Signal | Description |
|------|------------|
| RC0, RC1 | Water dispensing motor control |
| RC2, RC3 | Food dispensing motor control |
| RC4 | Cooling system activation |
| RC5 | Heating system activation |
| RC6 | Lighting control |
| RC7 | Supply refill warning LED |
| RB7 | Buzzer for critical temperature alert |

---

## LCD Display

**Pins:** RD2 – RD7  

Displayed information:
- Current temperature
- Amount of natural lighting detected

---

## Timers

- **Hourly check** to verify food and water availability
- Automatic dispensing of food and water for **30 seconds** when required

---

## Interruptions

- **RB0 Interrupt (Night Sensor):**
  - Turns on farm lighting
  - Disables food dispensing timer during night period

- **Timer1 Interrupt:**
  - Handles periodic supply replenishment logic

---

## Tools and Technologies

- **Simulation:** Proteus 8 Professional  
- **IDE:** MPLAB X IDE v5.35  
- **Language:** C  
- **Domain:** Embedded Systems / Automation  

---

## Schematic Capture

![projetofinal_pdf_page-0001](https://github.com/user-attachments/assets/0be72835-2a52-4e79-94d5-7f9f70842c0a)

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.
