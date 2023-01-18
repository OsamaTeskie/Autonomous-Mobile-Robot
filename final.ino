#include <Servo.h>
#include <avr/wdt.h>
#include "DeviceDriverSet_xxx0.h"
#include "ApplicationFunctionSet_xxx0.cpp"


DeviceDriverSet_Motor AppMotor;

Servo front;  // create servo object to control a servo
Servo back;   // create servo object to control a servo

#define enA A8
#define enB A9

#define in1 24
#define in2 22

#define in3 26
#define in4 28

int carSpeed = 60;
int moveTo = 1;  // start on left
int parked = 0;

// variable to read the value from the analog pin

void stopSide(int one, int two) {
  digitalWrite(one, LOW);
  digitalWrite(two, LOW);
}
void moveRight(int one, int two) {
  digitalWrite(one, LOW);
  digitalWrite(two, HIGH);
}
void moveLeft(int one, int two) {
  digitalWrite(one, HIGH);
  digitalWrite(two, LOW);
}

// 1 back, 2 middle, 3 front edge, 4 objects
int getDistance(int sensor = 4) {
  int trig;
  int echo;
  if (sensor == 1) {
    trig = 32;
    echo = 36;
  } else if (sensor == 2) {
    trig = 29;
    echo = 27;
  } else if (sensor == 3) {
    trig = 47;
    echo = 44;
  } else {
    trig = 35;
    echo = 37;
  }

  long duration;  // variable for the duration of sound wave travel
  int distance;
  // Clears the trigPin condition
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  return distance;
}

void parkRight() {
  carSpeed = 60;
  int frontEdge = digitalRead(31);
  int backEdge = digitalRead(30);
  moveCar(8, carSpeed);
  analogWrite(A8, 255);  // Send PWM signal to L298N Enable pin
  analogWrite(A9, 255);
  analogWrite(12, 192);

  back.write(110);  //lowered
  front.write(110);
  moveRight(in1, in2);
  delay(610);
  stopSide(in1, in2);
  back.write(75);  //raised
  front.write(75);

  moveCar(0, carSpeed);
  delay(1900);
  moveCar(8, carSpeed);
  int parkDistance = 45;
  int backUltra = getDistance(1);
  int middleUltra = getDistance(2);
  int frontUltra = getDistance(3);
  int time = millis();
  while (backUltra < parkDistance || middleUltra < parkDistance) {
    Serial.println(backUltra);
    int current = millis();
    if ((current - time >= 1350 && current - time < 1450)) {
      moveCar(2, carSpeed);
      delay(150);
    }
    moveCar(0, carSpeed);
    // moveCar(2, carSpeed);
    // delay(20);
    backUltra = getDistance(1);
    middleUltra = getDistance(2);
    frontUltra = getDistance(3);
  }
  moveCar(8, carSpeed);

  back.write(110);  //raised
  front.write(110);
  // //   delay(500);
  time = millis();
  while (frontEdge == LOW || backEdge == LOW) {
    if (frontEdge == LOW || backEdge == LOW) {
      stopSide(in1, in2);
      stopSide(in3, in4);
      // delay(200);
      moveRight(in1, in2);
      moveRight(in3, in4);

    } else if (frontEdge == LOW || backEdge == HIGH) {
      // stop the back motor
      stopSide(in1, in2);
      // delay(300);
      // move the front motor right
      moveRight(in3, in4);
    } else if (frontEdge == HIGH && backEdge == LOW) {
      // stop the front motor
      stopSide(in3, in4);
      // delay(275);
      // move the back motor right
      moveRight(in1, in2);
    }
    frontEdge = digitalRead(31);
    backEdge = digitalRead(30);
    // moveCar(8, carSpeed);
    // delay(500);
  }
   time = millis();
  int current = millis();

  while (current - time <= 250){
    stopSide(in1, in2);
    stopSide(in3, in4);
    moveLeft(in1, in2);
    moveLeft(in3, in4);
    current = millis();
  }
  // moveLeft(in1, in2);
  // moveLeft(in3, in4);
  // delay(300);
  // stop the parking motors and raise them
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
  back.write(75);  //raised
  front.write(75);
}


void setup() {
  Serial.begin(9600);
  back.attach(10);
  front.attach(11);

  pinMode(34, INPUT);   // Back left IR
  pinMode(36, INPUT);   // Back Right Ultra Echo
  pinMode(32, OUTPUT);  // Back Right Ultra trig
  // pinMode(33, INPUT);  // Middle right  IR
  pinMode(38, INPUT);  // Left back stop IR
  pinMode(40, INPUT);  // RIght back stop IR

  pinMode(27, INPUT);   // middle Ultra echo
  pinMode(29, OUTPUT);  // middle Ultra trig

  pinMode(51, INPUT);  // Right front stop IR
  pinMode(49, INPUT);  // left front stop IR

  pinMode(42, INPUT);   // Front left IR
  pinMode(44, INPUT);   // Front Right Ultra echo
  pinMode(47, OUTPUT);  // Front Right Ultra trig
  pinMode(46, INPUT);   // left line
  pinMode(39, INPUT);   // double left line
  pinMode(48, INPUT);   // middle line
  pinMode(50, INPUT);   // right line
  pinMode(53, INPUT);   // double right line
  pinMode(52, INPUT);   // heat left sensor
  pinMode(35, OUTPUT);  // Sets the trigPin as an OUTPUT
  pinMode(37, INPUT);
  pinMode(31, INPUT);  // front Edge
  pinMode(30, INPUT);  // back  Edge
  pinMode(A14, OUTPUT);
  pinMode(12, OUTPUT);  // voltage control
  pinMode(A8, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(A9, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  back.write(75);  //raised
  front.write(75);

  // digitalWrite(A8, 0);  // Send PWM signal to L298N Enable pin
  // digitalWrite(A9, 0);
  delay(3000);
  // parkLeft(carSpeed);
}
void moveCar(SmartRobotCarMotionControl direction, uint8_t speed) {
  ApplicationFunctionSet_SmartRobotCarMotionControl(direction /*direction*/, speed /*speed*/);
}

void loop() {

  int backLeft = digitalRead(34);
  int backRight = digitalRead(36);
  int frontLeft = digitalRead(42);
  int frontRight = digitalRead(44);
  int heatLeft = digitalRead(52);
  int heatRight = digitalRead(53);
  int leftLine = digitalRead(46);
  int doubleLeftLine = digitalRead(39);
  int middleLine = digitalRead(48);
  int rightLine = digitalRead(50);
  int doubleRightLine = digitalRead(53);
  int leftBackStop = digitalRead(38);
  int rightBackStop = digitalRead(40);
  int leftFrontStop = digitalRead(49);
  int rightFrontStop = digitalRead(51);

  // Serial.println(getDistance(2));

  if (heatLeft == LOW) {
    parkRight();
    parked = 1;
  }
  if (!(parked)) {
    if (heatLeft == LOW) {
      parkRight();
      parked = 1;
    } else if ((leftLine == 0 && middleLine == 0 && rightLine == 0) || (leftLine == 0 && middleLine == 1 && rightLine == 0)) {
      int distance = getDistance();
      if (distance <= 20) {
        moveCar(8, carSpeed);
        if (moveTo == 0) {
          carSpeed = 60;
          int start = millis();
          int current = millis();
          while (current - start <= 700) {
            moveCar(2, carSpeed);
            current = millis();
          }
          while (!(leftLine == HIGH || doubleLeftLine == HIGH)) {
            moveCar(0, 100);
            leftLine = digitalRead(46);
            doubleLeftLine = digitalRead(39);
          }
          moveCar(3, carSpeed);
          delay(400);
          moveTo = 1;
        } else if (moveTo == 1) {
          carSpeed = 60;
          int start = millis();
          int current = millis();
          while (current - start <= 700) {
            moveCar(3, carSpeed);
            current = millis();
          }
          while (!(rightLine == HIGH || doubleRightLine == HIGH)) {
            moveCar(0, 100);
            rightLine = digitalRead(50);
            doubleRightLine = digitalRead(53);
          }
          moveCar(2, carSpeed);
          delay(400);
          moveTo = 0;
        }
      } else if (distance <= 35 && distance > 20) {
        moveCar(0, 45);
      } else {
        moveCar(0, carSpeed);
      }

      moveCar(0, carSpeed);
    } else if (doubleLeftLine == HIGH && leftLine == 1) {
      while (doubleLeftLine == HIGH && leftLine == HIGH){
        moveCar(2, carSpeed+20);
        leftLine = digitalRead(46);
        doubleLeftLine = digitalRead(39);
      }
    } else if ((leftLine == 1 && middleLine == 0 && rightLine == 0) || (leftLine == 1 && middleLine == 1 && rightLine == 0)) {
      // Serial.println("GO LEFT UNTIL ONLY MIDDLE IS >= 100 ");
      // Move Left
      // moveCar(2, carSpeed+25);
      while (leftLine == 1){
        moveCar(2, carSpeed+25);
        leftLine = digitalRead(46);
      }

    } else if (doubleRightLine == HIGH && rightLine == 1) {
      while (doubleRightLine == HIGH && rightLine == 1){
        moveCar(3, carSpeed+20);
        rightLine = digitalRead(50);
        doubleRightLine = digitalRead(53);
      }
    } else if ((leftLine == 0 && middleLine == 0 && rightLine == 1) || (leftLine == 0 && middleLine == 1 && rightLine == 1)) {
      // Serial.println("GO RIGHT UNTIL ONLY MIDDLE IS >= 100 ");
      // moveCar(3, carSpeed+25);
      while (rightLine == 1){
        moveCar(3, carSpeed+25);
        rightLine = digitalRead(50);
      }

    } else if ((leftLine == 1 && middleLine == 1 && rightLine == 1) || (leftLine == 1 && middleLine == 0 && rightLine == 1)) {
      while (!(leftLine == 0 && rightLine == 0)) {
        moveCar(3, carSpeed);
        leftLine = digitalRead(46);
        rightLine = digitalRead(50);
      }
    }
  } else {
    moveCar(8, carSpeed);
  }
}