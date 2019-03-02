#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Device Name: Maximum 30 bytes
#define DEVICE_NAME "Cotra Dev"

// User service UUID: Change this to your generated service UUID
#define USER_SERVICE_UUID "91E4E176-D0B9-464D-9FE4-52EE3E9F1552"
// User service characteristics
#define WRITE_CHARACTERISTIC_UUID "E9062E71-9E62-4BC6-B0D3-35CDCD9B027B"
#define NOTIFY_CHARACTERISTIC_UUID "62FBD229-6EDD-4D1A-B554-5C4E1BB29169"

// PSDI Service UUID: Fixed value for Developer Trial
#define PSDI_SERVICE_UUID "E625601E-9E55-4597-A598-76018A0D293D"
#define PSDI_CHARACTERISTIC_UUID "26E2B12B-85F0-4F3F-9FDD-91D114270E6E"

#define NoisePin 15

//　セグメントの表示プログラム
const int anode_pins[] = {12, 8, 5, 3, 2, 11, 6, 4};    // アノードに接続するArduinoのピン
const int cathode_pins[] = {7, 9, 10, 13};  // カソードに接続するArduinoのピン
const int number_of_anode_pins = sizeof(anode_pins) / sizeof(anode_pins[0]);
const int number_of_cathode_pins = sizeof(cathode_pins) / sizeof(cathode_pins[0]);

bool CheckAlert false


const int digits[] = {
  0b00111111, // 0
  0b01101101, // 5
};
 
// 1桁の数字(n)を表示する
void display_number (int n) {
  for (int i = 0; i < number_of_anode_pins; i++) {
    digitalWrite(anode_pins[i], digits[n] & (1 << i) ? HIGH : LOW);
  }
}
 
// アノードをすべてLOWにする
void clear_segments() {
  for (int j = 0; j < number_of_anode_pins; j++) {
    digitalWrite(anode_pins[j], LOW);
  }
}
 
void display_numbers (int n) {
  for (int i = 0; i < number_of_cathode_pins; i++) {
    digitalWrite(cathode_pins[i], LOW);
    display_number(n % 10); // 最後の一桁を表示する
    delay(1);
    clear_segments();
    digitalWrite(cathode_pins[i], HIGH);
    n = n / 10; // 10で割る
  }
}

//BLEサーバー用のプログラム

class serverCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
   deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};
//ボタンが押されたら、ブザーとライトが点滅
class writeCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *bleWriteCharacteristic) {
    std::string value = bleWriteCharacteristic->getValue();
    if ((char)value[0] <= 1) 
	CheckAlert=true;
    	
    else
	CheckAlert=false;
	
  }
};
void ShowLight(){
	digitalWrite()
}

void makeNoise(){
	digitalWrite(NoisePin,HIGH);
	delay(500)
	digitalWrite(NoisePin,LOW);
	delay(500)
}

void setup(){
	Serial.begin(115200);
	pinMode(NoisePin,OUTPUT);  

	pinMode(BUTTON, INPUT_PULLUP);
      
        BLEDevice::init("");
        BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_NO_MITM);

	// Security Settings
	BLESecurity *thingsSecurity = new BLESecurity();
	thingsSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);
	thingsSecurity->setCapability(ESP_IO_CAP_NONE);
	thingsSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

	setupServices();
	startAdvertising();

	Serial.println("Ready to Connect");

	for (int i = 0; i < number_of_anode_pins; i++) {
    		pinMode(anode_pins[i], OUTPUT);  // anode_pinsを出力モードに設定する
  	}
  	for (int i = 0; i < number_of_cathode_pins; i++) {
    		pinMode(cathode_pins[i], OUTPUT);  // cathode_pinを出力モードに設定する
    		digitalWrite(cathode_pins[i], LOW);
  	}
}

void loop(){
	if (deviceConnected){
		if(CheckAlert){
			makeNoise();
			ShowLight();
		}
	}
	delay(1000);
}

void setupServices(void) {
  // Create BLE Server
  thingsServer = BLEDevice::createServer();
  thingsServer->setCallbacks(new serverCallbacks());

  // Setup User Service
  userService = thingsServer->createService(USER_SERVICE_UUID);
  // Create Characteristics for User Service
  writeCharacteristic = userService->createCharacteristic(WRITE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
  writeCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  writeCharacteristic->setCallbacks(new writeCallback());

  notifyCharacteristic = userService->createCharacteristic(NOTIFY_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  notifyCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  BLE2902* ble9202 = new BLE2902();
  ble9202->setNotifications(true);
  ble9202->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  notifyCharacteristic->addDescriptor(ble9202);

  // Setup PSDI Service
  psdiService = thingsServer->createService(PSDI_SERVICE_UUID);
  psdiCharacteristic = psdiService->createCharacteristic(PSDI_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
  psdiCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

  // Set PSDI (Product Specific Device ID) value
  uint64_t macAddress = ESP.getEfuseMac();
  psdiCharacteristic->setValue((uint8_t*) &macAddress, sizeof(macAddress));

  // Start BLE Services
  userService->start();
  psdiService->start();
}


void startAdvertising(void) {
  // Start Advertising
  BLEAdvertisementData scanResponseData = BLEAdvertisementData();
  scanResponseData.setFlags(0x06); // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  scanResponseData.setName(DEVICE_NAME);

  thingsServer->getAdvertising()->addServiceUUID(userService->getUUID());
  thingsServer->getAdvertising()->setScanResponseData(scanResponseData);
  thingsServer->getAdvertising()->start();
}
