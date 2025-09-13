/*
 * ESProxy
 * An ESP32 tool which locate BLE devices using proximity.
 * Author - WireBits
 */

#include <math.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>

BLEScan* pBLEScan;
int lastRSSI = -999;
bool scanning = false;
bool tracking = false;
const int txPower = -59;
String trackAddress = "";
unsigned long lastSeen = 0;
const float pathLossExp = 2.0;
const unsigned long timeoutMs = 5000;

float estimateDistance(int rssi) {
  if (rssi == 0) return -1.0;
  float ratio = (float)(txPower - rssi) / (10 * pathLossExp);
  return pow(10, ratio);
}

String rssiQuality(int rssi) {
  if (rssi >= -50) return "Excellent";
  else if (rssi >= -60) return "Good";
  else if (rssi >= -70) return "Fair";
  else if (rssi >= -80) return "Weak";
  else if (rssi >= -90) return "Poor";
  else return "Unusable";
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String devAddr = advertisedDevice.getAddress().toString().c_str();
    devAddr.toUpperCase();
    String devName = advertisedDevice.getName().c_str();
    if (devName.length() == 0) devName = "Device Name Not Available!";

    float dist = estimateDistance(advertisedDevice.getRSSI());
    String quality = rssiQuality(advertisedDevice.getRSSI());

    if (scanning && !tracking) {
      Serial.printf(
        "[DISCOVERED] Name: %s | MAC: %s | RSSI: %d | Distance: ~%.2fm | Strength: %s\n",
        devName.c_str(),
        devAddr.c_str(),
        advertisedDevice.getRSSI(),
        dist,
        quality.c_str()
      );
    }

    if (tracking && devAddr.equals(trackAddress)) {
      lastSeen = millis();
      lastRSSI = advertisedDevice.getRSSI();
      Serial.printf(
        "[TRACKING] Name: %s | MAC: %s | RSSI: %d | Distance: ~%.2fm | Strength: %s\n",
        devName.c_str(),
        devAddr.c_str(),
        lastRSSI,
        dist,
        quality.c_str()
      );
      Serial.flush();
    }
  }
};

void startScan() {
  if (!scanning) {
    Serial.println("[*] Starting BLE scan...");
    pBLEScan->start(0, nullptr, false);
    scanning = true;
  } else {
    Serial.println("[*] Already Scanning!");
  }
}

void stopAll() {
  pBLEScan->stop();
  if (scanning) {
    scanning = false;
    Serial.println("[!] Stopped Scanning!");
  }
  if (tracking) {
    tracking = false;
    trackAddress = "";
    Serial.println("[!] Stopped Tracking!");
  }
}

void startTracking(String mac) {
  mac.toUpperCase();
  trackAddress = mac;
  tracking = true;
  scanning = false;
  lastSeen = 0;
  lastRSSI = -999;
  Serial.printf("[*] Started Tracking %s\n", mac.c_str());
  pBLEScan->stop();
  pBLEScan->start(0, nullptr, false);
}

void showHelp() {
  Serial.println("Available commands:");
  Serial.println("  scan                 - Scan nearby BLE devices");
  Serial.println("  track <MAC_ADDRESS>  - Track MAC ADDRESS");
  Serial.println("  stop                 - Stop scanning/tracking");
  Serial.println("  help                 - Show this help message");
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Bluetooth Proximity Tool");
  BLEDevice::init("ESP32_Proximity");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), true);
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.equalsIgnoreCase("scan")) {
      startScan();
    } else if (input.equalsIgnoreCase("track")) {
      Serial.println("Usage: track <MAC_ADDRESS>");
    } else if (input.startsWith("track ")) {
      String mac = input.substring(6);
      mac.trim();
      startTracking(mac);
    } else if (input.equalsIgnoreCase("stop")) {
      stopAll();
    } else if (input.equalsIgnoreCase("help")) {
      showHelp();
    } else if (input.length() > 0) {
      Serial.println("[!] Unknown command. Type 'help' for list!");
    }
  }
  if (tracking) {
    if (lastSeen > 0 && (millis() - lastSeen > timeoutMs)) {
      Serial.println("[TRACKING] Device not seen recently (timeout)!");
    }
  }
  delay(20);
}