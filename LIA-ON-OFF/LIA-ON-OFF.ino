/*
   On-Off Control
   
   This code implements On-Off control with hysteresis. The actuator is controlled based on a setpoint and the sensor value.
   If the sensor value is below the setpoint minus the hysteresis, the actuator is turned on fully.
   If the sensor value is above the setpoint plus the hysteresis, the actuator is turned off completely.
   The setpoint and hysteresis values can be adjusted using push buttons.
   The sensor value, control signal, setpoint, hysteresis, and current page are displayed on an LCD.
*/

#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int Sensor = A1;            // Analog input pin for the sensor
int Actuator = 10;             // Pin for the actuator
int PV = 0;   // Process variable (sensor value)
int InputValue = 0; // Input value from user
int inputpin = A0; // Analog input pin for user input
int C0 = 0; // Control signal
int setpoint = 0; // Setpoint value
int HYS = 100; // Hysteresis value

int Varisp;
int VariHYS;

// Pushbuttons
const int buttonPin1 = 6;
const int buttonPin2 = 7;
const int buttonPin3 = 8;
const int buttonPin4 = 9;

// Button states and debounce variables
bool buttonState1, lastButtonState1, buttonState2, lastButtonState2, buttonState3, lastButtonState3, buttonState4, lastButtonState4 = false;
unsigned long lastButtonPress1, lastButtonPress2, lastButtonPress3, lastButtonPress4 = 0;
const unsigned long debounceDelay = 50;

// LCD page variable
int currentPage = 0;

void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);

  Serial.begin(9600);     // Initializing serial communication.
  pinMode(Actuator, OUTPUT);   // Defining actuator pin as output.
  lcd.begin(16, 2);  // Set up the LCD's number of columns and rows
}

void loop() {
  PV = analogRead(Sensor); 

  if (PV <= (setpoint - HYS)) {
    C0 = 255;
  } else if (PV >= (setpoint + HYS)) {
    C0 = 0;
  }
  analogWrite(Actuator, C0);
  
  // Debounce and button actions
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);

  Serial.print("SP:");
  Serial.print(setpoint);
  Serial.print("\tPV:");
  Serial.print(PV);
  Serial.print("\tC0 = ");
  Serial.print(C0);
  Serial.print("\tHYS = ");
  Serial.print(HYS);
  Serial.print("\tPage:");
  Serial.println(currentPage);

  if (!buttonState1 && lastButtonState1 && (millis() - lastButtonPress1) > debounceDelay) {
    currentPage--;
    if (currentPage < 0) currentPage = 1;
    lastButtonPress1 = millis();
  }
  lastButtonState1 = buttonState1;

  if (!buttonState2 && lastButtonState2 && (millis() - lastButtonPress2) > debounceDelay) {
    currentPage++;
    if (currentPage > 1) currentPage = 0;
    lastButtonPress2 = millis();
  }
  lastButtonState2 = buttonState2;

  if (!buttonState3 && lastButtonState3 && (millis() - lastButtonPress3) > debounceDelay) {
    if (currentPage == 0) {
      Varisp = setpoint + 20;
      setpoint = Varisp;
    } else if (currentPage == 1) {
      VariHYS = HYS + 10;
      HYS = VariHYS;
    }
    lastButtonPress3 = millis();
  }
  lastButtonState3 = buttonState3;

  if (!buttonState4 && lastButtonState4 && (millis() - lastButtonPress4) > debounceDelay) {
    if (currentPage == 0) {
      Varisp = setpoint - 20;
      setpoint = Varisp;
    } else if (currentPage == 1) {
      VariHYS = HYS - 10;
      HYS = VariHYS;
    }
    lastButtonPress4 = millis();
  }
  lastButtonState4 = buttonState4;

  // Update the LCD display based on currentPage
  lcd.clear();
  switch (currentPage) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("SP: ");
      lcd.print(setpoint);
      lcd.print("      ");
      lcd.setCursor(0, 1);
      lcd.print("C0:");
      lcd.print(C0);
      lcd.print("  inp:");
      lcd.print(InputValue);
      lcd.print("      ");
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Hys: ");
      lcd.print(HYS);
      lcd.print("      ");
      lcd.setCursor(0, 1);
      lcd.print("C0:");
      lcd.print(C0);
      lcd.print("  inp:");
      lcd.print(InputValue);
      lcd.print("      ");
      break;
  }

  // Delay to avoid screen flicker
  delay(100);
}
