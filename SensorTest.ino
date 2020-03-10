// Back 3 sensors
const byte    trigPinBR = 13 ;  // Back Right trigger pin
const byte    echoPinBR = 12 ;  // Back Right echo pin
unsigned int distanceBR ;       // Distance for Back right Sensor

const byte    trigPinBM = 11 ;  // Back Middle trigger pin
const byte    echoPinBM = 10 ;  // Back Middle echo pin
unsigned int distanceBM ;       // Distance for Back Middle Sensor  

const byte    trigPinBL = 9 ;   // Back Left trigger pin
const byte    echoPinBL = 8 ;   // Back Left echo pin
unsigned int distanceBL ;       // Distance for Back Left Sensor


// Front 3 sensors
const byte    trigPinBR = 7 ;   // Front Right trigger pin
const byte    echoPinBR = 6 ;   // Front Right echo pin
unsigned int distanceBR ;       // Distance for Front right Sensor

const byte    trigPinBM = 5 ;   // Front Middle trigger pin
const byte    echoPinBM = 4 ;   // Front Middle echo pin
unsigned int distanceBM ;       // Distance for Front Middle Sensor  

const byte    trigPinBL = 3 ;   // Front Left trigger pin
const byte    echoPinBL = 2 ;   // Front Left echo pin
unsigned int distanceBL ;       // Distance for Front Left Sensor


// Funtion to get distance for Back Right Sensor
unsigned int pingBR() {
  digitalWrite(trigPinBR,HIGH) ; 
  delayMicroseconds(10);   
  digitalWrite(trigPinBR,LOW) ;
  return  ( pulseIn(echoPinBR,HIGH)/58)  ; // Formula for geting it into cm 
}

// Funtion to get distance for Back Middle Sensor
unsigned int pingBM() {
  digitalWrite(trigPinBM,HIGH) ; 
  delayMicroseconds(10);   
  digitalWrite(trigPinBM,LOW) ;
  return  ( pulseIn(echoPinBM,HIGH)/58)  ; // Formula for geting it into cm 
}

// Funtion to get distance for Back Left Sensor
unsigned int pingBL() {
  digitalWrite(trigPinBL,HIGH) ; 
  delayMicroseconds(10);   
  digitalWrite(trigPinBL,LOW) ;
  return  ( pulseIn(echoPinBL,HIGH)/58)  ;  // Formula for geting it into cm 
}

// Funtion to get distance for Front Right Sensor
unsigned int pingFR() {
  digitalWrite(trigPinFR,HIGH) ; 
  delayMicroseconds(10);   
  digitalWrite(trigPinFR,LOW) ;
  return  ( pulseIn(echoPinFR,HIGH)/58)  ; // Formula for geting it into cm 
}

// Funtion to get distance for Back Middle Sensor
unsigned int pingFM() {
  digitalWrite(trigPinFM,HIGH) ; 
  delayMicroseconds(10);   
  digitalWrite(trigPinFM,LOW) ;
  return  ( pulseIn(echoPinFM,HIGH)/58)  ; // Formula for geting it into cm 
}

// Funtion to get distance for Front Left Sensor
unsigned int pingFL() {
  digitalWrite(trigPinFL,HIGH) ; 
  delayMicroseconds(10);   
  digitalWrite(trigPinFL,LOW) ;
  return  ( pulseIn(echoPinFL,HIGH)/58)  ; // Formula for geting it into cm 
}
  
void setup() {
  // put your setup code here, to run once:
  
  // Back Right
  pinMode(trigPinBR,  OUTPUT) ;
  pinMode(echoPinBR , INPUT) ;

  // Back Middle
  pinMode(trigPinBM,  OUTPUT) ;
  pinMode(echoPinBM,  INPUT) ;

  // Back Left
  pinMode(trigPinBL,  OUTPUT) ;
  pinMode(echoPinBL , INPUT) ;

  // Front Right
  pinMode(trigPinFR,  OUTPUT) ;
  pinMode(echoPinFR , INPUT) ; 

  // Front Middle
  pinMode(trigPinFM,  OUTPUT) ;
  pinMode(echoPinFM , INPUT) ;

  // Front Left
  pinMode(trigPinFL,  OUTPUT) ;
  pinMode(echoPinFL , INPUT) ;
  
  Serial.begin(9600) ;
}

void loop() {
  // put your main code here, to run repeatedly:
    String frontString  = "";
    String backString   = "";
    
    distanceBR  = pingBR();
    distanceBM  = pingBM();
    distanceBL  = pingBL();
    
    distanceFR  = pingFR();
    distanceFM  = pingFM();
    distanceFL  = pingFL();
    
    frontString = "Front Right=" + String(distanceFR) + "cm , Front Middle=" + String(distanceFM) + " cm , Front Left=" + String(distanceFL);
    backString  = "Back Right=" + String(distanceBR) + "cm , Back Middle=" + String(distanceBM) + " cm , Back Left=" + String(distanceBL);
    
    Serial.println(frontString);
    Serial.println(backString);
    delay(300) ;
}
