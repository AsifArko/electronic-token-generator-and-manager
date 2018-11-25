#include <GSMSim.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// baud rates
#define baud_ESP 9600
#define baud_GSM 4800

#define DEBUG true
#define DEBUGBaud 115200

#define LCD_contrast 20
#define LCD_contrast_pin 6

#define ACK "ACK"

// variables
int day,month,year,minute,second,hour; // time-keeping
String textSms,numberSms; // tracking SMSs
int currentToken = 0;
String IP;

// RX, TX
SoftwareSerial ESP8266E(7, 8); 
GSMSim GSM(11, 10, A0); // . . RESET
LiquidCrystal LCD(12, 13, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7

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

  // set contrast with analog value to run LCD without potentiometer
  analogWrite(LCD_contrast_pin, LCD_contrast);

  // begin 16x2 LCD Screen
  LCD.begin(16, 2);

  // print welcome message
  LCD.setCursor(0, 0);
  LCD.print(F("Welcome... "));

  // wait 1.5 sec and wipe the screen
  delay(1500);
  LCD.clear();

  // print a 0 on the screen initially
  LCD.print(currentToken);
  
  lcd_PrintBottom();
}

void loop() {
  // get the time
  GSM.timeGet(&day,&month,&year,&hour,&minute,&second);
  lcd_PrintBottom();
  
  // check if there's any new messages
  // If no message found it returns NO_SMS else returns SMSIndexNo:x,y,z. 
  // If you have a lot of un read messages, return only SMSIndexNo:
  if(GSM.smsListUnread() != "NO_SMS") {
    // the following method is not included in the library. todo: do a pull request
    numberSms = GSM.getNumberSms(1); // get the senders number
    textSms = GSM.smsRead(1, true); // get the contents of the sms and mark it as read

    // send the number and sms content to nodeMcu to send it over to the server
    
    // delete the SMS(s) marked read
    GSM.smsDeleteAllRead();
  }

  // clean the variables after processing each sms
  numberSms = "";
  textSms = "";

  // see if any message is ariving from the NodeMcu
  if(ESP8266E.available()) {
    String msg = ESP8266E.readString();
    if(msg.equalsIgnoreCase("NEXT")){
      LCD.setCursor(0, 0);
      LCD.print(++currentToken);
    } else if(msg.startsWith("IP"){
      IP = msg.substring(2);
      LCD.setCursor(0, 0);
      LCD.print(IP);
      delay(3000);
      LCD.print(currentToken);
    }
  }
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

// view date and time with signal strenght on the bottom row
void lcd_PrintBottom() {
  // show the time and date with the signal strength of the GSM both on the bottom line
  LCD.setCursor(0, 1);
  char bottomRow[16];
  sprintf(bottomRow, "%d/%d, %d:%d, %u", day, month, hour, minute, GSM.signalQuality()); // format the string
  LCD.print(bottomRow);
}
