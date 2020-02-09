#include <TinyGPS++.h>
#include <SoftwareSerial.h>

unsigned long timer = 0;
char inputSignal;

// The TinyGPS++ object=
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial GPSSerial(3, 2);                                            // (RX,TX)
SoftwareSerial BTSerial(12, 13);                                           // (RX,TX)

void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to AFD Systems");
  Serial.println("Starting...");
  GPSSerial.begin(9600);
  BTSerial.begin(9600);
}

void getSignal() {
  GPSSerial.listen();
  unsigned long timer = millis();
  bool reading = true;
  while(millis() < timer + 1000 && reading == true){
    delayMicroseconds(50);
    while (GPSSerial.available() > 0){
      reading = false;
      gps.encode(GPSSerial.read());
      if(gps.location.isUpdated()){
        Serial.println("Location Updated!");
      }
    }
  }
}

void loop() {
  BTSerial.listen();
  if (BTSerial.available() > 0) {
    inputSignal = BTSerial.read();
    switch (inputSignal) {
      case 'g':                                                              // Send GPS Data
        getSignal();
        Serial.println("Sending..");
        Serial.println(gps.location.lat(), 6);
        Serial.println(gps.location.lng(), 6);
        BTSerial.print(gps.location.lat(), 6);
        BTSerial.print(",");
        BTSerial.print(gps.location.lng(), 6);
        break;
      default:
        Serial.println("No valid input received");
        Serial.print("received: ");
        Serial.println(inputSignal);
        break;
    }
  }

}
