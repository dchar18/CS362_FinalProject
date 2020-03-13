// This version takes the MappedTest_led code and substitutes the LEDs
// with the motors
int steering_trig = 5; // opens gate from power supply to steering motor
int input_1 = 6; // determines direction of steering motor rotation
int input_2 = 7; // determines direction of steering motor rotation

int drive_trig = 8; // opens gate from power supply to drive motor
int input_3 = 9; // determines direction of drive motor rotation
int input_4 = 10; // determines direction of drive motor rotation

char input = ' ';
int input_int = 0;
int curr_state = -1;

void setup() {
  Serial.begin(9600); // start communication with Bluetooth module

  // initialize pins used to control L293D IC gates
  // steering pin setup
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
}

void loop() {
  if(Serial.available()){
    input = Serial.read();
    input_int = input - '0';
    
    if(input == '0'){ // stop
      stop_drive();
      straight();
    }
    else if(input_int % 2 == 1){ // use single motor
      if(input == '1'){ // forward
        straight(); // turn off steering
        forward();
      }
      else if(input == '5'){ // backward
        straight();
        backward();
      }
      else if(input == '3'){ // right
        stop_drive(); // turn off drive
        turnRight();
      }
      else if(input == '7'){ // left
        stop_drive(); // turn off drive
        turnLeft();
      }
    }
    else{ // use two motors
      if(input == '2'){ // forward right
        turnRight();
        forward();
      }
      else if(input == '8'){ // forward left
        turnLeft();
        forward();
      }
      else if(input == '4'){ // backward right
        turnRight();
        backward();
      }
      else if(input == '6'){ // backward left
        turnLeft();
        backward();
      }
    }
  }// end of Serial.available()
}

void turnLeft(){
  digitalWrite(input_1, HIGH);
  digitalWrite(input_2, LOW);
  digitalWrite(steering_trig, HIGH);
}

void turnRight(){
  digitalWrite(input_1, LOW);
  digitalWrite(input_2, HIGH);
  digitalWrite(steering_trig, HIGH);
}

void straight(){
  digitalWrite(steering_trig, LOW);
  digitalWrite(input_1, LOW);
  digitalWrite(input_2, LOW);
}

void forward(){ // May need to revise
  digitalWrite(input_3, HIGH);
  digitalWrite(input_4, LOW);
  digitalWrite(drive_trig, HIGH);
}

void backward(){ // May need to revise
  digitalWrite(input_3, LOW);
  digitalWrite(input_4, HIGH);
  digitalWrite(drive_trig, HIGH);
}

void stop_drive(){
  digitalWrite(drive_trig, LOW);
  digitalWrite(input_3, LOW);
  digitalWrite(input_4, LOW);
}
