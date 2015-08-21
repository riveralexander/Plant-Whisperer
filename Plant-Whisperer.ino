/* 
Created by River Alexander http://www.river-alexander.com
This project turns on different colors on an RGB Common Cathode LED and then the LCD shows what the plant is
"thinking" (ex. I need water.) It is actually a quite simple project. This is an unfinished project, I plan to
update with a temperature sensor, and Twitter API (which will replace the push button).

Connection pins:

Arduino - Soil Moisture Sensor YL-69 http://r.ebay.com/bzC8d8
  A0 --> Analog A0                            
  5V --> VCC
  GND --> GND
  
Arduino - Parallax 2 x 16 Serial LCD (Backlit) https://www.parallax.com/product/27977
  Pin 5 --> Rx
  5V --> 5V
  GND --> GND
  
Arduino - Water Pump
  Pin 7 --> 3V or 5V
  GND --> GND
  
Arduino - Button
Pin 6 --> one leg --> resistor --> GND
5V --> another leg

Arduino - Photocell
  3-5V --> one end
  A1 --> 10kΩ resistor --> another end

Arduino - RGB Common Cathode LED
  Pin 2 --> Red
  GND --> GND
  Pin 3 --> Green
  Pin 4 --> Blue
*/

const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;
const int rxPin = 5;
const int buttonPin = 6;
const int waterPin = 7;

#include <SoftwareSerial.h>         //include library containing Parallax 2 x 16 Serial LCD functions
SoftwareSerial LCD = SoftwareSerial(225, rxPin); //LCD is the Parallax 2 x 16 Serial LCD

#include <JeeLib.h>                 //include library containing low power functions
ISR(WDT_vect) {
Sleepy::watchdogEvent();            //setup for low power waiting
}

void setup() {
  LCD.begin(9600);                  //turns on LCD
  delay(5);                         //required delay
  LCD.write(17);                    //turns on backlight
  pinMode(A0, INPUT);               //sets up analog pin 0 as input
  pinMode(A1, INPUT);
  pinMode(buttonPin, INPUT);        //button
  pinMode(redPin, OUTPUT);          // red led
  pinMode(greenPin, OUTPUT);        // green led
  pinMode(bluePin, OUTPUT);         // blue led
  pinMode(rxPin, OUTPUT);           // Parallax 2 x 16 Serial LCD
  pinMode(buttonPin, OUTPUT);       //Button
  pinMode(waterPin, OUTPUT);        //water pump
  digitalWrite(rxPin, HIGH);
}

void loop()
{
  int moisture = analogRead(A0);    //plant soil moisture sample
  int light = analogRead(A1);       //plant light sample
  
  //if pressed, turn on water pump
  if (digitalRead(buttonPin) == HIGH) {
    digitalWrite(waterPin, HIGH);
  }
  
  //if not pressed, keep water pump off
  else {
    digitalWrite(waterPin, LOW);
  }

  //Sensor is not in soil or disconnected:
  if (moisture >= 1000) {
    LCD.write(12);                  //clears LCD
    delay(5);
    LCD.print("Sensor is");         //first line
    LCD.write(13);                  //goes to line 2
    LCD.print("disconnected");      //second line
    LCD.write(212);                 //quarter note
    LCD.write(220);                 //tone
    delay(3000);
    setColor(255, 0, 255);          //turns on purple
  }
  
  //Plant needs water:
  if (moisture < 1000 && moisture >= 600) {
    LCD.write(12);                  //clears LCD
    delay(5);
    LCD.print("Plant: I'm");        //first line
    LCD.write(13);                  //goes to line 2
    LCD.print("thirsty.");          //second line
    setColor(255, 0, 0);            //turns on red
  }
  
  //Plant is watered:
  if (moisture < 600 && moisture >= 370) {
    LCD.write(12);                  //clears LCD
    delay(5);
    LCD.print("Plant: I'm good.");  //first line
  /*LCD.write(13);                  //goes to line 2
    LCD.print("");                  //second line*/
    setColor(0, 255, 0);            //turns on green
  }
  
  //Plant is overwatered:
  if (moisture < 370) {
    LCD.write(12);                  //clears LCD
    delay(5);
    LCD.print("Plant: AHHHHH!");    //first line
    LCD.write(13);                  //goes to line 2
    LCD.print("I'm drowning!!");    //second line
    setColor(0, 0, 255);            //turns on blue
  }
  
  Sleepy::loseSomeTime(5000);      //replaces delay and saves power
  
  if (light < 949) {
    LCD.write(12);                  //clears LCD
    delay(5);
    LCD.print("Plant: I need");     //first line
    LCD.write(13);                  //goes to second line
    LCD.print("more sunlight");     //second line
    setColor(255, 255, 0);          //turns on yellow
  }
  
  else {
    LCD.write(12);                  //clears LCD
    delay(5);
    LCD.print("Plant: I have");     //first line
    LCD.write(13);                  //goes to second line
    LCD.print("plenty of light");   //second line
  }
  
  Sleepy::loseSomeTime(5000);       //replaces delay and saves power
}

void setColor(int red, int green, int blue) {
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
