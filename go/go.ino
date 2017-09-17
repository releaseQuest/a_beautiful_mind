#include "SoftwareSerial.h"
#include "DFPlayer_Mini_Mp3.h"

#define LED 13
#define CMD A4

#define route0 10
#define route1 11

#define dfTX 6
#define dfRX 9

#define lock0 3
#define lock1 4
#define lock2 5

String inputString = "";  
boolean stringComplete = false;

boolean flag0 = true;
boolean flag1 = true;

SoftwareSerial df(9, 6); // RX, TX

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(CMD, OUTPUT);

  pinMode(lock0, OUTPUT);
  pinMode(lock1, OUTPUT);
  pinMode(lock2, OUTPUT);

  pinMode(route0, INPUT_PULLUP);
  pinMode(route1, INPUT_PULLUP);
  
  digitalWrite(CMD, LOW);
  digitalWrite(LED, LOW);

  digitalWrite(lock0, LOW);
  digitalWrite(lock1, LOW);
  digitalWrite(lock2, LOW);
  
  Serial.begin(9600); 
  delay(20);
  Serial.println("AT+C073");
  delay(20);
  digitalWrite(CMD, HIGH);
  delay(500);
  inputString = "";
  
  df.begin(9600);
  mp3_set_serial (df);
  delay (2000);
  mp3_set_volume (30);
  delay (100);
  mp3_play ();
  delay (100);
}

void loop() {
  
  if( !digitalRead(route0)){
    if(flag0){
      mp3_play ();
      delay(2000);
      digitalWrite(lock0, HIGH );
    }
    flag0 = false;
  } else {
    flag0 = true;
    digitalWrite(lock0, LOW );
  }
  
  if( !digitalRead(route1) ){
    if(flag1){
      mp3_play ();
      delay(2000);
      digitalWrite(lock1, HIGH );
    }
    flag1 = false;
  } else {
    digitalWrite(lock1, LOW );
    flag1 = true;
  }
  
  if (stringComplete) {
    Serial.println(inputString);
    if (inputString == "OPEN_CACHE\r\n") {
      mp3_play ();
      delay(1000);
      digitalWrite(lock2, HIGH);
      
    } else if(inputString == "CLOSE_CACHE\r\n"){
      digitalWrite(lock2, LOW);
    }
    inputString = "";
    stringComplete = false;
  }
  delay(200);
}

void serialEvent() {
  Serial.println(inputString);
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
