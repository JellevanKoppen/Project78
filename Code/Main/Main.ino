#include <TinyGPS++.h>
#include <SoftwareSerial.h>

unsigned long timer = 0;
unsigned long waitTime = 500;
static const int RXPin = 3, TXPin = 2;

// The TinyGPS++ object=
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  timer = millis();
}

void loop() {
  /* ===== BLUETOOTH CONTROL ===== */
  if(BTSerial.available() > 0){
    inputSignal = BTSerial.read();
    switch(inputSignal){
      case '5': // Send GPS Data
        Serial.println("Spinning");
        esc1 = minimumThrottle;
        esc2 = minimumThrottle;
        esc3 = minimumThrottle;
        esc4 = minimumThrottle;
        break;
      default:
        Serial.println("No valid input received");
        esc1 = stopMotor;
        esc2 = stopMotor;
        esc3 = stopMotor;
        esc4 = stopMotor;
        break;
    }
  }

}
