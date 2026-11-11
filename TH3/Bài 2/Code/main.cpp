#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include "th3_config.h"

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* server) override {
    Serial.println("Dien thoai da ket noi");
  }

  void onDisconnect(BLEServer* server) override {
    Serial.println("Dien thoai da ngat ket noi, bat quang ba lai");
    server->startAdvertising();
  }
};

class MessageCallbacks : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic* characteristic) override {
    Serial.print("Dien thoai DOC gia tri: ");
    Serial.println(characteristic->getValue().c_str());
  }

  void onWrite(BLECharacteristic* characteristic) override {
    Serial.print("Dien thoai GHI gia tri moi: ");
    Serial.println(characteristic->getValue().c_str());
  }
};

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("Bai 2 - ESP32 lam BLE server");

  BLEDevice::init(BLE_DEVICE_NAME);
  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService* service = server->createService(BLE_SERVICE_UUID);
  BLECharacteristic* messageCharacteristic = service->createCharacteristic(
      BLE_CHAR_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  messageCharacteristic->setCallbacks(new MessageCallbacks());
  messageCharacteristic->setValue(BLE_INITIAL_MESSAGE);
  service->start();

  BLEAdvertising* advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(BLE_SERVICE_UUID);
  advertising->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.print("Ten thiet bi: ");
  Serial.println(BLE_DEVICE_NAME);
  Serial.print("Dia chi BLE: ");
  Serial.println(BLEDevice::getAddress().toString().c_str());
  Serial.print("Service UUID: ");
  Serial.println(BLE_SERVICE_UUID);
  Serial.print("Characteristic UUID: ");
  Serial.println(BLE_CHAR_UUID);
  Serial.print("Gia tri ban dau: ");
  Serial.println(BLE_INITIAL_MESSAGE);
  Serial.println("Dang quang ba, mo nRF Connect de quet");
}

void loop() {
  delay(2000);
}
