# FreeRTOS and Bare-Metal Microcontroller Concepts (C, C++, AVR)

## Overview

A collection of low-level microcontroller projects implemented in C/C++ for the AVR architecture (ATmega2560). 
The repository demonstrates advanced embedded programming techniques, including real-time task scheduling with FreeRTOS and bare-metal hardware manipulation via direct register access.

The projects focus on time-critical operations, inter-task synchronization, hardware interrupts, and peripheral interfacing.

---

## Features

### Real-Time Operating System (RTOS)
- Multi-tasking implementation using **FreeRTOS**
- Inter-task synchronization via **Binary Semaphores**
- Non-blocking task delays using `vTaskDelay` for efficient CPU utilization
- Task priority management and scheduling

### Bare-Metal Hardware Manipulation
- Direct configuration of **8-bit and 16-bit Hardware Timers**
- Implementation of **CTC (Clear Timer on Compare Match)** mode for precision timing
- Manual configuration of Prescalers and Compare Values
- Custom **Interrupt Service Routines (ISR)** for asynchronous event handling

### Peripheral Interfacing & Storage
- Non-volatile data persistence using **EEPROM**
- Analog-to-Digital Conversion (ADC) for sensor data acquisition
- Pulse Width Modulation (PWM) for actuator control (Servos)
- Communication with external displays (LCD via 4-bit mode) and digital sensors (DHT22)

---

## Projects Overview

### 1. RTOS Voltage Monitor
A pipeline-based monitoring system synchronized via semaphores.
- **Architecture:** 3-task execution flow (Measure 1 -> Measure 2 -> Process/Compare).
- **Logic:** Compares absolute voltage differences against thresholds stored in EEPROM.
- **Synchronization:** Binary semaphores ensure sequential execution and data integrity between tasks.

### 2. Timer Climate Control
A bare-metal climate regulation system utilizing hardware interrupts.
- **Logic:** Periodically reads temperature and humidity data using Timer 2.
- **Hardware Control:** Directly manipulates timer registers to trigger measurements every 1500ms without blocking the main loop.
- **User Interface:** External button-triggered ISR for system state toggling.

### 3. Precision Timer Stopwatch
A high-accuracy stopwatch implementation using Timer 0.
- **Logic:** Configured for 1ms intervals using a 64-prescaler on a 16MHz clock.
- **Accuracy:** Utilizes CTC mode to ensure no drift in time measurements.
- **Control:** Hardware interrupts handle Start/Pause and Reset functionalities.

---

## Technical Specifications (Registers Used)

The projects involve direct manipulation of the following AVR registers:

- **TCCR0A/B, TCCR2A/B** – Timer/Counter Control Registers
- **OCR0A, OCR2A** – Output Compare Registers
- **TIMSK0, TIMSK2** – Timer Interrupt Mask Registers
- **EICRA, EIMSK** – External Interrupt Control and Mask Registers
- **EEAR, EEDR, EECR** – EEPROM Access Registers

---

## Architecture Overview

The projects follow an interrupt-driven and task-oriented architecture:

**Event/Timer Trigger → Interrupt Service Routine (ISR) → Semaphore/Flag Update → Task/Logic Execution → Peripheral Output**

---

## Requirements

### Hardware
- ATmega2560 (Arduino Mega) or compatible AVR microcontroller
- Sensors: DHT22, Potentiometers
- Actuators: Servo motors, Relays
- Display: Hitachi HD44780 compatible LCD

### Software/Tools
- AVR-GCC Compiler
- Arduino IDE (for Framework support)
- FreeRTOS Library
- Proteus Design Suite (for simulation)

---

## Build & Deployment

### Compilation
The projects are structured as `.ino` files but follow strict C/C++ standards. They can be compiled using the Arduino CLI or IDE with the FreeRTOS library installed.

### Simulation
Proteus project files (`.pdsprj`) are included in original project directories to demonstrate the hardware-software interaction in a simulated environment.

---

## Purpose

The primary goal of this repository is to demonstrate an understanding of microcontroller internals, real-time constraints, and the transition from high-level abstraction to low-level hardware control.
