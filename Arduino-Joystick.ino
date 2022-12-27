#include "ezButton.h"
#include "TimedAction.h"

#define PIN_POT_X A0
#define PIN_POT_Y A1
#define PIN_PUSH_PAUSE 2
#define PIN_PUSH_UP 12
#define PIN_PUSH_DOWN 8
#define PIN_PUSH_LEFT 7
#define PIN_PUSH_RIGHT 4
#define PIN_LED_UP 5
#define PIN_LED_DOWN 9
#define PIN_LED_LEFT 3
#define PIN_LED_RIGHT 6
#define PIN_LED_Z 10

ezButton PIN_POT_Z(11);

volatile byte PUSH_PAUSE = false;

bool POT_Z, PUSH_UP, PUSH_DOWN, PUSH_LEFT, PUSH_RIGHT, 
      PAUSE = false, LEDS_STATE = true, STATE_CHANGED;

int POT_X, POT_Y, UP = 0, DOWN = 0, LEFT = 0, RIGHT = 0;

void processPot(){
  if ( !LEFT && !RIGHT ) {
    if ( POT_X < 512 ){
      RIGHT = -5*POT_X/511 + 5;
      LEFT = 0;
    }
    else {
      LEFT = POT_X/102 - 171/34;
      RIGHT = 0;
    }
  }
  
  if ( !UP && !DOWN ) {
    if ( POT_Y > 512 ){
      UP = POT_Y/102 - 171/34;
      DOWN = 0;
    }
    else {
      DOWN = -5*POT_Y/511 + 5;
      UP = 0;
    }
  }

  if ( POT_Z ) {
    LEDS_STATE = !LEDS_STATE;
    analogWrite(PIN_LED_Z, 168);
  }
  else
    analogWrite(PIN_LED_Z, 0);
}

void processPushUpRight(){
  if ( !LEFT && PUSH_RIGHT ) {
      RIGHT = 5;
      LEFT = 0;
  }
  if ( !DOWN && PUSH_UP ) {
      UP = 5;
      DOWN = 0;
  }
}

void processPushDownLeft(){
  if ( !RIGHT && PUSH_LEFT ) {
      LEFT = 5;
      RIGHT = 0;
  }
  if ( !UP && PUSH_DOWN ) {
      DOWN = 5;
      UP = 0;
  }
}

void processPushPause(){
  PUSH_PAUSE = true;
  PAUSE = !PAUSE;
  analogWrite(PIN_LED_Z, 168);
}

void leds() {
  if ( LEDS_STATE ) {
    analogWrite( PIN_LED_UP, 168*UP/5 );
    analogWrite( PIN_LED_DOWN, 168*DOWN/5 );
    analogWrite( PIN_LED_LEFT, 168*LEFT/5 );
    analogWrite( PIN_LED_RIGHT, 168*RIGHT/5 );
  }
  else {
    analogWrite( PIN_LED_UP, 0 );
    analogWrite( PIN_LED_DOWN, 0 );
    analogWrite( PIN_LED_LEFT, 0 );
    analogWrite( PIN_LED_RIGHT, 0 );
  }
}

void readPot(){
  POT_X = analogRead(PIN_POT_X);
  POT_Y = analogRead(PIN_POT_Y);
  POT_Z = !PIN_POT_Z.getState();
  processPot();
}

void readPushUpRight(){
  PUSH_UP = digitalRead(PIN_PUSH_UP);
  PUSH_RIGHT = digitalRead(PIN_PUSH_RIGHT);
  processPushUpRight();
}

void readPushDownLeft(){
  PUSH_DOWN = digitalRead(PIN_PUSH_DOWN);
  PUSH_LEFT = digitalRead(PIN_PUSH_LEFT);
  processPushDownLeft();
}

void readPushPause(){
  if (PUSH_PAUSE) {
    PUSH_PAUSE = false;
    analogWrite(PIN_LED_Z, 0);
  }
}

void reset(){
  UP = 0;
  DOWN = 0;
  RIGHT = 0;
  LEFT = 0;
}

void printData(){
  Serial.print("U");
  Serial.println(UP);
  Serial.print("D");
  Serial.println(DOWN);
  Serial.print("L");
  Serial.println(LEFT);
  Serial.print("R");
  Serial.println(RIGHT);
  Serial.print("P");
  Serial.println(PAUSE);
}

TimedAction POT_CONTROL(20, readPot);
TimedAction PUSH_CONTROL_1(20, readPushUpRight);
TimedAction PUSH_CONTROL_2(20, readPushDownLeft);
TimedAction PUSH_CONTROL_3(10, readPushPause);
TimedAction LED_CONTROL(20, leds);
TimedAction RESET(18, 50, reset);
TimedAction PRINT(10, printData);

void setup() {
  Serial.begin(115200);
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
  attachInterrupt(digitalPinToInterrupt(PIN_PUSH_PAUSE),
                  processPushPause,
                  FALLING);
}

void loop() {
  PIN_POT_Z.loop();
  PUSH_CONTROL_1.check();
  PUSH_CONTROL_2.check();
  PUSH_CONTROL_3.check();
  POT_CONTROL.check();
  LED_CONTROL.check();
  RESET.check();
  PRINT.check();
}
