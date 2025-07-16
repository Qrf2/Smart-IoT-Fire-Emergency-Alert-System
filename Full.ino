#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

// === WiFi Credentials ===
const char* ssid = " WIFI NAME HERE";
const char* password = "WIFI PASSWORD HERE";

// === Firebase Info ===
const char* firebaseHost = "https://smartemergencyalert-faae0-default-rtdb.firebaseio.com/";
const char* firebaseAuth = "AIzaSy...... KEY HERE"; // Updated to match Firebase config

// === Sensor Pins ===
#define FLAME_PIN 2
#define MQ2_DIGITAL_PIN 16
#define DHT_PIN 13
#define BUZZER_PIN 12 // Added for local alert

#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// === Fire Thresholds ===
const float TEMP_CRITICAL = 50.0;    // °C
const int FLAME_DETECTED = 0;        // LOW = Flame Detected
const int GAS_DETECTED = 0;          // LOW = Gas Detected from MQ2 DO
const int DEBOUNCE_COUNT = 3;        // Require 3 consecutive readings for confirmation

// === Timing ===
unsigned long lastSensorRead = 0;
const unsigned long SENSOR_INTERVAL = 4000;

bool emergencySent = false;
int fireDetectionCount = 0;

struct SensorData {
  int flame;
  int gas;
  float temperature;
  float humidity;
  bool isValid;
};

void checkWiFi();
SensorData readSensors();
bool isRealFireEmergency(const SensorData& data);
void sendEmergencyToFirebase(const SensorData& data);
void displaySensorStatus(const SensorData& data);

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(FLAME_PIN, INPUT);
  pinMode(MQ2_DIGITAL_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  dht.begin();
  checkWiFi();
  Serial.println("\n📡 System Ready - Monitoring Started");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    SensorData data = readSensors();
    if (data.isValid) {
      displaySensorStatus(data);
      if (isRealFireEmergency(data)) {
        fireDetectionCount++;
        if (fireDetectionCount >= DEBOUNCE_COUNT && !emergencySent) {
          sendEmergencyToFirebase(data);
          digitalWrite(BUZZER_PIN, HIGH); // Activate buzzer
          emergencySent = true;
        }
      } else {
        fireDetectionCount = 0;
        if (emergencySent) {
          digitalWrite(BUZZER_PIN, LOW); // Deactivate buzzer
          emergencySent = false; // Reset emergency flag
        }
      }
    }
    lastSensorRead = now;
  }
}

void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconnecting to WiFi");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\n✅ Reconnected to WiFi");
  }
}

SensorData readSensors() {
  SensorData data;
  data.flame = digitalRead(FLAME_PIN);
  data.gas = digitalRead(MQ2_DIGITAL_PIN);
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();
  data.isValid = !isnan(data.temperature) && !isnan(data.humidity);
  return data;
}

bool isRealFireEmergency(const SensorData& data) {
  return (data.flame == FLAME_DETECTED &&
          data.gas == GAS_DETECTED &&
          data.temperature >= TEMP_CRITICAL);
}

void sendEmergencyToFirebase(const SensorData& data) {
  checkWiFi();
  HTTPClient http;
  String endpoint = String(firebaseHost) + "/emergency_alert.json?auth=" + firebaseAuth;

  StaticJsonDocument<512> doc;
  doc["status"] = "CRITICAL";
  doc["fire"] = data.flame == FLAME_DETECTED;
  doc["gas"] = data.gas == GAS_DETECTED;
  doc["temp"] = data.temperature;
  doc["humidity"] = data.humidity;
  doc["lat"] = 33.56959065914976; // Hardcoded; replace with GPS if available
  doc["lon"] = 73.2302353671416;  // Hardcoded; replace with GPS if available
  doc["timestamp"] = millis(); // Added for alert timing

  String payload;
  serializeJson(doc, payload);
  http.begin(endpoint);
  http.addHeader("Content-Type", "application/json");
  int response = http.PUT(payload);

  if (response > 0) {
    Serial.println("\n🚨 FIRE ALERT SENT TO FIREBASE!");
  } else {
    Serial.println("❌ Failed to send to Firebase: " + http.errorToString(response));
  }
  http.end();
}

void displaySensorStatus(const SensorData& data) {
  Serial.print("🔥 Flame: ");
  Serial.print(data.flame == FLAME_DETECTED ? "DETECTED" : "None");

  Serial.print(" | 💨 Gas: ");
  Serial.print(data.gas == GAS_DETECTED ? "DETECTED" : "None");

  Serial.print(" | 🌡️ Temp: ");
  Serial.print(data.temperature);
  Serial.print("°C");

  Serial.print(" | 💧 Hum: ");
  Serial.print(data.humidity);
  Serial.println("%");
}
