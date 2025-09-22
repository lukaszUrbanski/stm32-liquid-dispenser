# Automatic Drink Dispenser

An embedded project built on **STM32 Nucleo-F411RE**, designed to control a drink dispensing machine with TFT display, push buttons, rotary encoder, flow sensor, and relay-controlled pump.  
The project starts as a **state machine implementation** and will later be migrated to **FreeRTOS** for multitasking.

---

## ✨ Features

- **320x240 TFT display** for GUI (volume, status, user options).
- **Push buttons** (Start / Stop / Back) for basic control.
- **Rotary encoder** with push button for parameter adjustment.
- **Relay module** to control the water pump.
- **YF-S201 flow sensor** for measuring dispensed volume.
- Configurable **target volume (ml)** with safety **max pour time**.
- Modular code structure with clean separation of:
  - State machine
  - Display driver
  - Input handling
  - Flow measurement

---

## 📐 System Architecture

- **MCU:** STM32F411RE (Nucleo board)
- **Display:** TFT 320x240, parallel/SPI interface
- **Inputs:**
  - 3 push buttons (Start / Stop / Back)
  - Rotary encoder with push button
- **Outputs:**
  - Relay module for pump
- **Sensors:**
  - YF-S201 flow sensor (Hall-effect)

---

## 🧩 Software Design

- **Phase 1:** State machine (FSM) implementation  
- **Phase 2:** Migration to **FreeRTOS** with tasks:
  - Display/UI Task
  - Input Task
  - Flow Measurement Task
  - Control Task (FSM)

---
