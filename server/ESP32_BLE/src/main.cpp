#include <Arduino.h>
#include "../lib/DataGenerate.cpp"

#define DEBUG

//BLE include 
#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLE2902.h>
#include <BLEUtils.h>

//timer vars
unsigned long lastTime = 0;
unsigned long delayTime = 5000;

//gloval var
bool deviceConnected = false;

//BLE device defines
#define SERVER_NAME "ESP32"

//Max30102 ble vars
#define SERVICE_UUID "1c697d04-f628-11ec-b939-0242ac120002"
//Pulse descriptor and characteristic
BLECharacteristic PulseCharacterisric("1c697fd4-f628-11ec-b939-0242ac120002", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor PulseDescriptor(BLEUUID((uint16_t)0x2902));
//O2 descriptor and characteristic
BLECharacteristic O2Characterisric("1c698196-f628-11ec-b939-0242ac120002", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor O2Descriptor(BLEUUID((uint16_t)0x2903));
//O2 descriptor and characteristic
BLECharacteristic GyroCharacterisric("1c698196-f628-11ec-b939-0242ac120002", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor GyroDescriptor(BLEUUID((uint16_t)0x2904));
//Accel descriptor and characteristic
BLECharacteristic AccelCharacterisric("1c698664-f628-11ec-b939-0242ac120002", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor AccelDescriptor(BLEUUID((uint16_t)0x2905));

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};


void setup() {
  Serial.begin(115200);

  BLEDevice::init(SERVER_NAME);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //Init BLE Descriptors and Characterristics
  //pulse
  
  pService->addCharacteristic(&PulseCharacterisric);
  PulseDescriptor.setValue("Pulse");
  PulseCharacterisric.addDescriptor(new BLE2902());
  //o2
  pService->addCharacteristic(&O2Characterisric);
  O2Descriptor.setValue("O2");
  O2Characterisric.addDescriptor(new BLE2902());
  //gyro
  pService->addCharacteristic(&GyroCharacterisric);
  GyroDescriptor.setValue("Gyro");
  GyroCharacterisric.addDescriptor(new BLE2902());
  //accel
  pService->addCharacteristic(&AccelCharacterisric);
  AccelDescriptor.setValue("Accel");
  AccelCharacterisric.addDescriptor(new BLE2902());

  //service start
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

}

MAX30102 m;
int pulse, o2;
float accel, gyro;

void loop() {
    static const int packsize = 9;
    if (deviceConnected) {
    
      // generate data
      // Pulse
      static char pulse[3];
      dtostrf(((double)m.Pulse()), 3, 0, pulse);
      PulseCharacterisric.setValue(pulse);
      PulseCharacterisric.notify();
      // O2
      static char o2[3];
      dtostrf((double)(m.O2()),3,0,o2);
      O2Characterisric.setValue(o2);
      O2Characterisric.notify();
      // Gyro
      static char gyro[10];
      dtostrf(m.Gyro(), 10, 2, gyro);
      GyroCharacterisric.setValue(gyro);
      GyroCharacterisric.notify();
      // Accel
      static char accel[10];
      dtostrf(m.Acc(), 10, 2, accel);
      AccelCharacterisric.setValue(accel);
      AccelCharacterisric.notify();
      
      #ifdef DEBUG
      Serial.print("Пульс ");
      Serial.println(pulse);
      Serial.print("Кислород ");
      Serial.println(o2);
      Serial.print("Гироскоп ");
      Serial.println(gyro);
      Serial.print("Акселерометр ");
      Serial.println(accel);
      #endif //DEBUG





      delay(delayTime);
    }
}