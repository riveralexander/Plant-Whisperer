/* 
Created by River Alexander http://www.river-alexander.com
This project turns on different colors on an RGB Common Cathode LED and then the LCD shows what the plant is
"thinking" (ex. I need water.) It is actually a quite simple project. This is an unfinished project, I plan to
update with a photocell, humidity/temperature sensor, water pump, and Twitter API.

Connection pins:

Arduino - Soil Moisture Sensor YL-69 http://r.ebay.com/bzC8d8
  A0 --> Analog A0                            
  5V --> VCC
  GND --> GND
  
Arduino - Parallax 2 x 16 Serial LCD (Backlit) https://www.parallax.com/product/27977
  Pin 5 --> Rx
  5V --> 5V
  GND --> GND

Arduino - RGB Common Cathode LED
  Pin 2 --> Red
  GND --> GND
  Pin 3 --> Green
  Pin 4 --> Blue
*/

const int red = 2;
const int green = 3;
const int blue = 4;
const int rxPin = 5;

#include <SoftwareSerial.h>
SoftwareSerial LCD = SoftwareSerial(225, rxPin); //LCD is the Parallax 2 x 16 Serial LCD

void setup() {
  LCD.begin(9600);                  //turns on LCD
  delay(5);                         //required delay
  LCD.write(17);                    //turns on backlight
  pinMode(A0, INPUT);               //sets up analog pin 0 as input
  pinMode(red, OUTPUT);             // red led
  pinMode(green, OUTPUT);           // green led
  pinMode(blue, OUTPUT);            // blue led
  pinMode(rxPin, OUTPUT);           // Parallax 2 x 16 Serial LCD
  digitalWrite(rxPin, HIGH);
}

void loop()
{
  int moisture = analogRead(A0); //Plant soil sample

  //Sensor is not in soil or disconnected:
  if (moisture >= 1000) {
    LCD.write(12);                  //Clears LCD
    delay(5);
    LCD.print("Sensor is");         //First line
    LCD.write(13);                  //Goes to line 2
    LCD.print("disconnected");      //Second line
    LCD.write(212);                 //Quarter note
    LCD.write(220);                 //Tone
    delay(3000);
    digitalWrite(red, HIGH);        //Turns on red LED
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);       //Turns on blue LED (combines to make purple)
  }
  
  //Plant needs water:
  if (moisture < 1000 && moisture >= 600) {
    LCD.write(12);                  //Clears LCD
    delay(5);
    LCD.print("Plant: I'm");        //First line
    LCD.write(13);                  //Goes to line 2
    LCD.print("thirsty.");          //Second line
    digitalWrite(red, HIGH);        //Turns on red LED
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
  }
  
  //Plant is watered:
  if (moisture < 600 && moisture >= 370) {
    LCD.write(12);                  //Clears LCD
    delay(5);
    LCD.print("Plant: I'm good.");  //First line
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);      //Turns on green LED
    digitalWrite(blue, LOW);
  }
  
  //Plant is overwatered:
  if (moisture < 370) {
    LCD.write(12);                  //Clears LCD
    delay(5);
    LCD.print("Plant: AHHHHH!");    //First line
    LCD.write(13);                  //Goes to line 2
    LCD.print("I'm drowning!!");    //Second line
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);       //Turns on blue LED
  }
  
  delay(50);
}
