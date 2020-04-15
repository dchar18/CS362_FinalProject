#include <LiquidCrystal.h>

//LCD with pins
LiquidCrystal lcd(12, 11, 5, 4, 9, 8);  //can change pins here

//Button pin and keeping track of button info
int button = 13;  //pin
int buttonState = 0;
bool buttonPressed = false;

//Determines what to display on LCD
//0 is displaying distance of sensor closest to the wall
//1 is sensor 1 distance
//2 is sensor 2 distance
//3 is sensor 3 distance
//4 is sensor 4 distance
//5 is sensor 5 distance
//6 is sensor 6 distance
//7 is distance of all sensors in one LCD screen
//8 is error message given when a sensor is too close
//  to an object. This is not part of a mode but just
//  automatic
int mode = 0;
int prevMode = 0; //keeps track of last mode
bool isErrorMode = false; //tracks when mode 8 is entered for the first time

int speedThreshold = 60;

//Distance of each sensor
int distance[] = {0,0,0,0,0,0};

void setup() {
  // put your setup code here, to run once:
  lcd.begin(2,16);
  pinMode(button, INPUT);
  Serial.begin(9600); //Simulate getting input from sensor
}

//This function checks to see if any of the sensor
//is within the threshold.
bool isWithinThreshold(){
  if(distance[0] <= speedThreshold ||
    distance[1] <= speedThreshold ||
    distance[2] <= speedThreshold ||
    distance[3] <= speedThreshold ||
    distance[4] <= speedThreshold ||
    distance[5] <= speedThreshold)
    {
      return true;  
    }

    return false;
}

//This function parses the integers in the string
//and stores them in distance respectively
//This assumes that you inputted the distances correctly
//as this format(number with space in between. Semicolon at end): 
//  # # # # # #;
void parseDist(String dist){
  int sensorNum = 0;  //tracks what current sensor is
  String strToNum = ""; //string to add to eventually turn to an int

  for(int i = 0; i < dist.length(); i++){
    if(dist.charAt(i) == ' '){
      distance[sensorNum] = strToNum.toInt();
      sensorNum++;
      strToNum = "";
    }
    else{
      strToNum += dist.charAt(i);
    }
  }

  //Add last number in
  distance[sensorNum] = strToNum.toInt();
}

//Print LCD for mode 0
void mode0(){
    int closeSensor = -1;
    if(distance[0] <= speedThreshold)
      closeSensor = 1;
    else if(distance[1] <= speedThreshold)
      closeSensor = 2;
    else if(distance[2] <= speedThreshold)
      closeSensor = 3;
    else if(distance[3] <= speedThreshold)
      closeSensor = 4;
    else if(distance[4] <= speedThreshold)
      closeSensor = 5;
    else
      closeSensor = 6;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Closest Sensor");
    lcd.setCursor(0,1);
    lcd.print("Sensor " + String(closeSensor) + ":" + String(distance[closeSensor-1]));
}

//Print LCD for mode 1
void mode1(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensor 1");
    lcd.setCursor(0,1);
    lcd.print(String(distance[0]));
}

//Print LCD for mode 2
void mode2(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensor 2");
    lcd.setCursor(0,1);
    lcd.print(String(distance[1]));
}

//Print LCD for mode 3
void mode3(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensor 3");
    lcd.setCursor(0,1);
    lcd.print(String(distance[2]));
}

//Print LCD for mode 4
void mode4(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensor 4");
    lcd.setCursor(0,1);
    lcd.print(String(distance[3]));
}

//Print LCD for mode 5
void mode5(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensor 5");
    lcd.setCursor(0,1);
    lcd.print(String(distance[4]));
}

//Print LCD for mode 6
void mode6(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensor 6");
    lcd.setCursor(0,1);
    lcd.print(String(distance[5]));
}

//Print LCD for mode 7
void mode7(){
    String line1 = String(distance[0]) + " " + String(distance[1]) + " " + String(distance[2]);
    String line2 = String(distance[3]) + " " + String(distance[4]) + " " + String(distance[5]);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(line1);
    lcd.setCursor(0,1);
    lcd.print(line2);
}

void loop() {
  //Simulate obtaining values of sensor distances here.
  //Using serial input, input all 6 numbers with spaces
  //  in between and end with a ';'
  //  Ex. 25 50 100 125 74 300;
  //  Note: You must enter sensor data from 1-6 in order
  if(Serial.available()){
    //Take input until ; is read
    String s = Serial.readStringUntil(';');
    parseDist(s);
  }

  //Check to see if any sensors are within threshold range
  //so that LCD can warn user(mode 8)
  if(!isErrorMode && isWithinThreshold()){
    prevMode = mode;
    mode = 8;
    isErrorMode = true;
    lcd.clear();
    lcd.setCursor(0,0);

    //Checks what sensor is the closests sensor
    int closeSensor = -1;
    if(distance[0] <= speedThreshold)
      closeSensor = 1;
    else if(distance[1] <= speedThreshold)
      closeSensor = 2;
    else if(distance[2] <= speedThreshold)
      closeSensor = 3;
    else if(distance[3] <= speedThreshold)
      closeSensor = 4;
    else if(distance[4] <= speedThreshold)
      closeSensor = 5;
    else
      closeSensor = 6;

    //Prints warning for user in mode 8
    lcd.print("CAUTION"); 
    lcd.setCursor(0,1);
    lcd.print("Sensor " + String(closeSensor) + ":" + String(distance[closeSensor-1]));
  }

  //When user is still in mode 8 and distance to object gets closer or
  //another sensor gets past threshold, update LCD
  else if(isErrorMode && isWithinThreshold()){
    int closeSensor = -1;
    if(distance[0] <= speedThreshold)
      closeSensor = 1;
    else if(distance[1] <= speedThreshold)
      closeSensor = 2;
    else if(distance[2] <= speedThreshold)
      closeSensor = 3;
    else if(distance[3] <= speedThreshold)
      closeSensor = 4;
    else if(distance[4] <= speedThreshold)
      closeSensor = 5;
    else
      closeSensor = 6;

    lcd.clear();
    lcd.print("CAUTION"); 
    lcd.setCursor(0,1);
    lcd.print("Sensor " + String(closeSensor) + ":" + String(distance[closeSensor-1]));
  }

  //This checks to see if in errorMode and isn't within threshold
  else if(isErrorMode && !isWithinThreshold()){
    mode = prevMode;
    isErrorMode = false;
  }

  //If no sensors within threshold
  if(!isErrorMode){
    //Get read button to see if pressed
    buttonState = digitalRead(button);

    //If first time pressed, disable button
    if(buttonState == HIGH && buttonPressed == false){
      buttonPressed = true;
      if(mode == 7){
        mode = 0;
      }
      else
        mode++;
    }

    //User no longer pressing button so can reset button to be pressed again
    else if(buttonState == LOW && buttonPressed == true){
      buttonPressed = false;
    }

    //Prints stuff on LCD based on mode
    if(mode == 0){
      mode0();
    }
    else if(mode == 1){
      mode1();
    }
    else if(mode == 2){
      mode2();
    }
    else if(mode == 3){
      mode3();
    }
    else if(mode == 4){
      mode4();
    }
    else if(mode == 5){
      mode5();
    }
    else if(mode == 6){
      mode6();
    }
    else if(mode == 7){
      mode7();
    }
  }

  //Small delay
  delay(50);
}
