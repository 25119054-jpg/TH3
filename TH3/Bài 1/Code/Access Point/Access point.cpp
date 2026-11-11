#include <Arduino.h>
#include <WiFi.h>
#include "th3_config.h"

int lastStationCount = -1;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("Bai 1 - ESP32 o che do Access Point (AP)");

  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP(AP_SSID, AP_PASSWORD)) {
    Serial.println("Khong phat duoc AP (mat khau phai tu 8 ky tu)");
    return;
  }

  Serial.print("SSID: ");
  Serial.println(AP_SSID);
  Serial.print("Mat khau: ");
  Serial.println(AP_PASSWORD);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("AP MAC: ");
  Serial.println(WiFi.softAPmacAddress());
}

void loop() {
  int stationCount = WiFi.softAPgetStationNum();
  if (stationCount != lastStationCount) {
    lastStationCount = stationCount;
    Serial.print("So thiet bi dang noi vao AP: ");
    Serial.println(stationCount);
  }
  delay(1000);
}
