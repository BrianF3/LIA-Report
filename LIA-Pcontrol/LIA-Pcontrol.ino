// P Control System with LCD Display and Pushbuttons

// Include LiquidCrystal library for managing the LCD screen
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pushbuttons
const int PBU = 6;
const int PBD = 7;
const int PBL = 8;
const int PBR = 9;

int sensor = A1; // Analog input pin for sensor
int actuator = 10; // Output pin for actuator
int PV = 0; // Process variable
int setPoint = 0; // Setpoint for the control system
int CO = 0; // Control output
float pGain = 0; // Proportional gain
int Error = 0; // Error between setpoint and process variable

// Button states and debounce variables
bool PBLstate, lastButtonStateL, PBRstate, lastButtonStateR, PBUstate, lastButtonStateU, PBDstate, lastButtonStateD = false;
unsigned long lastButtonPressL, lastButtonPressR, lastButtonPressU, lastButtonPressD = 0;
const unsigned long debounceDelay = 50;

// LCD page variable
int Screen = 0;

void setup() {
  pinMode(actuator, OUTPUT); // Set actuator pin as output
  Serial.begin(9600); // Initialize serial communication
  lcd.begin(16, 2); // Initialize LCD
  pinMode(PBL, INPUT_PULLUP); // Set pushbutton pins as input with pull-up resistors
  pinMode(PBR, INPUT_PULLUP);
  pinMode(PBU, INPUT_PULLUP);
  pinMode(PBD, INPUT_PULLUP);
}

void loop() {
  PV = analogRead(sensor); // Read the sensor value
  Error = setPoint - PV; // Calculate the error
  CO = pGain * Error; // Calculate the control output

  if (CO > 255) {
    CO = 255; // Limit the control output to 255
  }
  if (CO < 0) {
    CO = 0; // Limit the control output to 0
  }

  if (Error < 0) {
    Error = 0; // Set error to 0 if it is negative
  }
  
  analogWrite(actuator, CO); // Set the actuator value based on the control output

  PBLstate = digitalRead(PBL); // Read the state of the left pushbutton
  PBRstate = digitalRead(PBR); // Read the state of the right pushbutton
  PBUstate = digitalRead(PBU); // Read the state of the up pushbutton
  PBDstate = digitalRead(PBD); // Read the state of the down pushbutton

  Serial.print("SP: "); // Print the setpoint value
  Serial.print(setPoint);
  Serial.print("\tPV: "); // Print the process variable value
  Serial.print(PV);
  Serial.print("\tCO: "); // Print the control output value
  Serial.print(CO);
  Serial.print("\tError: "); // Print the error value
  Serial.print(Error);
  Serial.print("\tpGain: "); // Print the proportional gain value
  Serial.println(pGain);

  if (!PBLstate && lastButtonStateL && (millis() - lastButtonPressL) > debounceDelay) {
      Screen = 0; // Set the screen to display the first page
    lastButtonPressL = millis(); // Update the last button press time
  }
  lastButtonStateL = PBLstate; // Update the last button state

  if (!PBRstate && lastButtonStateR && (millis() - lastButtonPressR) > debounceDelay) {
    Screen = 1; // Set the screen to display the second page
    lastButtonPressR = millis(); // Update the last button press time
  }
  lastButtonStateR = PBRstate; // Update the last button state

  if (!PBUstate && lastButtonStateU && (millis() - lastButtonPressU) > debounceDelay) {
    if (Screen == 0) {
      setPoint += 25; // Increase the setpoint by 25 if on the first page
    } else if (Screen == 1) {
      pGain += 0.01; // Increase the proportional gain by 0.01 if on the second page
    }
    lastButtonPressU = millis(); // Update the last button press time
  }
  lastButtonStateU = PBUstate; // Update the last button state

  if (!PBDstate && lastButtonStateD && (millis() - lastButtonPressD) > debounceDelay) {
    if (Screen == 0) {
      setPoint -= 25; // Decrease the setpoint by 25 if on the first page
    } else if (Screen == 1) {
      pGain -= 0.01; // Decrease the proportional gain by 0.01 if on the second page
    }
    lastButtonPressD = millis(); // Update the last button press time
  }
  lastButtonStateD = PBDstate; // Update the last button state

  // Update the LCD display based on the current page
  lcd.clear();
  switch (Screen) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("PV=");
      lcd.print(PV);
      lcd.setCursor(8, 0);
      lcd.print("CO=");
      lcd.print(CO);
      lcd.setCursor(0, 1);
      lcd.print("SP = ");
      lcd.print(setPoint);
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("PV=");
      lcd.print(PV);
      lcd.setCursor(9, 0);
      lcd.print("CO=");
      lcd.print(CO);
      lcd.setCursor(0, 1);
      lcd.print("pGain = ");
      lcd.print(pGain);
      break;
  }

  delay(100); // Delay to avoid screen flickering
}
 
