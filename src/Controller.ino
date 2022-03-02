
#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

#define RED D2     
#define ORANGE D3     
#define GREEN D4
 
#define RED1 D5     
#define ORANGE1 D6     
#define GREEN1 D9

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED pin to output mode
  pinMode(RED, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED1, OUTPUT);
  pinMode(ORANGE1, OUTPUT);
  pinMode(GREEN1, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
     // if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()==0x00) { 
            //Accélération
          
          digitalWrite(RED, LOW);
          digitalWrite(ORANGE, LOW);   
          digitalWrite(GREEN, HIGH); 
          digitalWrite(RED1, HIGH);
          digitalWrite(ORANGE1, LOW);
          digitalWrite(GREEN1,LOW );    
        }
        else {           
                            
         if (switchCharacteristic.value()==0x01){
         digitalWrite(RED, LOW);
          digitalWrite(ORANGE, HIGH);   
          digitalWrite(GREEN,LOW ); 
          digitalWrite(RED1, HIGH);
          digitalWrite(ORANGE1, LOW);
          digitalWrite(GREEN1, LOW);
         }
         else {                             
           //Stop
          digitalWrite(RED, HIGH);
          digitalWrite(ORANGE, LOW);   
          digitalWrite(GREEN, LOW); 
          digitalWrite(RED1, LOW);
          digitalWrite(ORANGE1, LOW);
          digitalWrite(GREEN1, HIGH); 
          
         }
        }
      }
    }
 

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }