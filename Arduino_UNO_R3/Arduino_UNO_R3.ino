#include <SoftwareSerial.h>
#include <GSMSim.h>

// baud rates
#define baud_ESP 9600
#define baud_GSM 4800

#define DEBUG true
#define DEBUGBaud 115200

#define ACK "ACK"

// variables
int day,month,year,minute,second,hour; // time-keeping
String textSms,numberSms; // tracking SMSs

// RX, TX
SoftwareSerial ESP8266E(7, 8); 
GSMSim GSM(11, 10, A0); // . . RESET

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

  // RESET the GSM network
  GSM.reset();

  // For setting time from ntp servers, we must connect to internet!
  GSM.gprsConnectBearer();
  
  // the parameter to send is UTC of your country
  GSM.timeSetServer(6); // +6 for DHK // time server is: 202.120.2.101 (Shanghai Jiaotong University - China)
  //GSM.timeSetServer(6, "IP"); // to set a different server, replace IP with the server address

  // SYNC time with server
  GSM.timeSyncFromServer();

  // close the internet connection
  GSM.gprsCloseConn();

  // set the times
  GSM.timeGet(&day,&month,&year,&hour,&minute,&second);

  // Set to PDU mode
  GSM.smsTextMode(true);
  
  // wait for 3 seconds so proper network connections are up 
  delay(3000);
}

void loop() {
  // get the time
  GSM.timeGet(&day,&month,&year,&hour,&minute,&second);

  // check if there's any new messages
  // If no message found it returns NO_SMS else returns SMSIndexNo:x,y,z. 
  // If you have a lot of un read messages, return only SMSIndexNo:
  if(GSM.smsListUnread() != "NO_SMS") {
    numberSms = GSM.getNumberSms(1); // get the senders number
    textSms = GSM.smsRead(1, true); // get the contents of the sms and mark it as read

    // delete the SMS(s) marked read
    GSM.smsDeleteAllRead();
  }

  // clean the variables after processing each sms
  numberSms = "";
  textSms = "";
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
