#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Pin Definitions
#define DHTPIN 23
#define DHTTYPE DHT11

#define MQ135_PIN 34
#define IR_PIN 13       // Uses internal INPUT_PULLUP
#define BUZZER_PIN 18   // Inverted Logic Buzzer (LOW = ON, HIGH = OFF)

// ESP32 Pins (Swapped Red and Green pins to match your module's physical wiring)
#define RED_PIN 21      // Changed from 19 to 21
#define GREEN_PIN 19    // Changed from 21 to 19
#define BLUE_PIN 22     // Unchanged (High Humidity)

// Environmental Thresholds
#define GAS_THRESHOLD 1200      // Analog value threshold
#define HUMIDITY_HIGH 70.0      // High humidity threshold (%)
#define TEMP_LOW 18.0           // Low temperature threshold (°C)

// Access Point Credentials
const char* ssid = "Adaptive-Ambient-AP";
const char* password = "12345678";

WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

// System Variables
bool alarmActive = false;
String alertMessage = "System Normal";
float temp = 0.0;
float humidity = 0.0;
int gasVal = 0;

// Clock & Alarm Variables
int alarmHour = 6;
int alarmMinute = 0;
int currentHour = 8;     
int currentMinute = 0;
int currentSecond = 0;

unsigned long previousMillis = 0;
unsigned long clockMillis = 0;
const long interval = 1500; 

// Inverted Buzzer Control Function
void setBuzzer(bool stateOn) {
  if (stateOn) {
    digitalWrite(BUZZER_PIN, LOW);  // Active LOW: ON
  } else {
    digitalWrite(BUZZER_PIN, HIGH); // Active LOW: OFF
  }
}

// Single-Color Pin Control
void setSingleColor(String color) {
  if (color == "RED") {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  } else if (color == "GREEN") {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
  } else if (color == "BLUE") {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
  } else { // OFF
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  }
}

String formatTwoDigits(int number) {
  if (number < 10) return "0" + String(number);
  return String(number);
}

// Web Dashboard HTML Generator
String getHTMLPage() {
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='3'>";
  html += "<style>body{font-family:Arial,sans-serif;text-align:center;background:#121212;color:#fff;padding:20px;}";
  html += ".card{background:#1e1e1e;padding:15px;margin:10px auto;max-width:350px;border-radius:10px;box-shadow:0 4px 8px rgba(0,0,0,0.3);}";
  html += ".btn{background:#008CBA;color:white;border:none;padding:10px 18px;font-size:15px;border-radius:5px;cursor:pointer;margin:5px;}";
  html += ".btn-off{background:#f44336;} input[type=time]{padding:8px;font-size:16px;border-radius:5px;}</style></head><body>";
  
  html += "<h2>Adaptive Ambient Control Hub</h2>";
  
  html += "<div class='card'><h3>System Time</h3>";
  html += "<p style='font-size:24px;font-weight:bold;'>" + formatTwoDigits(currentHour) + ":" + formatTwoDigits(currentMinute) + ":" + formatTwoDigits(currentSecond) + "</p>";
  html += "<form action='/setTime' method='GET'>";
  html += "<input type='time' name='sysTime' required> ";
  html += "<input type='submit' class='btn' value='Sync Clock'></form></div>";

  html += "<div class='card'><h3>Live Sensor Readings</h3>";
  html += "<p><b>Temperature:</b> " + String(temp, 1) + " &deg;C</p>";
  html += "<p><b>Humidity:</b> " + String(humidity, 1) + " %</p>";
  html += "<p><b>Air/Gas Level:</b> " + String(gasVal) + "</p></div>";
  
  html += "<div class='card'><h3>Status & Alerts</h3>";
  html += "<p style='color:" + String(alarmActive || alertMessage != "System Normal" ? "#ff4d4d" : "#00ff88") + ";font-weight:bold;'>";
  html += alertMessage + "</p></div>";
  
  html += "<div class='card'><h3>Set Daily Alarm Time</h3>";
  html += "<form action='/setAlarm' method='GET'>";
  html += "<input type='time' name='alarmTime' value='" + formatTwoDigits(alarmHour) + ":" + formatTwoDigits(alarmMinute) + "' required><br><br>";
  html += "<input type='submit' class='btn' value='Set Alarm Time'></form>";
  html += "<p>Current Preset: <b>" + formatTwoDigits(alarmHour) + ":" + formatTwoDigits(alarmMinute) + "</b></p></div>";

  html += "<div class='card'><h3>Manual Controls</h3>";
  html += "<a href='/alarmOn'><button class='btn'>Trigger Test Alarm</button></a>";
  html += "<a href='/alarmOff'><button class='btn btn-off'>Snooze / Silence Alarm</button></a>";
  html += "</div></body></html>";
  
  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHTMLPage());
}

void handleSetTime() {
  if (server.hasArg("sysTime")) {
    String t = server.arg("sysTime");
    currentHour = t.substring(0, 2).toInt();
    currentMinute = t.substring(3, 5).toInt();
    currentSecond = 0;
    alertMessage = "Clock Synced to " + formatTwoDigits(currentHour) + ":" + formatTwoDigits(currentMinute);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleSetAlarm() {
  if (server.hasArg("alarmTime")) {
    String timeStr = server.arg("alarmTime");
    alarmHour = timeStr.substring(0, 2).toInt();
    alarmMinute = timeStr.substring(3, 5).toInt();
    alertMessage = "Alarm Preset Updated to " + formatTwoDigits(alarmHour) + ":" + formatTwoDigits(alarmMinute);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleAlarmOn() {
  alarmActive = true;
  alertMessage = "Manual Alarm Activated!";
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleAlarmOff() {
  alarmActive = false;
  setBuzzer(false);
  alertMessage = "System Normal";
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  pinMode(MQ135_PIN, INPUT);
  pinMode(IR_PIN, INPUT_PULLUP);
  
  pinMode(BUZZER_PIN, OUTPUT);
  setBuzzer(false);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  setSingleColor("GREEN");
  
  WiFi.softAP(ssid, password);
  Serial.print("Access Point Started! IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/setTime", handleSetTime);
  server.on("/setAlarm", handleSetAlarm);
  server.on("/alarmOn", handleAlarmOn);
  server.on("/alarmOff", handleAlarmOff);
  server.begin();
}

void loop() {
  server.handleClient();
  
  unsigned long currentMillis = millis();

  // Internal Clock Counter Logic
  if (currentMillis - clockMillis >= 1000) {
    clockMillis = currentMillis;
    currentSecond++;
    if (currentSecond >= 60) {
      currentSecond = 0;
      currentMinute++;
      if (currentMinute >= 60) {
        currentMinute = 0;
        currentHour++;
        if (currentHour >= 24) {
          currentHour = 0;
        }
      }
    }

    // Trigger Alarm when Scheduled Time matches
    if (currentHour == alarmHour && currentMinute == alarmMinute && currentSecond == 0) {
      alarmActive = true;
      alertMessage = "SCHEDULED ALARM TRIGGERED!";
    }
  }

  // IR Sensor: ONLY silences an active alarm or gas alert
  if (digitalRead(IR_PIN) == LOW) {
    delay(50);
    if (digitalRead(IR_PIN) == LOW && (alarmActive || gasVal > GAS_THRESHOLD)) {
      alarmActive = false;
      setBuzzer(false);
      alertMessage = "Alarm Silenced via Touchless Sensor";
      delay(800);
    }
  }

  // Periodic Environmental Monitoring
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    humidity = dht.readHumidity();
    temp = dht.readTemperature();
    gasVal = analogRead(MQ135_PIN);

    if (!alarmActive) {
      // 1. Gas Hazard (Solid RED + Buzzer)
      if (gasVal > GAS_THRESHOLD) {
        setSingleColor("RED");
        setBuzzer(true);
        alertMessage = "ALERT: High Gas/Air Pollution Detected!";
      } 
      // 2. High Humidity Warning (Solid BLUE)
      else if (humidity >= HUMIDITY_HIGH) {
        setSingleColor("BLUE");
        setBuzzer(false);
        alertMessage = "WARNING: High Humidity Level (" + String(humidity, 1) + "%)!";
      } 
      // 3. Low Temp Warning (Solid BLUE)
      else if (temp < TEMP_LOW) {
        setSingleColor("BLUE");
        setBuzzer(false);
        alertMessage = "ALERT: Low Temperature Warning!";
      } 
      // 4. Normal State (Solid GREEN)
      else {
        setSingleColor("GREEN");
        setBuzzer(false);
        alertMessage = "System Normal";
      }
    }
  }

  // Active Alarm Beep Loop (Solid RED + Pulsing Buzzer)
  if (alarmActive) {
    setSingleColor("RED");
    setBuzzer(true);
    delay(150);
    setBuzzer(false);
    delay(150);
  }
}