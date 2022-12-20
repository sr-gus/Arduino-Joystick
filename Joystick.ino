#include "ezButton.h"

#define PIN_POT_X A0
#define PIN_POT_Y A1
#define PIN_PUSH_PAUSE 12
#define PIN_PUSH_UP 2
#define PIN_PUSH_DOWN 8
#define PIN_PUSH_LEFT 7
#define PIN_PUSH_RIGHT 4
#define PIN_LED_UP 5
#define PIN_LED_DOWN 9
#define PIN_LED_LEFT 3
#define PIN_LED_RIGHT 6
#define PIN_LED_Z 10

ezButton PIN_POT_Z(11);

bool POT_Z, LED_UP, LED_DOWN, LED_LEFT, LED_RIGHT, LED_Z = false,
      PUSH_UP, PUSH_DOWN, PUSH_LEFT, PUSH_RIGHT, PUSH_PAUSE, 
      PAUSE_STATE = false, LEDS_STATE = true,
      UP, DOWN, LEFT, RIGHT;
      
int POT_X, POT_Y, LED_SIGNAL, LED_DEFAULT = 168, currentTime;

void readPins() {
    POT_X = analogRead(PIN_POT_X);
    POT_Y = analogRead(PIN_POT_Y);
    POT_Z = !PIN_POT_Z.getState();
    
    PUSH_UP = digitalRead(PIN_PUSH_UP);
    PUSH_DOWN = digitalRead(PIN_PUSH_DOWN);
    PUSH_LEFT = digitalRead(PIN_PUSH_LEFT);
    PUSH_RIGHT = digitalRead(PIN_PUSH_RIGHT);
    PUSH_PAUSE = digitalRead(PIN_PUSH_RIGHT);
}

void ledUp() {
  if ( PUSH_UP ){
    LED_UP = true;
    analogWrite( PIN_LED_UP, LED_DEFAULT );
  }
  else if ( POT_Y > 511 ){
    LED_UP = true;
    LED_SIGNAL = ( POT_Y - 511 ) * LED_DEFAULT / 510;
    analogWrite( PIN_LED_UP, LED_SIGNAL );
  }
  else {
    LED_UP = false;
    digitalWrite( PIN_LED_UP, LOW);
  }
}

void ledDown() {
  if ( PUSH_DOWN ){
    LED_DOWN = true;
    analogWrite( PIN_LED_DOWN, LED_DEFAULT );
  }
  else if ( POT_Y < 511 ){
    LED_DOWN = true;
    LED_SIGNAL = ( 511 - POT_Y ) * LED_DEFAULT / 510;
    analogWrite( PIN_LED_DOWN, LED_SIGNAL );
  }
  else {
    LED_DOWN = false;
    digitalWrite( PIN_LED_DOWN, LOW);
  }
}

void ledLeft() {
  if ( PUSH_LEFT ){
    LED_LEFT = true;
    analogWrite( PIN_LED_LEFT, LED_DEFAULT );
  }
  else if ( POT_X > 511 ){
    LED_LEFT = true;
    LED_SIGNAL = ( POT_X - 515 ) * LED_DEFAULT / 510;
    analogWrite( PIN_LED_LEFT, LED_SIGNAL );
  }
  else {
    LED_LEFT = false;
    digitalWrite( PIN_LED_LEFT, LOW);
  }
}

void ledRight() {
  if ( PUSH_RIGHT ){
    LED_RIGHT = true;
    analogWrite( PIN_LED_RIGHT, LED_DEFAULT );
  }
  else if ( POT_X < 511 ){
    LED_RIGHT = true;
    LED_SIGNAL = ( 511 - POT_X ) * LED_DEFAULT / 510;
    analogWrite( PIN_LED_RIGHT, LED_SIGNAL );
  }
  else {
    LED_RIGHT = false;
    digitalWrite( PIN_LED_RIGHT, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_POT_X, INPUT);
  pinMode(PIN_POT_Y, INPUT);
  pinMode(PIN_PUSH_UP, INPUT);
  pinMode(PIN_PUSH_DOWN, INPUT);
  pinMode(PIN_PUSH_LEFT, INPUT);
  pinMode(PIN_PUSH_RIGHT, INPUT);
  pinMode(PIN_PUSH_PAUSE, INPUT);
  pinMode(PIN_LED_UP, OUTPUT); 
  pinMode(PIN_LED_DOWN, OUTPUT); 
  pinMode(PIN_LED_LEFT, OUTPUT); 
  pinMode(PIN_LED_RIGHT, OUTPUT);
  pinMode(PIN_LED_Z, OUTPUT); 
}

void loop() {
  PIN_POT_Z.loop();
  readPins();  
  
  if( POT_Z ) {
    if( !LED_Z )
      LEDS_STATE = !LEDS_STATE;
    LED_Z = true;
    analogWrite(PIN_LED_Z, LED_DEFAULT);
  }
  else {
    LED_Z = false;
    if ( !PAUSE_STATE )
      analogWrite(PIN_LED_Z, 0);
  }
  
  if( LEDS_STATE ){
    ledUp();
    ledDown();
    ledLeft();
    ledRight();
  }

  Serial.println()
}
