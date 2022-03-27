#include <ArduinoBLE.h>

// variables for button

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // initialize the BLE hardware
  BLE.begin();

  Serial.println("GESTURE RECOGNITION - ENSIM 2022");

  // start scanning for peripherals
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "TinyML") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");

  if (!ledCharacteristic) {
    Serial.println("Peripheral does not have characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canWrite()) {
    Serial.println("Peripheral does not have a writable characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) 
  {
    // while the peripheral is connected

    // read the button pin
    //int buttonState = digitalRead(buttonPin);

   // if (oldButtonState != buttonState) {
      // button changed
      //oldButtonState = buttonState;
      for(int a=0;a<3;a++){
      if (a==0) {
        // button is pressed, write 0x01 to turn the LED on
        ledCharacteristic.writeValue((byte)0x01);
        delay(5000);
                } 
      else {
        if(a==1){
        // button is released, write 0x00 to turn the LED off
        ledCharacteristic.writeValue((byte)0x00);
        delay(5000);
               }
      else{
        ledCharacteristic.writeValue((byte)0x10);
        delay(5000);
          }
        }
      }
  }

  Serial.println("Peripheral disconnected");
}
