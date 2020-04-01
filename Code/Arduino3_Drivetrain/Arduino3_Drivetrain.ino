/*
  Collision Prevention System - Arduino 3 (Drivetrain)

  Outline:
  1. Receive data from Ar1
  2. Receive data from Ar2
  3. If sensor data below threshold, override app controls
*/

#include <Wire.h>

int a = 12;

// used for driving
int steering_trig = 5; // opens gate from power supply to steering motor
int input_1 = 6; // determines direction of steering motor rotation
int input_2 = 7; // determines direction of steering motor rotation

int drive_trig = 8; // opens gate from power supply to drive motor
int input_3 = 9; // determines direction of drive motor rotation
int input_4 = 10; // determines direction of drive motor rotation

// used for controlling motors
char input = '0';
int input_int = 0;

// used for determining whether sensors should be active or not
bool sensors_enabled = true;

// used to receive data over I2C
char first_input = ' ';
int averages[] = {0, 0, 0, 0, 0, 0}; // used to receive data from Ar2

// used to determine if certain areas around the car are clear
bool front_clear = true;
bool fl_clear = true;
bool fr_clear = true;
bool back_clear = true;
bool bl_clear = true;
bool br_clear = true;

bool front_all_clear = true;
bool back_all_clear = true;


void setup() {
  Wire.begin(); // Wire.begin(3);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);

  pinMode(steering_trig, OUTPUT);
  digitalWrite(steering_trig, LOW);
  pinMode(input_1, OUTPUT);
  pinMode(input_2, OUTPUT);
  digitalWrite(input_1, LOW);
  digitalWrite(input_2, LOW);

  // drivetrain pin setup
  pinMode(drive_trig, OUTPUT);
  digitalWrite(drive_trig, LOW);
  pinMode(input_3, OUTPUT);
  pinMode(input_4, OUTPUT);
  digitalWrite(input_3, LOW);
  digitalWrite(input_4, LOW);

  pinMode(a, OUTPUT);
  digitalWrite(a, LOW);
}

void loop() {
  Wire.requestFrom(1, 1); // request 1 byte from Arduino1 (bluetooth)

  while(Wire.available()){
    Serial.println("Receiving something...");
  }

  if(sensors_enabled){
    Wire.requestFrom(2, 6); // request 6 bytes from Arduino2 (senors)
  }
  
  

  if (!front_all_clear || !back_all_clear) {
    stop_drive();
  }

  if (input == '0') { // stop
    stop_drive();
    straight();
  }
  else if (input == 'A') { // turn on sensors
    sensors_enabled = true;
  }
  else if (input == 'B') { // turn off sensors
    sensors_enabled = false;
  }
  else if (input_int % 2 == 1) { // use single motor
    if (input == '1') { // forward
      straight(); // turn off steering
      if (sensors_enabled && front_clear) {
        forward();
      }
    }
    else if (input == '5') { // backward
      straight();
      if (sensors_enabled && back_clear) {
        backward();
      }
    }
    else if (input == '3') { // right
      stop_drive(); // turn off drive
      turnRight();
    }
    else if (input == '7') { // left
      stop_drive(); // turn off drive
      turnLeft();
    }
  }
  else { // use two motors
    if (input == '2') { // forward right
      if (sensors_enabled && fr_clear) {
        turnRight();
        forward();
      }
    }
    else if (input == '8') { // forward left
      if (sensors_enabled && fl_clear) {
        turnLeft();
        forward();
      }
    }
    else if (input == '4') { // backward right
      if (sensors_enabled && br_clear) {
        turnRight();
        backward();
      }
    }
    else if (input == '6') { // backward left
      if (sensors_enabled && bl_clear) {
        turnLeft();
        backward();
      }
    }
  }
}

void receiveEvent(int numbytes) {
  digitalWrite(a, HIGH);
  Serial.println(numbytes);

  input = Wire.read();
  Serial.print(input);

/*
  if (numbytes == 0x01) { // motor controls came in 
    input = Wire.read();
  }
  else { // sensor data came in
    for (int i = 0; i < 6; i++) {
      averages[i] = Wire.read() - '0'; // cast back to int
      Serial.print(" ");
      Serial.print(averages[i]);
    }
    Serial.println();

    // front left sensor
    if (averages[0] <= 15) {
      fl_clear = false;
    }
    else {
      fl_clear = true;
    }

    // front middle sensor
    if (averages[1] <= 40) {
      front_clear = false;
    }
    else {
      front_clear = true;
    }

    // front right sensor
    if (averages[2] <= 15) {
      fr_clear = false;
    }
    else {
      fr_clear = true;
    }

    // back right sensor
    if (averages[3] <= 15) {
      br_clear = false;
    }
    else {
      br_clear = true;
    }

    // back middle sensor
    if (averages[4] <= 40) {
      back_clear = false;
    }
    else {
      back_clear = true;
    }

    // back left sensor
    if (averages[5] <= 15) {
      bl_clear = false;
    }
    else {
      bl_clear = true;
    }

    front_all_clear = fl_clear && front_clear && fr_clear;
    back_all_clear = bl_clear && back_clear && br_clear;
  }
*/


  //  first_input = Wire.read();
  //  Serial.println(first_input);
  //  digitalWrite(a, HIGH);
  //  if (first_input == 'S') {
  //    if (sensors_enabled) {
  //      for (int i = 0; i < 6; i++) {
  //        averages[i] = Wire.read() - '0'; // cast back to int
  //        Serial.print(" ");
  //        Serial.print(averages[i]);
  //      }
  //      Serial.println();
  //
  //      // front left sensor
  //      if (averages[0] <= 15) {
  //        fl_clear = false;
  //      }
  //      else {
  //        fl_clear = true;
  //      }
  //
  //      // front middle sensor
  //      if (averages[1] <= 40) {
  //        front_clear = false;
  //      }
  //      else {
  //        front_clear = true;
  //      }
  //
  //      // front right sensor
  //      if (averages[2] <= 15) {
  //        fr_clear = false;
  //      }
  //      else {
  //        fr_clear = true;
  //      }
  //
  //      // back right sensor
  //      if (averages[3] <= 15) {
  //        br_clear = false;
  //      }
  //      else {
  //        br_clear = true;
  //      }
  //
  //      // back middle sensor
  //      if (averages[4] <= 40) {
  //        back_clear = false;
  //      }
  //      else {
  //        back_clear = true;
  //      }
  //
  //      // back left sensor
  //      if (averages[5] <= 15) {
  //        bl_clear = false;
  //      }
  //      else {
  //        bl_clear = true;
  //      }
  //
  //      front_all_clear = fl_clear && front_clear && fr_clear;
  //      back_all_clear = bl_clear && back_clear && br_clear;
  //    }
  //  }
  //  else { // input is for drivetrain controls
  //
  //  }
}

void turnLeft() {
  digitalWrite(input_1, HIGH);
  digitalWrite(input_2, LOW);
  digitalWrite(steering_trig, HIGH);
}

void turnRight() {
  digitalWrite(input_1, LOW);
  digitalWrite(input_2, HIGH);
  digitalWrite(steering_trig, HIGH);
}

void straight() {
  digitalWrite(steering_trig, LOW);
  digitalWrite(input_1, LOW);
  digitalWrite(input_2, LOW);
}

void forward() { // May need to revise
  digitalWrite(input_3, HIGH);
  digitalWrite(input_4, LOW);
  digitalWrite(drive_trig, HIGH);
}

void backward() { // May need to revise
  digitalWrite(input_3, LOW);
  digitalWrite(input_4, HIGH);
  digitalWrite(drive_trig, HIGH);
}

void stop_drive() {
  digitalWrite(drive_trig, LOW);
  digitalWrite(input_3, LOW);
  digitalWrite(input_4, LOW);
}
