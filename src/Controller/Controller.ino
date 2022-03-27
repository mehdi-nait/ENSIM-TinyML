
#include <ArduinoBLE.h>

BLEService BLService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE Service


BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);// Définir les aratéristiques de l'arduino nano pour établir la commuication Bluetoth


//Définir les pins 
#define RED D2     
#define ORANGE D3     
#define GREEN D4
 
#define RED1 D5     
#define ORANGE1 D6     
#define GREEN1 D9

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // définir les pins en mode de sortie
  pinMode(RED, OUTPUT);
  pinMode(ORANGE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED1, OUTPUT);
  pinMode(ORANGE1, OUTPUT);
  pinMode(GREEN1, OUTPUT);

  //  initialisation de la communiation
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // changer le nom de la arte
  BLE.setLocalName("TinyML");
  BLE.setAdvertisedService(BLService);

  // ajout des aratéristiques au servie de la carte
  BLService.addCharacteristic(switchCharacteristic);

  // L'ajout du  service
  BLE.addService(BLService);

  // Initialisation des caractéristiqes de la carte
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE Peripheral");
}

void loop() {
  // Demande de connexion:
  BLEDevice central = BLE.central();

  // Si les deux cartes sont connetés:
  if (central) {
    Serial.print("Connected to central: ");
    // Ecrire l'adresse MAC:
    Serial.println(central.address());

    // Si les deux cartes sont connetés:
    while (central.connected()) {
      
     
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
           //Ralentir                
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
 

    // Si les deux cartes sont connectés:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
