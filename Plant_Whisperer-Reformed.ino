//The Plant Whisperer
/*
Created by River Alexander http://www.twitter.com/riveralexander
This project shows what your plant is "thinking" (ex. "I need water" or "I need more sunlight") then prints it to an LCD screen. 
This project uses:
(1)RGB Common Cathode LED
(1)Parallax 16x2 LCD Screen
(1)Moisture Sensor
(1)Photocell
(1)Humidity/Temperature Sensor
(1)Water pump
(1)Button
(1)Arduino MEGA or Uno
*/

const int waterPumpPin = 7;
const int buttonPin = 6;
const int lcdPin = 5;
const int redPin = 4, red = 4;
const int greenPin = 3, green = 3;
const int bluePin = 2, blue = 2;
const int moistureSensorPin = A1;
const float tempSensorPin = A2;
const float lightSensorPin = A3;

#include <SoftwareSerial.h>
SoftwareSerial LCD = SoftwareSerial(255, lcdPin);
#include <JeeLib.h>
ISR(WDT_vect)
{
	Sleepy::watchdogEvent();
}

int buttonState = 0;

void setup()
{
	pinMode(moistureSensorPin, INPUT);
	pinMode(tempSensorPin, INPUT);
	pinMode(lightSensorPin, INPUT);
  	pinMode(buttonPin, INPUT);
  	pinMode(waterPumpPin, OUTPUT);
	pinMode(lcdPin, OUTPUT);
	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);
	pinMode(lcdPin, HIGH);
	LCD.begin(9600);
	delay(5);
	LCD.write(17);
}

void loop()
{
	int moistureRead = analogRead(moistureSensorPin);
	float lightRead = analogRead(lightSensorPin);
 	buttonState = digitalRead(buttonPin);
	
	float voltage = analogRead(tempSensorPin) * 0.004882814;
	float degreesC = (voltage - 0.5) * 100.0;
	float degreesF = degreesC * (9.0/5.0) + 32.0;

	if (buttonState == HIGH)
	{
		digitalWrite(waterPumpPin, HIGH);
	}
	else
	{
		digitalWrite(waterPumpPin, LOW);
	}
	
	if (moistureRead > 999)
	{
		moistureSensorDisconnected();
	}
	else if (moistureRead < 1000 && moistureRead > 599)
	{
		needWater();
	}
	else if (moistureRead < 600 && moistureRead > 369)
	{
		alreadyWatered();
	}
	else 
	{
		overWatered();
	}
	
	if (degreesF < 32.1)
	{
		Sleepy::loseSomeTime(5000);
		needWarmth();
	}
	
	if (lightRead < 949.9)
	{
		Sleepy::loseSomeTime(5000);
		needSunlight();
	}
	
	Sleepy::loseSomeTime(10000);
}	

void setColor(int red, int green, int blue) 
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

void clearLCD()
{
	LCD.write(12);
	delay(5);
}

void moistureSensorDisconnected()
{
	clearLCD();
	LCD.print("Sensor is") + LCD.write(13) + ("disconnected");
	LCD.write(212);
	LCD.write(220);
	digitalWrite(redPin, HIGH);
	delay(1000);
	digitalWrite(redPin, LOW);
}

void needWater()
{
	clearLCD();
	LCD.print("Plant: I'm") + LCD.write(13) + ("thirsty.");
	setColor(255, 0, 0);
}

void alreadyWatered()
{
	clearLCD();
	LCD.print("Plant: I'm good.");
	setColor(0, 255, 0);
}

void overWatered()
{
	clearLCD();
	LCD.print("Plant: AHHH!") + LCD.write(13) + ("I'm drowning!!");
	setColor(0, 0, 255);
}

float getyVoltage(int pin)
{
	return (analogRead(tempSensorPin) * 0.004882814);
}

void needWarmth()
{
	clearLCD();
	LCD.print("Plant: I'm too") + LCD.write(13) + ("cold.");
	setColor(80, 0, 80);
}

void needSunlight()
{
	clearLCD();
	LCD.print("Plant: I need") + LCD.write(13) + ("more sunlight.");
	setColor(255, 255, 0);
}
	
