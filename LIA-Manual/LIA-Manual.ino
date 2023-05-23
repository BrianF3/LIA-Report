/*
   Manual Control
   
   This code allows manual control of an actuator using user input from an analog pin.
   The input value is mapped to a control range and applied as the actuator's control signal.
   The sensor value is read and displayed on an LCD along with the input value and control signal.
   The input value and control signal are also printed to the serial monitor.
*/

#include <LiquidCrystal.h>

// LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Analog input pin for the sensor
int Sensor = A1;

// Pin for the actuator
int Actuator = 10;

// Process variable (sensor value)
int PV = 0;

// Input value from user
int InputValue = 0;

// Analog input pin for user input
int inputpin = A0;

// Control signal
int C0 = 0;

void setup() {
  Serial.begin(9600); // Initializing serial communication
  pinMode(Actuator, OUTPUT); // Defining actuator pin as output
  lcd.begin(16, 2); // Set up the LCD's number of columns and rows
}

void loop() {
  // Read sensor value
  PV = analogRead(Sensor);

  // Read input value from user and map it to the control range
  InputValue = analogRead(inputpin);
  C0 = map(InputValue, 0, 1023, 0, 255);

  // Set actuator control signal
  analogWrite(Actuator, C0);

  // Print input, control signal, and process variable values to the serial monitor
  Serial.print("Input= ");
  Serial.print(InputValue);
  Serial.print("    C0=    ");
  Serial.print(C0);
  Serial.print("    PV=    ");
  Serial.print(PV);
  Serial.println("      ");

  // Print input value and control signal to the LCD display
  lcd.print("Inp=");
  lcd.print(InputValue);
  lcd.print("  C0=");
  lcd.print(C0);
  lcd.print("      ");
  lcd.setCursor(0, 0);
  lcd.print("PV=    ");
  lcd.print(PV);
  lcd.print("      ");
  lcd.setCursor(0, 1);
}
