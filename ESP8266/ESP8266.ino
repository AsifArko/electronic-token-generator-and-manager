#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#define LED0 2 // the one at the board
#define baud 9600
#define ssid "---------"
#define ssid_password "---------"
#define DEBUG true
#define DEBUGBaud 115200

SoftwareSerial main(12, 13); // RX, TX

void setup() {
  
  pinMode(LED0, OUTPUT);

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
  
  // connect WiFi
  WiFi.begin(ssid, ssid_password);
  
  // wait while WiFi connects
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // blink thrice upon connection
  Blink(LED0, 3, 500);

  if(DEBUG) {
    debugWiFi();
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

void debugWiFi() {
  Serial.begin(DEBUGBaud);
  delay(100);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
}
