#pragma once
#include <string>
#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>

using namespace std;

// Pins
const int LED_R_PIN = 5;
const int LED_G_PIN = 4;
const int LED_B_PIN = 3;
const int SERVO_PIN = 2;
const int DOOR_PIN = 1;

// Servo
Servo doorServo;