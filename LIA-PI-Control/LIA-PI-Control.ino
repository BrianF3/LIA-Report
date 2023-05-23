// This code implements PI control using an LCD screen and pushbuttons for control adjustment

// Include LiquidCrystal library for managing the LCD screen
#include <LiquidCrystal.h>

// LCD screen pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pushbutton pins
const int PBU = 6;
const int PBD = 7;
const int PBL = 8;
const int PBR = 9;

// Sensor and actuator pins
int sensor = A1;
int actuator = 10;

// Control variables
int CO = 0;
int setPoint = 0;
int PV = 0;
int Error = 0;
float pGain = 0.0;
float iGain = 0.0;
int integralAction = 0;
float EXCO;

// Time variables for control interval
unsigned long previousMillis = 0;
const long interval = 100;
unsigned long currentMillis = 0;

// Button states and debounce variables
bool PBLstate, lastButtonStateL, PBRstate, lastButtonStateR, PBUstate, lastButtonStateU, PBDstate, lastButtonStateD = false;
unsigned long lastButtonPressL, lastButtonPressR, lastButtonPressU, lastButtonPressD = 0;
const unsigned long debounceDelay = 10;

// LCD page variable
int Screen = 0;

void setup() {
  // Initialize pin modes and serial communication
  pinMode(actuator, OUTPUT);
  pinMode(PBL, INPUT_PULLUP);
  pinMode(PBR, INPUT_PULLUP);
  pinMode(PBU, INPUT_PULLUP);
  pinMode(PBD, INPUT_PULLUP);
  Serial.begin(9600);

  // Initialize the LCD screen
  lcd.begin(16, 2);
}

void loop() {
  // Read sensor value
  PV = analogRead(sensor);

  // Calculate error and integral action
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Error = setPoint - PV;
    integralAction = (integralAction + Error);
    CO = ((pGain * Error) + (iGain * integralAction));

    // Limit the control output
    if (CO > 255) {
      CO = 255;
    }
    if (CO < 0) {
      CO = 0;
    }
  }

  // Convert control output to a scaled value
  EXCO = CO / 10;

  // Ensure error and scaled control output are within bounds
  if (Error < 0) {
    Error = 0;
  }
  if (EXCO < 0) {
    EXCO = 0;
  }
  if (EXCO > 255) {
    EXCO = 255;
  }

  // Apply control output to the actuator
  analogWrite(actuator, CO);

  // Display values on the serial monitor
  Serial.print(" PV:");
  Serial.print(PV);
  Serial.print("\t SP:");
  Serial.print(setPoint);
  Serial.print("\t CO =");
  Serial.print(CO);
  Serial.print("\t Error=");
  Serial.print(Error);
  Serial.print("\t pGain=");
  Serial.print(pGain);
  Serial.print("\t iGain:");
  Serial.println(iGain);

  // Debounce and button actions
  PBLstate = digitalRead(PBL);
  PBRstate = digitalRead(PBR);
  PBUstate = digitalRead(PBU);
  PBDstate = digitalRead(PBD);

  if (!PBLstate && lastButtonStateL && (millis() - lastButtonPressL) > debounceDelay) {
    // Left button pressed
    Screen--;
    if (Screen < 0) {
      Screen = 2;
    }
    lastButtonPressL = millis();
  }
  lastButtonStateL = PBLstate;

  if (!PBRstate && lastButtonStateR && (millis() - lastButtonPressR) > debounceDelay) {
    // Right button pressed
    Screen++;
    if (Screen > 2) {
      Screen = 0;
    }
    lastButtonPressR = millis();
  }
  lastButtonStateR = PBRstate;

  if (!PBUstate && lastButtonStateU && (millis() - lastButtonPressU) > debounceDelay) {
    // Up button pressed
    if (Screen == 0) {
      setPoint += 25;
    } else if (Screen == 1) {
      pGain += 0.01;
    } else if (Screen == 2) {
      iGain += 0.01;
    }
    lastButtonPressU = millis();
  }
  lastButtonStateU = PBUstate;

  if (!PBDstate && lastButtonStateD && (millis() - lastButtonPressD) > debounceDelay) {
    // Down button pressed
    if (Screen == 0) {
      setPoint -= 25;
    } else if (Screen == 1) {
      pGain -= 0.01;
    } else if (Screen == 2) {
      iGain -= 0.01;
    }
    lastButtonPressD = millis();
  }
  lastButtonStateD = PBDstate;

  // Update the LCD display based on the current page
  lcd.clear();
  switch (Screen) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("PV=");
      lcd.print(PV);
      lcd.setCursor(7, 0);
      lcd.print("Err=");
      lcd.print(Error);
      lcd.setCursor(0, 1);
      lcd.print("SP:");
      lcd.print(setPoint);
      break;
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("PV=");
      lcd.print(PV);
      lcd.setCursor(7, 0);
      lcd.print("Err=");
      lcd.print(Error);
      lcd.setCursor(0, 1);
      lcd.print("pGain=");
      lcd.print(pGain);
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("PV=");
      lcd.print(PV);
      lcd.setCursor(7, 0);
      lcd.print("Err=");
      lcd.print(Error);
      lcd.setCursor(0, 1);
      lcd.print("iGain:");
      lcd.print(iGain);
      break;
  }

  delay(100);
}

