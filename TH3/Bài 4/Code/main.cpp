#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "th3_config.h"

WebServer server(80);
const int ledPin = LED_PIN;
bool ledOn = false;

void setLed(bool on) {
  ledOn = on;
  digitalWrite(ledPin, on ? HIGH : LOW);
  Serial.printf("LED GPIO%d: %s\n", ledPin, on ? "BAT (HIGH)" : "TAT (LOW)");
}

void logRequest() {
  Serial.print(server.client().remoteIP());
  Serial.print(" -> ");
  Serial.print(server.method() == HTTP_GET ? "GET " : "POST ");
  Serial.println(server.uri());
}

String buildPage() {
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 LED</title><style>";
  html += "body{font-family:sans-serif;text-align:center;margin-top:40px}";
  html += ".btn{display:inline-block;padding:14px 32px;margin:8px;border-radius:8px;color:#fff;text-decoration:none;font-size:20px}";
  html += ".on{background:#2e7d32}.off{background:#c62828}";
  html += "</style></head><body>";
  html += "<h1>ESP32 điều khiển LED</h1>";
  html += "<p>LED nối GPIO" + String(ledPin) + "</p>";
  html += "<p style='font-size:24px'>Trạng thái: <b>";
  html += ledOn ? "đang BẬT" : "đang TẮT";
  html += "</b></p>";
  html += "<a class='btn on' href='/on'>BẬT</a>";
  html += "<a class='btn off' href='/off'>TẮT</a>";
  html += "</body></html>";
  return html;
}

void redirectHome() {
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleRoot() {
  logRequest();
  server.send(200, "text/html; charset=utf-8", buildPage());
}

void handleOn() {
  logRequest();
  setLed(true);
  redirectHome();
}

void handleOff() {
  logRequest();
  setLed(false);
  redirectHome();
}

void handleNotFound() {
  logRequest();
  server.send(404, "text/plain", "Khong co trang nay");
}

void connectWifi() {
#ifdef TH3_WOKWI
  const char* ssid = "Wokwi-GUEST";
  const char* password = "";
#else
  const char* ssid = WIFI_SSID;
  const char* password = WIFI_PASSWORD;
#endif
  Serial.print("Dang ket noi Wi-Fi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries % 40 == 0) {
      Serial.println();
      Serial.println("Chua vao duoc mang, kiem tra WIFI_SSID / WIFI_PASSWORD trong common/wifi_secrets.h");
    }
  }
  Serial.println();
  Serial.print("Da vao mang, IP cua ESP32: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("Bai 4 - HTTP web server dieu khien LED");

  pinMode(ledPin, OUTPUT);
  setLed(false);
  connectWifi();

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.print("Mo trinh duyet (cung mang): http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  server.handleClient();
}
