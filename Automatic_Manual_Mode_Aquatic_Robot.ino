#include <SoftwareSerial.h>
#include <Servo.h>
#include <NewPing.h> 

const int  lft_mtr1=3, lft_mtr2=4, rht_mtr1=5, rht_mtr2=6;
#define BT_RX 10
#define BT_TX 11

#define TRIGGER_PIN 9
#define ECHO_PIN 8

#define SERVO_PIN 7

SoftwareSerial bluetooth(BT_RX, BT_TX);

long duration;
int distance;

#define maximum_distance 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, maximum_distance);//M
Servo servo;
char command;

void setup() {
  bluetooth.begin(9600);
  servo.attach(SERVO_PIN);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servo.write(115);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  //servo.write(90);
  
  Serial.begin(9600);
}

void loop() {

  int distanceRight = 0
  int distanceLeft = 0;
  delay(50);
  
  // Check if data is available from Bluetooth module
  if (bluetooth.available()) {
    command = bluetooth.read();
    executeCommand(command);
  }

  // Check if in autonomous mode
  if (command == 'A') {
    // Get distance from ultrasonic sensor
    distance = getUltrasonicDistance();

    // If obstacle detected within 19cm, turn right
    if (distance < 19) {
      stopMoving();
      delay(300);

      
      moveBackward();
      delay(400);
      stopMoving();
      delay(300);
      distanceRight = lookRight();
      delay(300);
      distanceLeft = lookLeft();
      delay(300);

      if (distance >= distanceLeft){
        turnRight();
        stopMoving();
      }

     else {//m
      turnLeft();
      stopMoving();
    }
  }
  else{
    moveForward();
  }
  distance = readPing();
}
}//m


int lookRight(){ 
  servo.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  servo.write(115);
  return distance;
}


int lookLeft(){
  servo.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  servo.write(115);
  return distance;
  delay(100);
}


int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

void executeCommand(char cmd) {
  switch (cmd) {
    case 'F':
      moveForward();
      break;
    case 'B':
      moveBackward();
      break;
    case 'L':
      turnLeft();
      break;
    case 'R':
      turnRight();
      break;
    case 'M':
      stopMoving();
      break;
  }
}

void moveForward() {
  Serial.println("Moving forward");
  digitalWrite(lft_mtr1,HIGH);
  digitalWrite(lft_mtr2,LOW);
  digitalWrite(rht_mtr1,HIGH);
  digitalWrite(rht_mtr2,LOW);
}

void moveBackward() {
  Serial.println("Moving backward");
  digitalWrite(lft_mtr1,LOW);
  digitalWrite(lft_mtr2,HIGH);
  digitalWrite(rht_mtr1,LOW);
  digitalWrite(rht_mtr2,HIGH);
}

void turnLeft() {
  Serial.println("Turning left");
  digitalWrite(lft_mtr1,LOW);
  digitalWrite(lft_mtr2,HIGH);
  digitalWrite(rht_mtr1,HIGH);
  digitalWrite(rht_mtr2,LOW);
}

void turnRight() {
  Serial.println("Turning right");
  digitalWrite(lft_mtr1,HIGH);
  digitalWrite(lft_mtr2,LOW);
  digitalWrite(rht_mtr1,LOW);
  digitalWrite(rht_mtr2,HIGH);
}

void stopMoving() {
  Serial.println("Stopping");
  digitalWrite(lft_mtr1,LOW);
  digitalWrite(lft_mtr2,LOW);
  digitalWrite(rht_mtr1,LOW);
  digitalWrite(rht_mtr2,LOW);
}

int getUltrasonicDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}
