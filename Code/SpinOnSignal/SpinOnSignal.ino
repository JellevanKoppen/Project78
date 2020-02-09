/* Author: Jelle van Koppen
 * Date: 20-05-2019
 * Team: Clear Skies
 * 
 * Purpose:
 * Spin the motors of the AFDrone once
 * an input is received form the mobile App.
 * 
 */

/* ===== LIBRARIES ===== */
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(12,13);                                            // (RX,TX)

/* ===== GLOBAL VARIABLES ===== */
int minimumThrottle = 1100;                                                // Minimum throttle value required to spin motors
int stopMotor = 1000;                                                      // Value to stop motors from spinning
int esc1 = stopMotor,esc2 = stopMotor,esc3 = stopMotor,esc4 = stopMotor;   // Electronic Speed Controllers
unsigned long loopTimer;
unsigned long timerChannel1, timerChannel2, timerChannel3, timerChannel4, escTimer, escLoopTimer;
char inputSignal;


void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to AFD Systems");
  Serial.println("Starting...");
  DDRD |= B11110000;                                                        //Configure digital poort 4, 5, 6 and 7 as output.
  BTSerial.begin(38400);
  loopTimer = micros();                                                     //Set the timer for the next loop.
}

void loop() {

  /* ===== BLUETOOTH CONTROL ===== */
  if(BTSerial.available() > 0){
    inputSignal = BTSerial.read();
    switch(inputSignal){
      case '1': // Spin motors
        Serial.println("Spinning");
        esc1 = minimumThrottle;
        esc2 = minimumThrottle;
        esc3 = minimumThrottle;
        esc4 = minimumThrottle;
        break;
      case '0': // Stop motors
        Serial.println("Stopping");
        esc1 = stopMotor;
        esc2 = stopMotor;
        esc3 = stopMotor;
        esc4 = stopMotor;
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

  /* ===== ESC CONTROL ===== */
  
  PORTD |= B11110000;                                                        //Set digital outputs 4,5,6 and 7 high.
  timerChannel1 = esc1 + loopTimer;                                          //Calculate the time of the faling edge of the esc-1 pulse.
  timerChannel2 = esc2 + loopTimer;                                          //Calculate the time of the faling edge of the esc-2 pulse.
  timerChannel3 = esc3 + loopTimer;                                          //Calculate the time of the faling edge of the esc-3 pulse.
  timerChannel4 = esc4 + loopTimer;                                          //Calculate the time of the faling edge of the esc-4 pulse.
  
  while(PORTD >= 16){                                                        //Stay in this loop until output 4,5,6 and 7 are low.
    escLoopTimer = micros();                                                 //Read the current time.
    if(timerChannel1 <= escLoopTimer)PORTD &= B11101111;                     //Set digital output 4 to low if the time is expired.
    if(timerChannel2 <= escLoopTimer)PORTD &= B11011111;                     //Set digital output 5 to low if the time is expired.
    if(timerChannel3 <= escLoopTimer)PORTD &= B10111111;                     //Set digital output 6 to low if the time is expired.
    if(timerChannel4 <= escLoopTimer)PORTD &= B01111111;                     //Set digital output 7 to low if the time is expired.
  }
  
  while(micros() - loopTimer < 4000);                                       //Wait until 4000us are passed.
  loopTimer = micros();                                                     //Set the timer for the next loop.
}
