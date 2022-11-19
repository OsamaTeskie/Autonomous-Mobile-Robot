#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"


DeviceDriverSet_Motor AppMotor;

int carSpeed = 60;

void setup() {
  Serial.begin(9600);

  pinMode(31, INPUT);  // left IR
  pinMode(33, INPUT);  // middle IR
  pinMode(35, INPUT);  // right IR
  pinMode(52, INPUT);  // right IR table
  pinMode(50, INPUT);  // middle IR table
  pinMode(48, INPUT);  // left IR table

  AppMotor.DeviceDriverSet_Motor_Init();
  delay(2000);
}

void moveCar(SmartRobotCarMotionControl direction, uint8_t speed) {
  ApplicationFunctionSet_SmartRobotCarMotionControl(direction /*direction*/, speed /*speed*/);
}

int blackRight(void) {
  return digitalRead(26);
}

int blackMiddle(void) {
  return digitalRead(24);
}

int blackLeft(void) {
  return digitalRead(22);
}


void loop() {

  int leftLine = blackLeft();
  int middleLine = blackMiddle();
  int rightLine = blackRight();
  int leftIR = digitalRead(31);
  int middleIR = digitalRead(33);
  int rightIR = digitalRead(35);
  int leftSwitchState = digitalRead(48);
  // int frontSwitchState = digitalRead(50);
  int rightSwitchState = digitalRead(52);

  if ((leftLine == 0 && middleLine == 0 && rightLine == 0) || (leftLine == 0 && middleLine == 1 && rightLine == 0)) {
    // Serial.println("DRIVE STRAIGHT");
    if (leftIR == 1 && middleIR == 1 && rightIR == 1) {
      if (leftSwitchState == LOW && rightSwitchState == LOW) {
        moveCar(0, carSpeed);

      } else if (leftSwitchState == HIGH && rightSwitchState == LOW) {
        //move back
        // Serial.println("HERE");
        moveCar(7, carSpeed);
        delay(250);
        // move right
        moveCar(3, carSpeed);
        delay(700);

      } else if (leftSwitchState == LOW && rightSwitchState == HIGH) {
        //move back
        moveCar(5, carSpeed);
        delay(250);
        // move left
        moveCar(2, carSpeed);
        delay(700);
        // Serial.println("GOING LEFT");
      } 
      // else if (leftSwitchState == LOW && frontSwitchState == HIGH && rightSwitchState == HIGH) {
      //   // move back
      //   moveCar(1, carSpeed);
      //   delay(100);
      //   // move left
      //   moveCar(2, carSpeed);
      //   delay(700);
      // } else if (leftSwitchState == HIGH && frontSwitchState == HIGH && rightSwitchState == LOW) {
      //   // Move back
      //   moveCar(1, carSpeed);
      //   delay(100);
      //   // move right
      //   moveCar(3, carSpeed);
      //   delay(700);
      //   // Serial.println("HERE");
      // } 
      else if (leftSwitchState == LOW &&  rightSwitchState == LOW) {
        // When the middle switch is set off, move back for 200ms
        moveCar(1, carSpeed);
        delay(200);

        moveCar(3, carSpeed);
        delay(1000);
        // moveCar(8, carSpeed);
      }
    } else if ((leftIR == 1 && middleIR == 1 && rightIR == 0) || (leftIR == 1 && middleIR == 0 && rightIR == 0)) {

      if (leftSwitchState == LOW) {
        moveCar(1, carSpeed);
        delay(200);
        moveCar(2, carSpeed);
        delay(450);
      } else {
        moveCar(8, carSpeed);
        while (rightIR == 0) {
          moveCar(1, carSpeed);
          rightIR = digitalRead(35);
        }
        moveCar(3, carSpeed);
        delay(700);
      }
    } else if ((leftIR == 0 && middleIR == 1 && rightIR == 1) || (leftIR == 0 && middleIR == 0 && rightIR == 1)) {

      if (rightSwitchState == LOW) {
        moveCar(1, carSpeed);
        delay(200);
        moveCar(3, carSpeed);
        delay(550);
      } else {
        while (leftIR == 0) {
          moveCar(1, carSpeed);
          leftIR = digitalRead(31);
        }
        moveCar(2, carSpeed);
        delay(700);
      }
    } else if (leftIR == 1 && middleIR == 0 && rightIR == 1) {
      moveCar(1, carSpeed);
      delay(200);

      moveCar(2, carSpeed);
      delay(1000);
    }

  } else if (leftLine == 1 && middleLine == 1 && rightLine == 1) {
    while (leftLine == 1 && rightLine == 1){
      moveCar(3, carSpeed);
      leftLine = blackLeft();
      rightLine = blackRight();
    }
    
  } else if ((leftLine == 1 && middleLine == 0 && rightLine == 0) || (leftLine == 1 && middleLine == 1 && rightLine == 0)) {
    // Serial.println("GO LEFT UNTIL ONLY MIDDLE IS >= 100 ");
    // Move Left
    if (leftIR == 1 && leftSwitchState == LOW) {
      moveCar(2, carSpeed);
    } else {
      // checking();
      if (rightIR == 1 && rightSwitchState == LOW) {
        moveCar(3, carSpeed);
        delay(700);
      } else {
        while (leftIR == 0) {
          moveCar(1, carSpeed);
          leftIR = digitalRead(31);
        }
        moveCar(2, carSpeed);
        delay(700);
      }
      // delay(500);
    }

  } else if ((leftLine == 0 && middleLine == 0 && rightLine == 1) || (leftLine == 0 && middleLine == 1 && rightLine == 1)) {
    // Serial.println("GO RIGHT UNTIL ONLY MIDDLE IS >= 100 ");
    // moveCar(3, carSpeed);

    if (rightIR == 1 && rightSwitchState == LOW) {
      moveCar(3, carSpeed);
    } else {
      // checking();
      if (leftIR == 1 && leftSwitchState == LOW) {
        moveCar(2, carSpeed);
        delay(700);
      } else {
        while (rightIR == 0) {
          moveCar(1, carSpeed);
          rightIR = digitalRead(35);
        }
        moveCar(3, carSpeed);
        delay(700);
      }
    }
  }
}