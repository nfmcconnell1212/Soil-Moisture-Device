//LIBRARIES
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//SCREEN DEFINITIONS
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//SOIL MOISTURE MODULE DEFINITIONS
#define sensorPower 7
#define sensorPin A3


int readSensor() {
  digitalWrite(sensorPower, HIGH);
  delay(10);
  int val = analogRead(sensorPin);
  digitalWrite(sensorPower, LOW);
  return val;
}

//Calibrate the sensor to determine the high value to create a percentage
int calibrateSensor() {
  int sensor_max = 0;
  
  for(int i = 0; i<50; i++){ 
    int val = analogRead(sensorPin);
    if(val > sensor_max) {
      sensor_max = val;
    }
    delay(50);
  }
  Serial.println("Done!");

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40,10);
  display.println("Done");
  display.display();
  delay(1000);


  Serial.print("Max wet value is ");
  Serial.println(sensor_max);
  return sensor_max;
}

void setup() {
  Serial.begin(9600);

  //Check Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  //Set display parameters
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  // Display static text
  display.println("Hello!");
  display.display(); 
  delay(2000);

  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);

  //Load Calibrating screen
  Serial.println("Calibrating...");
  display.clearDisplay();
  display.println("Calibrating...");
  display.display();
  int sensor_max = calibrateSensor(); //Max value is when it is dry
  int sensor_min = 100; //Min value in water...so really wet

}

void loop() {
  Serial.print("Analog output: ");
  Serial.println(readSensor());
  
  int sensor_val = analogRead(sensorPin);
  
  if(sensor_val <=130) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(45,10);
    display.println("Very Wet");
    drawProgressbar(0,20,120,10, sensor_val); // declare the graphics fillrect(int x, int y, int width, int height)
    display.setCursor(0, 0);
    display.println("Your soil is..");
    display.display();
    delay(1000);
  }

  if(sensor_val >130 && sensor_val < 230){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(45,10);
    display.println("Wet");
    drawProgressbar(0,20,120,10, sensor_val); // declare the graphics fillrect(int x, int y, int width, int height)
    display.setCursor(0, 0);
    display.println("Your soil is..");
    display.display();
    delay(1000);
  }

  if(sensor_val > 230 && sensor_val < 280){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(45,10);
    display.println("Moist");
    drawProgressbar(0,20,120,10, sensor_val); // declare the graphics fillrect(int x, int y, int width, int height)
    display.setCursor(0, 0);
    display.println("Your soil is..");
    display.display();
    delay(1000);    
  }

  if(sensor_val > 280 && sensor_val < 300){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20,10);
    display.println("in need of lil water");
    drawProgressbar(0,20,120,10, sensor_val); // declare the graphics fillrect(int x, int y, int width, int height)
    display.setCursor(0, 0);
    display.println("Your soil is..");
    display.display();
    delay(1000);   
  }

  if(sensor_val > 300){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40,10);
    display.println("very dry");
    drawProgressbar(0,20,120,10, sensor_val); // declare the graphics fillrect(int x, int y, int width, int height)
    display.setCursor(0, 0);
    display.println("Your soil is..");
    display.display();   
    delay(1000);  
  }

  delay(1000);
}

void drawProgressbar(int x,int y, int width,int height, int sensor_val)
{
  int val = map(sensor_val, 180, 350, 100, 0);
  Serial.print(val);
  //percentage = percentage > 100 ? 100 : percentage; // set the progress value to 100
  //percentage = percentage < 0 ? 0 :percentage; // start the counting to 0-100
  float bar = ((float)(width-1) / 100) * val;
  //Serial.println(bar);
  display.drawRect(x, y, width, height, WHITE);
  display.fillRect(x+2, y+2, bar , height-4, WHITE); // initailize the graphics fillRect(int x, int y, int width, int height)
}
