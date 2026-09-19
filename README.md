# Adaptive-Ambient-Control-Hub
ESP32-based Edge Computing IoT Hub with ambient monitoring, web clock, and touchless IR alarm dismissal.
## 🔌 Hardware Pin Mapping

| Component | Pin / Type | ESP32 GPIO Pin | Description |
| :--- | :--- | :--- | :--- |
| **DHT11 Sensor** | Data Pin | **GPIO 23** | Temperature & Humidity Sensor |
| **MQ-135 Sensor** | Analog Output (AO) | **GPIO 34** | Air Quality & Gas Sensor |
| **IR Proximity Sensor**| Digital Output (OUT)| **GPIO 13** | Touchless Gesture / Silence Switch |
| **Active-LOW Buzzer** | Positive Signal | **GPIO 18** | Audible Hazard Beeper |
| **RGB LED (Red)** | Anode / Cathode | **GPIO 21** | Gas Hazard & Alarm Indicator |
| **RGB LED (Green)**| Anode / Cathode | **GPIO 19** | Normal / Safe System State |
| **RGB LED (Blue)** | Anode / Cathode | **GPIO 22** | Humidity & Temperature Warning |
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

## 💬 Community & Open Feedback

This project was built with an open-source mindset **for the people, by the people**. 

We welcome all feedback, technical reviews, and constructive criticism to help improve this system:
* **Bug Reports & Code Improvements:** Open a ticket under the [Issues](../../issues) tab.
* **General Feedback & Feature Ideas:** Start a thread in [Discussions](../../discussions).
* **Pull Requests:** Feel free to fork this repository, refine the logic or circuits, and submit a PR!
       - GPIO 22  ===> [ 220Ω Resistor ] ===> RGB LED Blue Pin
       - GND      =========================> RGB LED GND Pin
