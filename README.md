# Adaptive Ambient & Touchless Control Hub 🚀

An autonomous **Edge Computing IoT System** built with ESP32 that integrates environmental monitoring, web-configurable alarms, and touchless IR sensor gesture controls without relying on external cloud infrastructure.

---

## 🌟 Key Features

* **Edge Computing Architecture:** All sensor processing, logic, and web server rendering occur locally on the ESP32 with zero internet or cloud dependency.
* **Environmental Monitoring:** Real-time temperature and humidity tracking (DHT11) alongside air pollution and gas detection (MQ-135).
* **Single-Color LED Visual States:**
  * 🟢 **Solid Green:** System Normal / Safe
  * 🔵 **Solid Blue:** High Humidity / Temperature Warning
  * 🔴 **Solid Red:** Gas Hazard / Active Alarm
* **Touchless IR Gesture Dismissal:** Wave a hand in front of the IR sensor to immediately silence active alarms or hazard beepers.
* **Embedded Web Control Hub:** Local Wi-Fi Access Point (`192.168.4.1`) hosting an auto-refreshing dashboard to sync system clocks, view live sensor metrics, and program daily alarm presets.

---

## 🛠️ Hardware Stack

| Component | Pin / Type | ESP32 GPIO Pin | Description |
| :--- | :--- | :--- | :--- |
| **DHT11 Sensor** | Data Pin | **GPIO 23** | Temperature & Humidity Sensor |
| **MQ-135 Sensor** | Analog Output (AO) | **GPIO 34** | Air Quality & Gas Sensor |
| **IR Proximity Sensor**| Digital Output (OUT)| **GPIO 13** | Touchless Gesture / Silence Switch |
| **Active-LOW Buzzer** | Positive Signal | **GPIO 18** | Audible Hazard Beeper |
| **RGB LED (Red)** | Anode / Cathode | **GPIO 21** | Gas Hazard & Alarm Indicator |
| **RGB LED (Green)**| Anode / Cathode | **GPIO 19** | Normal / Safe System State |
| **RGB LED (Blue)** | Anode / Cathode | **GPIO 22** | Humidity & Temperature Warning |

---

## 📐 Circuit Schematic & Wiring Diagram

```text
       +-------------------------------------------------------+
       |                    ESP32 BOARD                        |
       +-------------------------------------------------------+
          |         |         |         |        |        |
        3.3V       GND     GPIO 34   GPIO 23  GPIO 13  GPIO 18
          |         |         |         |        |        |
          +---------+--+      |         |        |        |
          |         |  |      |         |        |        |
       +-----+   +-----+   +-----+   +-----+  +-----+  +-----+
       |VCC  |   |GND  |   | AO  |   |DATA |  | OUT |  | Neg |
       | MQ-135 Sensor |   |DHT11|   |  IR Sensor  |  |Buzzer|
       +---------------+   +-----+   +---------+  +-----+

       ESP32 Output Pins for RGB LED Indicator:
       - GPIO 21  ===> [ 220Ω Resistor ] ===> RGB LED Red Pin
       - GPIO 19  ===> [ 220Ω Resistor ] ===> RGB LED Green Pin
       - GPIO 22  ===> [ 220Ω Resistor ] ===> RGB LED Blue Pin
       - GND      =========================> RGB LED GND Pin
