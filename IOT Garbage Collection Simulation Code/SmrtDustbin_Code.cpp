// Copyright (c) 2025 Kemar Christie
// All rights reserved. Unauthorized use, copying, or distribution is prohibited.
// Contact kemar.christie@yahoo.com for licensing inquiries.
// Authors: Kemar Christie


#include <LiquidCrystal.h>
#include <Servo.h>

const int pirSensorPin = 3;         // Connect PIR sensor to digital pin 2
const int ultrasonicTrigPin = 7;   // Connect ultrasonic sensor trig pin to digital pin 3
const int ultrasonicEchoPin = 8;  // Connect ultrasonic sensor echo pin to digital pin 4
const int servoPin = 9;          // Connect servo motor signal pin to digital pin 9

LiquidCrystal lcd( A0, A1, A2, A3, A4, A5); 
Servo lidServo;

const int maxGarbageCapacity = 100; // Adjust this value based on your dustbin capacity
int currentGarbageLevel = 0;
bool isLidOpen = false;

void setup() {
  pinMode(pirSensorPin, INPUT);
  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);

  lcd.begin(16, 2);
  
  lcd.setCursor(0, 0);
  lcd.print(" Smart Dustbin ");
  lcd.setCursor(0, 1);
  lcd.print("Waste Monitoring");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MJPRJ MP18");
  delay(3000);
  lcd.clear();

  lidServo.attach(servoPin);
  closeLid();
}

void loop() {
  int pirSensorValue = digitalRead(pirSensorPin);
  int ultrasonicDistance = getUltrasonicDistance();

  if (pirSensorValue == HIGH && !isLidOpen) {
   openLid();
    delay(5000);  // Open the lid if the IR sensor detects an object (garbage) and the lid is closed
       
  }

  if (pirSensorValue == LOW && isLidOpen) {
    
    closeLid();
              // Close the lid if the IR sensor doesn't detect any object (garbage) and the lid is open
 
  }

  // Update garbage level when ultrasonic sensor detects an object (garbage)
  if (ultrasonicDistance < 200) {          // Adjust this value based on your setup
    currentGarbageLevel = map(ultrasonicDistance, 200,0, 0, maxGarbageCapacity);
  }

  // Display garbage level on the LCD
  int percentage = map(currentGarbageLevel, 0, maxGarbageCapacity, 0, 100);
  lcd.setCursor(0, 0);
  lcd.print("Garbage Level:");
  lcd.setCursor(6, 1);
  lcd.print(percentage);
  lcd.setCursor(8, 1);
  lcd.print("%");

  delay(100);     // Add a small delay to avoid rapid updates
}

  int getUltrasonicDistance() {
  digitalWrite(ultrasonicTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrigPin, LOW);

  long duration = pulseIn(ultrasonicEchoPin, HIGH);
  int distance = duration * 0.034 / 2; // Calculate distance in cm

  return distance;
}

void openLid() {
  lidServo.write(90);    // Adjust the angle to open the lid as required
  isLidOpen = true;
}

void closeLid() {
  lidServo.write(0);    // Adjust the angle to close the lid as required
  isLidOpen = false;
}