// Function: handles LEDs and bluetooth communication
//           sends the input received via bluetooth to drivetrain Arduino (master)

/*
Collision Prevention System - Arduino 1 (Bluetooth)

Outline:
1. Receive input from app
2. Send command to Ar3
*/

#include <Wire.h>

// underglow LEDs
int u_fl = 2;
int u_fr = 3;
int u_bl = 4;
int u_br = 5;

// headlight RGB LEDs
int r_red = 6;
int r_green = 7;
int r_blue = 8;
int l_red = 9;
int l_green = 10;
int l_blue = 11;

// taillight LEDs
int led_tailL = 12;
int led_tailR = 13;

char input = ' ';
char car_command = '0';

void setup(){
  Serial.begin(9600); // start communication with Bluetooth module
  Wire.begin(1);
  Wire.onRequest(requestEvent);

  pinMode(r_red, OUTPUT);
  pinMode(r_green, OUTPUT);
  pinMode(r_blue, OUTPUT);
  pinMode(l_red, OUTPUT);
  pinMode(l_green, OUTPUT);
  pinMode(l_blue, OUTPUT);
  
  green();

  pinMode(led_tailL, OUTPUT);
  digitalWrite(led_tailL, HIGH);
  pinMode(led_tailR, OUTPUT);
  digitalWrite(led_tailR, HIGH);

  pinMode(u_fl, OUTPUT);
  digitalWrite(u_fl, HIGH);
  pinMode(u_fr, OUTPUT);
  digitalWrite(u_fr, HIGH);
  pinMode(u_bl, OUTPUT);
  digitalWrite(u_bl, HIGH);
  pinMode(u_br, OUTPUT);
  digitalWrite(u_br, HIGH);
}

void loop(){
  if(Serial.available()){
    input = Serial.read();
    Serial.println(input);

    if(input == 'Z'){
      Serial.println("Magenta");
      magenta();
    }
    else if(input == 'Y'){
      Serial.println("Red");
      red();
    }
    else if(input == 'X'){
      Serial.println("Green");
      green();
    }
    else if(input == 'W'){
      Serial.println("Cyan");
      cyan();
    }
    else if(input == 'V'){
      Serial.println("Blue");
      blue();
    }
    else{
      Serial.print("Sending to drivetrain...");
      car_command = input;
      Wire.beginTransmission(3);
      Wire.write(car_command); // send a char
      int result = Wire.endTransmission();
      if(result == 0){
        Serial.println("Sent!");
      }
      else{
        Serial.println(result);
      }
    }
  }
}

void requestEvent(){
  Serial.println("Ard3 is requesting...");
  Wire.beginTransmission(3);
  Wire.write(car_command); // send a char
  Wire.endTransmission();
}

void magenta(){
  analogWrite(l_red, 255);
  analogWrite(r_red, 255);
  analogWrite(l_green, 0);
  analogWrite(r_green, 0);
  analogWrite(l_blue, 255);
  analogWrite(r_blue, 255);
}

void red(){
  analogWrite(l_red, 255);
  analogWrite(r_red, 255);
  analogWrite(l_green, 0);
  analogWrite(r_green, 0);
  analogWrite(l_blue, 0);
  analogWrite(r_blue, 0);
}

void green(){
  analogWrite(l_red, 0);
  analogWrite(r_red, 0);
  analogWrite(l_green, 255);
  analogWrite(r_green, 255);
  analogWrite(l_blue, 0);
  analogWrite(r_blue, 0);
}

void cyan(){
  analogWrite(l_red, 0);
  analogWrite(r_red, 0);
  analogWrite(l_green, 255);
  analogWrite(r_green, 255);
  analogWrite(l_blue, 255);
  analogWrite(r_blue, 255);
}

void blue(){
  analogWrite(l_red, 0);
  analogWrite(r_red, 0);
  analogWrite(l_green, 0);
  analogWrite(r_green, 0);
  analogWrite(l_blue, 255);
  analogWrite(r_blue, 255);
}
