#include <Arduino.h>
#include <WiFi.h>
#include "th3_config.h"

const char* targetSsid = WIFI_SSID;
const char* targetPassword = WIFI_PASSWORD;

wl_status_t lastStatus;

const char* statusText(wl_status_t status) {
  switch (status) {
    case WL_CONNECTED: return "WL_CONNECTED (da vao mang)";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL (khong thay SSID)";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED (ket noi that bai)";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST (mat ket noi)";
    case WL_DISCONNECTED: return "WL_DISCONNECTED (chua ket noi)";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    default: return "trang thai khac";
  }
}

void printNetworkInfo() {
  Serial.print("IP nhan duoc: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.print("MAC STA: ");
  Serial.println(WiFi.macAddress());
}

void connectWifi() {
  Serial.print("Dang ket noi toi SSID: ");
  Serial.println(targetSsid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(targetSsid, targetPassword);

  unsigned long startMs = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startMs < 20000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Trang thai: ");
  Serial.println(statusText(WiFi.status()));
  if (WiFi.status() == WL_CONNECTED) {
    printNetworkInfo();
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("Bai 1 - ESP32 o che do Station (STA)");
  connectWifi();
  lastStatus = WiFi.status();
}

void loop() {
  wl_status_t status = WiFi.status();
  if (status != lastStatus) {
    lastStatus = status;
    Serial.print("Trang thai doi thanh: ");
    Serial.println(statusText(status));
    if (status == WL_CONNECTED) {
      printNetworkInfo();
    }
  }
  delay(1000);
}
