# IoT-Based Energy Meter with Home Automation

This project is an IoT-based smart energy monitoring system built using **ESP32**, **Blynk**, **LCD display**, and **EmonLib**. It allows users to monitor voltage, current, power consumption (kWh), and electricity bills in real-time, and also control home appliances remotely via the Blynk app or manually using switches.

## 🔧 Features

- 📡 **Real-time monitoring** of:
  - Voltage (Vrms)
  - Current (Irms)
  - Apparent Power (Watt)
  - Energy Consumption (kWh)
  - Estimated Electricity Bill (BDT)
- 📱 **Remote Control** of 2 appliances via Blynk (Virtual Pins V6 and V7)
- 🔘 **Manual Control** using push buttons
- 💡 **Status Display** on 16x2 LCD
- 🔌 Wi-Fi and Blynk integration using ESP32
- 🔁 Auto-reconnect and Blynk sync on boot

## ⚙️ Hardware Used

- ESP32 Dev Board  
- ACS712 Current Sensor  
- Voltage Divider Circuit  
- 16x2 LCD with I2C or direct pins  
- Relay Module (4-channel)  
- Push Buttons (2x)  
- Wi-Fi (via ESP32)  
- Blynk IoT Platform  

## 📲 Mobile Control (Blynk App)

- Use Blynk Legacy App (or update to Blynk IoT if supported)
- Configure 2 buttons (V6, V7) to control Relay 1 and Relay 2
- Read voltage, current, power, units, and bill on virtual pins V0–V4

## 💡 Author

**Maruf Ahmed**  
Department of Electronics & Telecommunication Engineering  
Rajshahi University of Engineering & Technology (RUET)

---

