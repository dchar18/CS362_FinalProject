// Currently the only script that works...

int steering_trig = 5; // opens gate from power supply to steering motor
int input_1 = 6; // determines direction of steering motor rotation
int input_2 = 7; // determines direction of steering motor rotation

int drive_trig = 8; // opens gate from power supply to drive motor
int input_3 = 9; // determines direction of drive motor rotation
int input_4 = 10; // determines direction of drive motor rotation

String input; // used to receive input from app via bluetooth
char motor = ' '; // motor (1 = drive, 2 = steer)
char direct = ' '; // direction (1 = forward/left, 2 = back/right)

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
  if(Serial.available()){ // command was sent from app
    input = Serial.readString(); // receive the command, SLOWER THAN Serial.read();
    motor = input.charAt(0); // parse out first char to determine which motor
    direct = input.charAt(1); // parse out second char to determine direction

    if(motor == '1'){ // drive
      if(direct == '0'){ // stop driving
        stop_drive();
      }
      else if(direct == '1'){ // forward
        forward();
      }
      else if(direct == '2'){ // backward
        backward();
      }
    }
    else if(motor = '2'){ // steer
      if(direct == '0'){ // stop turning
        straight();
      }
      else if(direct == '1'){ // left
        turnLeft();
      }
      else if(direct == '2'){ // right
        turnRight();
      }
    }
  }
}

/*
    The following functions send HIGH and LOW signals to the corresponding gates
    in order to control motor rotation

    When trigger is HIGH, power from the power supply is sent to the motor
    Then, input_X pins determine which direction the motor spins

    For each function, change the input_X pins first before setting trigger to HIGH
    in order to prevent unwanted results. The opposite is done when setting trigger
    to LOW
*/
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
