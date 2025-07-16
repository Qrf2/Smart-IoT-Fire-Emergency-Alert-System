# 🔥 Smart IoT Fire Emergency Alert System

## 📘 Project Overview
This project is a **real-time fire emergency detection and alert system** using the ESP32-CAM, various sensors, Firebase Realtime Database, and a Leaflet-based web dashboard. The system monitors for critical fire conditions and sends alerts with precise location data to a custom-built web interface.

## ❓ Why We Need This
- Fire accidents are unpredictable and can spread rapidly — early detection is **crucial**.
- Manual fire watch systems are **inefficient** and often fail to react fast.
- This system provides a **real-time**, **automated**, and **location-aware** response that can prevent loss of life and property.
- It’s scalable for **smart homes**, **industrial environments**, **warehouses**, and even **smart city infrastructure**.

## 📸 Screenshots
<img width="1913" height="1019" alt="1" src="https://github.com/user-attachments/assets/7fb04421-beb7-43dd-9318-3cbcaf452bec" />
<img width="1919" height="1079" alt="2" src="https://github.com/user-attachments/assets/55e598ea-c0a9-43d3-8ed5-2479615a14f8" />
<img width="1919" height="1079" alt="3" src="https://github.com/user-attachments/assets/b8170784-cc5b-4b1c-a2c3-853da2c9d89d" />
<img width="1919" height="1022" alt="4" src="https://github.com/user-attachments/assets/3a3bc98f-c6f9-40ba-bee7-c6f4ed88f179" />
<img width="1802" height="892" alt="5" src="https://github.com/user-attachments/assets/6c6627c7-f3d7-4870-9b33-efbe46bf699d" />

---

## 🎯 Objectives
- Detect real fire emergencies using a combination of temperature, gas, and flame sensors.
- Send real-time alerts to Firebase when all conditions for fire are met.
- Display live alerts and pin the location on a web map using Leaflet.
- Make the system lightweight, fast, and ideal for emergency response scenarios.

---

## ⚙️ Hardware Components Used
| Component              | Description                                     |
|------------------------|-------------------------------------------------|
| ESP32-CAM              | Microcontroller with WiFi and camera module     |
| DHT11                  | Temperature and humidity sensor                 |
| MQ-2 Gas Sensor        | Detects gas, smoke, butane, propane, etc.       |
| Flame Sensor (IR)      | Detects visible flame (lighter, fire)           |
| Breadboard + Jumper Wires | For connections and prototyping            |
| Power Source           | 5V power supply or USB for ESP32-CAM            |

---

## 🛠️ Software & Technologies
| Tool/Tech              | Purpose                                         |
|------------------------|-------------------------------------------------|
| Arduino IDE            | Microcontroller programming                     |
| Firebase Realtime DB   | Cloud backend for data logging & sync          |
| HTML + CSS + JavaScript| Web interface                                   |
| Leaflet.js             | Interactive real-time map visualization         |
| Firebase JS SDK        | Web integration with Firebase                   |

---

## 🧠 How It Works
1. **Sensor Readings:**
   - Flame: Detects if a fire is visually visible (LOW = detected)
   - MQ2: Analog gas reading, sensitive to smoke/butane
   - DHT11: Measures temperature and humidity

2. **Decision Logic (ESP32 Code):**
   - All 3 conditions must be met: High temp (>50°C), High gas, Flame present
   - If so, a **Firebase alert** is sent with sensor values and exact coordinates

3. **Web Dashboard:**
   - Uses Leaflet map to plot the alert location
   - Displays card with alert type, timestamp, and sensor readings

---

## 🗺️ Firebase Data Format Example
```json
{
  "status": "CRITICAL",
  "fire": true,
  "gas": 725,
  "temp": 56.3,
  "humidity": 41.2,
  "lat": LATITUDE,
  "lon": LONGITUDE
}
```

---

## 🚨 Real-time Workflow
1. Fire conditions detected by hardware
2. Firebase gets updated with critical data
3. Web page reads this and plots real-time alert on map

---

## 📦 Deployment Notes
- The ESP32 sends data directly to Firebase.
- Web dashboard can be deployed to [Netlify](https://netlify.com) or [Vercel](https://vercel.com).
- Requires no backend — pure frontend + Firebase logic.

---

## 🏁 Final Thoughts
This project is a practical example of how **IoT and real-time web technologies** can be used for smart alert systems. It's scalable, efficient, and a solid foundation for larger smart city safety networks.

---


