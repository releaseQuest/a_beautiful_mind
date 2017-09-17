#define LED 13
#define CMD 5

#define lock A5

#define plugs A4
#define key   A3

#define lRed A2
#define lYel A1
#define lGrn A0

#define btn 13

bool onceOpened = true;
bool onceClosed = true;
bool openFlag = true;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(CMD, OUTPUT);

  pinMode(lRed, OUTPUT);
  pinMode(lYel, OUTPUT);
  pinMode(lGrn, OUTPUT);

  pinMode(lock, OUTPUT);

  pinMode(key, INPUT_PULLUP);
  pinMode(btn, INPUT_PULLUP);

  digitalWrite(CMD, LOW);
  Serial.begin(9600);
  delay(20);
  Serial.println("AT+C073");
  delay(20);
  digitalWrite(CMD, HIGH);

  digitalWrite(lRed, LOW);
  digitalWrite(lYel, LOW);
  digitalWrite(lGrn, LOW);

  digitalWrite(lock, LOW);
}

void loop() {
  if( !digitalRead(btn) ) {
    digitalWrite(lock, HIGH);
  } else if( digitalRead(key) ) {
    digitalWrite(lock, LOW);
  }
  
  if ( checkPlugs() ) {
    onceClosed = true;
    if (openFlag) {
      digitalWrite(lRed, LOW);
      digitalWrite(lYel, HIGH);
      digitalWrite(lGrn, LOW);
      if (onceOpened) {
        digitalWrite(lock, LOW);
        Serial.println("OPEN_CACHE");
        onceOpened = false;
      }
      delay(100);
    }
    if ( !digitalRead(key) ) {
      digitalWrite(lock, HIGH);
      digitalWrite(lRed, LOW);
      digitalWrite(lYel, LOW);
      digitalWrite(lGrn, HIGH);
      openFlag = false;
    } else{
      if( digitalRead(btn) ) {
        digitalWrite(lock, LOW);
      }
      digitalWrite(lRed, LOW);
      digitalWrite(lYel, HIGH);
      digitalWrite(lGrn, LOW);
      openFlag = false;
    }
  } else if(onceClosed){
    digitalWrite(lock, LOW);
    digitalWrite(lRed, HIGH);
    digitalWrite(lYel, LOW);
    digitalWrite(lGrn, LOW);
    Serial.println("CLOSE_CACHE");
    onceOpened = true;
    openFlag = true;
    onceClosed = false;
  }
  delay(100);
}

bool checkPlugs(){
  unsigned int val = analogRead( plugs );
  if(val <= 180 && val >= 160) return true;
  else return false;
}
