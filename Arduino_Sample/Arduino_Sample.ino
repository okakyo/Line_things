#include<BLEServer.h>
#include<BLEDevice.h>
#include<BLEUnits.h>
#include<BLE2902.h>

// The name of Device 
#define DEVICE_NAME=""

//The setting of UUID

#define USER_SERVICE_UUID=""

//You can freely define the two UUIDs（Write the same UUIDs in liff.js）
#define WRITE_CHARACTERISTIC_UUID=""
#define NOTIFY_CHARACTERISTIC_UUID=""

#define PSDI_SERVICE_UUID=""
#define PSDI_CHARACTERISTIC_UUID=""

//the pin of GPIO
#define BUTTON 0
#define LED1 2

#define MOTOR_1=5
#define MOTOR_2=7
#define MOTOR_3=9
#define MOTOR_4=11


//The setting of BLE Server
BLEServer *thingsServer;
BLESecurity *thingsSecurity;
BLEService *userService;
BLEService *psdiService;
BLECharacteristic *psdiCharacteristic;
BLECharacteristic *writeCharacteristic;
BLECHaracteristic *notifyCharacteristic;

bool deviceConnected=false;
bool oldDeviceConnected=false;
volatile int btnAction=0;


void setup() {
  Serial.begin(115200);
  pinMode(LED1,OUTPUT);
  pinMode();
  setupServices();
  startAdvertising();
}

void loop() {
   delay(500);
}

//Write the two functions as almost all the same ways as examples

void setupServices(void){
  thingsServer=BLEDevice::createServer();
  thingsServer->setCallbacks(new serverCallbacks());

  //Setup Services
  userService=thingsStarter->createService(USER_SERVICE_UUID);
  //Setup PSDI Services

  //Set PSDI(Product specofoc Device ID) value
  //Start BLE Services
  userService->start();
  psdiService->start();
  }
void startAdvertising(void){
  BLEAdvertisementData scanResponseData=BLEAdvertisementData();
  scanResponseData.setFlags(0x06); // GENERAL_DISC_MODE 0x02|BR_EDR_NOT_SUPPORTED 0x04
  scanResponseData.setName(DEVICE_NAME);
  
  thingsServer->getAdvertising()->addServiceUUID(userService->getUUID());
  thingsServer->getAdvertising()->setScanResponseData(scanResponseData);
  thingsServer->getAdvevrtising()->start();
  }
