// Use a photoresistor to change the color of the RGB LED

int r_red = 5;
int r_green = 6;
int r_blue = 7;
int l_red = 8;
int l_green = 9;
int l_blue = 10;

int led_tailL = 11;
int led_tailR = 12;

int photo_pin = A0;
int photo = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(r_red, OUTPUT);
  pinMode(r_green, OUTPUT);
  pinMode(r_blue, OUTPUT);
  pinMode(l_red, OUTPUT);
  pinMode(l_green, OUTPUT);
  pinMode(l_blue, OUTPUT);

  digitalWrite(r_red, LOW);
  digitalWrite(r_green, LOW);
  digitalWrite(r_blue, LOW);
  digitalWrite(l_red, LOW);
  digitalWrite(l_green, LOW);
  digitalWrite(l_blue, LOW);

  pinMode(photo_pin, INPUT);

  pinMode(led_tailL, OUTPUT);
  digitalWrite(led_tailL, HIGH);
  pinMode(led_tailR, OUTPUT);
  digitalWrite(led_tailR, HIGH);
}
void loop() {
  // put your main code here, to run repeatedly:
  photo = analogRead(photo_pin);
  if(photo < 400){
    RGB_color(255, 0, 0); // Red
  }
  else if(photo < 475){
    RGB_color(255, 255, 0); // Yellow
  }
  else if(photo < 550){
    RGB_color(255, 255, 125); // Raspberry
  }
  else if(photo < 625){
    RGB_color(255, 0, 255); // Magenta
  }
  else if(photo < 700){
    RGB_color(0, 255, 0); // Green
  }
  else if(photo < 775){
    RGB_color(0, 255, 255); // Cyan
  }
  else if(photo < 850){
    RGB_color(0, 0, 255); // Blue
  }
  else{
    RGB_color(255, 255, 255); // White
  }
  delay(1000);
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(l_red, red_light_value);
  analogWrite(r_red, red_light_value);
  analogWrite(l_green, green_light_value);
  analogWrite(r_green, green_light_value);
  analogWrite(l_blue, blue_light_value);
  analogWrite(r_blue, blue_light_value);
}
