#include <SoftwareSerial.h>

#define LED0 2 // the one at the board
#define baud 9600 

SoftwareSerial main(12, 13); // RX, TX

void setup() {
  
  pinMode(LED, OUTPUT);

  // begin main at baud rate: 9600
  main.begin(baud);
  while(!main){
    ;
  }

  // send an ack of success
  main.println("ACK");
  
  // wait for an ack back
  while(main.available()){
    if(main.readString() == "ACK") {
      break;
    }
  }

  // blink 3 times upon successful communication with the main board.
  Blink(LED0, 3, 200);
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
