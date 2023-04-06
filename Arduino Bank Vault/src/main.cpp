#include "main.h"

void setup() {
  Serial.begin(9600);
  pinMode(LED_R_PIN, OUTPUT);       // Red LED pin
  pinMode(LED_G_PIN, OUTPUT);       // Green LED pin
  pinMode(LED_B_PIN, OUTPUT);       // Blue LED pin
  doorServo.attach(SERVO_PIN);      // Servo pin
  pinMode(DOOR_PIN, INPUT_PULLUP);  // Button pin
  
  digitalWrite(LED_B_PIN, HIGH);    // Set LED to blue
  doorServo.write(0);               // Move servo to starting position
}

void loop() {
  byte buttonState = digitalRead(DOOR_PIN);
  
  if (buttonState == LOW) {
      Serial.println("Button is pressed");
      doorServo.write(180);
  }
  else {
      Serial.println("Button is not pressed");
      doorServo.write(0);
  }
  delay(100);
}