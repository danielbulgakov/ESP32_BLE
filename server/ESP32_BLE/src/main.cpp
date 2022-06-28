#include <Arduino.h>
#include "../lib/DataGenerate.cpp"
#include <cstring>

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

MAX30102 Sensor;
void setup() {
  Serial.begin(115200);
  Sensor._init_();
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


char pulse[4] = "NaN";
char o2[4] = "NaN";
char gyro[6] = "NaN";
char accel[6] = "NaN";
const int packsize = 9;

void loop() {
    if (deviceConnected) {
    
      #ifdef DEBUG
      Serial.print("Пульс        = ");
      Serial.println(pulse);
      Serial.print("Кислород     = ");
      Serial.println(o2);
      Serial.print("Гироскоп     = ");
      Serial.println(gyro);
      Serial.print("Акселерометр = ");
      Serial.println(accel);
      #endif //DEBUG
      
      // generate data
      // Pulse
      strcpy(pulse, std::to_string(Sensor.Pulse()).c_str());
      PulseCharacterisric.setValue(pulse);
      PulseCharacterisric.notify();
      // O2
      strcpy(o2, std::to_string(Sensor.O2()).c_str());
      O2Characterisric.setValue(o2);
      O2Characterisric.notify();
      // Gyro
      dtostrf(Sensor.Gyro(), 6, 2, gyro);
      GyroCharacterisric.setValue(gyro);
      GyroCharacterisric.notify();
      // Accel
      dtostrf(Sensor.Acc(), 6, 2, accel);
      AccelCharacterisric.setValue(accel);
      AccelCharacterisric.notify();
      





      delay(delayTime);
    }
}