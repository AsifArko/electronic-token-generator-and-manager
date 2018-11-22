#include <SoftwareSerial.h>

#define baud 9600
#define DEBUG true
#define DEBUGBaud 115200
#define ACK "ACK"

SoftwareSerial ESP8266E(7, 8); // RX, TX

void setup() {
  // set BULTIN LED as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // begin ESP8266
  ESP8266E.begin(baud);

  // wait while it connects
  while(!ESP8266E) {
    ;
  }
  
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
