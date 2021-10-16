#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <EEPROM.h>

#define EEPROM_SIZE 64
#define SERVICE_UUID "5e87bd74-b350-4a6a-ae36-bb33e30499af"
#define CHARATERISTIC_DATA_UUID "2e550f9f-c882-467a-b35a-c7f12ecae2b3"
#define DELAY 250
#define motorDriveIN1 2
#define motorDriveIN2 4
#define motorDriveIN3 5
#define motorDriveIN4 18

bool _messageReceivedComplete = false;
BLECharacteristic *pCharacteristicData;
std::string _message;
// std::string _data = "<!DOCTYPE html><html><body><h1>BLE Button Admin Page</h1><p>Hello</p></body></html>";

class ServerReadCallbacks: public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
      _message = pCharacteristic->getValue();
      _messageReceivedComplete = true;
    }
};

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  EEPROM.begin(EEPROM_SIZE);
  String deviceName = EEPROM.readString(0);
  Serial.println("Name " + deviceName);
  int nameLength = deviceName.length();
  if(nameLength == 0 ||
      nameLength == 1 && 
        deviceName == "0")
  {
    deviceName = "BLE ESP32";
  }

  BLEDevice::init(deviceName.c_str());
  BLEServer *pServer = BLEDevice::createServer();
  //pServer->setCallbacks(new EchoServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristicData = pService->createCharacteristic(
                          CHARATERISTIC_DATA_UUID,
                          BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE
                        );
  pCharacteristicData->addDescriptor(new BLE2902());
  pCharacteristicData->setCallbacks(new ServerReadCallbacks());


  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  Serial.println("Characteristic defined.");
  pinMode(motorDriveIN1, OUTPUT);
  pinMode(motorDriveIN2, OUTPUT);
  pinMode(motorDriveIN3, OUTPUT);
  pinMode(motorDriveIN4, OUTPUT);
}

void loop()
{
  delay(DELAY);

  if (_messageReceivedComplete)
  {
    _messageReceivedComplete = false;
    int length = _message.length();
    if (length > 0)
    {
 
      if(_message == "Start")
        moveForward();
	  if(_message == "Stop")
        moveStop();
      Serial.println("Code Recieved");

    }
  }
}

void moveForward(){
  //motor R
  digitalWrite(motorDriveIN1, LOW); 
  digitalWrite(motorDriveIN2, HIGH);
  //motor L
  digitalWrite(motorDriveIN3, LOW); 
  digitalWrite(motorDriveIN4, HIGH); 

}

void moveBackward(){
  //motor R
  digitalWrite(motorDriveIN1, HIGH);
  digitalWrite(motorDriveIN2, LOW);
  //motor L
  digitalWrite(motorDriveIN3, HIGH);
  digitalWrite(motorDriveIN4, LOW);
}

void moveStop(){
  //motor R
  digitalWrite(motorDriveIN1, LOW);
  digitalWrite(motorDriveIN2, LOW);
  //motor L
  digitalWrite(motorDriveIN3, LOW);
  digitalWrite(motorDriveIN4, LOW);  
}
