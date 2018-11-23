#include <SoftwareSerial.h>
#include <Sim800L.h>

// baud rates
#define baud_ESP 9600
#define baud_GSM 4800

#define DEBUG true
#define DEBUGBaud 115200

#define ACK "ACK"

// RX, TX
SoftwareSerial ESP8266E(7, 8); 
Sim800L GSM(11, 10);

void setup() {
  // set BULTIN LED as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // begin ESP8266
  ESP8266E.begin(baud_ESP);

  // wait while it connects
  while(!ESP8266E) {
    ;
  }

  // blink thrice
  Blink(LED_BUILTIN, 3, 500);

  // blink once for 1 sec
  Blink(LED_BUILTIN, 1, 1000);

  // begin SIM800L
  GSM.begin(baud_GSM);

  // wait while it connects
  while(!GSM){
    ;
  }

  // blink thrice
  Blink(LED_BUILTIN, 3, 300);

  // blink once for 1 sec
  Blink(LED_BUILTIN, 1, 1000);
  
}

void loop() {
  
}

// sends signal on a pin on defined parameter
void Blink(int pin, int times, int DELAY){
  int i;
  for(i = 0; i < times; i++){
    digitalWrite(pin, HIGH);
    delay(DELAY);
    digitalWrite(pin, LOW);
    delay(DELAY);
  }
}
