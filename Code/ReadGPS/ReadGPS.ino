#include <TinyGPS++.h>
#include <SoftwareSerial.h>

unsigned long timer = 0;
unsigned long waitTime = 500;
static const int RXPin = 3, TXPin = 2;

// The TinyGPS++ object=
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
  timer = millis();
}

void loop(){
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      if(millis() > timer + waitTime){ 
        Serial.print(gps.location.lat(), 6);
        Serial.print(",");
        Serial.println(gps.location.lng(), 6);
//        Serial.print(" Satellites: ");
//        Serial.println(gps.satellites.value());
//        Serial.print(" Speed: ");
//        Serial.println(gps.speed.kmph());
        timer = millis();
      }
    }
  }
}
