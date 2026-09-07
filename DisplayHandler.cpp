#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int connection = 0; //0 for connecting, 1 for connected, 2 for failed connection

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void displaySetup() {
  // Address 0x3C is typically the default for 128x64 OLEDs
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 address allocation failed"));
  }
}

void displayConnection(int connected){
  connection = connected; 
}

void displayLoop(){
  // Clear the buffer
  display.clearDisplay();

  // Set text size and color
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.println(F("Aquarium Controller\n"));

  switch(connection){
    case 0:
      display.println(F("Connecting....."));
      break;
    case 1: 
      display.println(F("Connection\nMade Sucessfully"));
      break;
    case 2:
      display.println(F("Connection Failed"));
      break;
    default:
      display.println(F(""));
  }
  
  // Set cursor and print text
  display.setCursor(0,0);
  display.display();
}