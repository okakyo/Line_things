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
BLECharacteristic *notifyCharacteristic;

bool deviceConnected=false;
bool oldDeviceConnected=false;

void left_motor(int pin1,int pin2){
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,LOW);
}
void right_motor(int pin1,int pin2){
  digitalWrite(pin1,LOW);
  digitalWrite(pin2,HIGH);
}

void forward(){
  right_motor(MOTOR_1,MOTOR_2);
  right_motor(MOTOR_3,MOTOR_4);
}
void back(){
  left_motor(MOTOR_1,MOTOR_2);
  left_motor(MOTOR_3,MOTOR_4);
}
void left(){
  left_motor(MOTOR_1,MOTOR_2);
  right_motor(MOTOR_3,MOTOR_4);
}
void right(){
  right_motor(MOTOR_1,MOTOR_2);
  left_motor(MOTOR_3,MOTOR_4);
}

void controller(char signal){
  switch (signal){
    case 'l':{
      left();
    }
    case 'r':{
      right();
    }
    case 'f':{
      forward();
    }
    case 'b':{
      back();
    }
  }
}

class serverCallbacks public BLEServerCallbacks{
  void onConnect(BLEServer *pServer){
    deviceConnected=true;
  }

  void onDisconnecct(BLEServer *pServer){
    deviceConnected=false;        
  }
};

class writeCallback public BLECharacteristicCallbacks{
  void onWrite(BLECharacteristic *bleWriteCharacteristic){
    std::string value bleWriteCharacteristic->getValue();
    if((char)value[0]<=1){
        digitalWrite(LED1,(char)value[0])      
      }
    }
};

void setup() {
  Serial.begin(115200);
  
  pinMode(MOTOR_1,OUTPUT);
  pinMode(MOTOT_2,OUTPUT);
  pinMode(MOTOR_3,OUTPUT);
  pinMode(MOTOR_4,OUTPUT);
  
  BLEDevice::init("");
  BLEDevice::setEncryptionLevel(ESP_SEC_ENCRYPT_NO_MTIM);

  BLESecurity *thingsSecurity=new BLESecurity();
  thingsSecurity->setAuthenticationNode(ESP_LE_AUTH_REQ_SC_ONLY);
  thingsSecurity->setCapability(ESP_IO_CAP_NONE);
  thingsSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK|ESP_BLE_ID_KEY_MASK);
  
  setupServices();
  startAdvertising();
  Serial.println("Ready to Connect");
}

void loop() {
  while(deviceConnected){
    char signal=Serial.read();
    controller(char);
  }
  delay(500);
}

//Write the two functions as almost all the same ways as examples

void setupServices(void){
  thingsServer=BLEDevice::createServer();
  thingsServer->setCallbacks(new serverCallbacks());

  //Setup Services
  
  userService=thingsStarter->createService(USER_SERVICE_UUID);

  writeCharacteristic=userService->createCharacteristic(WRITE_CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_WRITE);
  writeCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED|ESP_GGATT_PREM_WRITE_ENCRIPTED);
  writeCharacteristic->setCallbacks(new writeCallbacks());

  notifyCharacteristic=userService->createCharacteristic(WRITE_CHARACTERISTIC_UUID,BLECharacteristic)
  notifyCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED|ESP_GGATT_PREM_WRITE_ENCRIPTED)
  //Setup PSDI Services
  
  psdiService=thingsStarter->createService(PSDI_SERVICE_UUID);
  psdiCharacteristic=psdiService->createCharacteristic(PSDI_CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ);
  psdiCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED|ESP_GGATT_PREM_WRITE_ENCRIPTED);
  
  //Set PSDI(Product specofoc Device ID) value
  uint64_t macAddress=ESP.getEfuseMac();
  psdiCharacteristic->setValue((uint8_t*) &macAddress,sizeof(macAddress));
  
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
  thingsServer->getAdvertising()->start();
  }
