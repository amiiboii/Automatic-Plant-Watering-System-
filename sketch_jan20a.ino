#include <LiquidCrystal.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);  

byte topPlant[8] = {
  B00000,
  B01110,
  B01010,
  B01110,
  B00100,
  B10101,
  B01110,
  B00100
};

byte can[8] = {
  B00000,
  B11100,
  B01000,
  B01000,
  B11100,
  B11110,
  B11101,
  B11100
};
byte a[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00100
};

byte a2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B00100,
  B00100
};

byte a3[8] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110
};
byte a4[8] = {
  B00000,
  B00000,
  B00000,
  B01110,
  B00100,
  B00100,
  B10101,
  B01110
};
byte a5[8] = {
  B00000,
  B00000,
  B01010,
  B01110,
  B00100,
  B00100,
  B10101,
  B01110
};
byte a6[8] = {
  B00000,
  B01110,
  B01010,
  B01110,
  B00100,
  B00100,
  B10101,
  B01110
};

const int buttonPin = 9;    // Pin f12r the push button
const int buttonPin2 = 8;  
const int ledPin = 5;      // Pin for the LED
const int onled = 7;   
int relay1 = 2;

int raiddrop = A3;
int Soil1 = A2;
int Soil2 = A1;
int Soil3 = A0;

int buttonState;        // Variable to store the button state
int buttonState2; 

bool rained = false; // Variable to track if it has rained
bool watering_now = 0;
int times_water = 0;

unsigned long lastRainCheck = 0; // Variable to track the last time the sensor was checked
const unsigned long checkInterval = 86400000; // Interval for checking the sensor (24 hours in milliseconds)

const int raindrop_tesh = 700;
const int soil1_tesh = 450;


void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);  

  pinMode(raiddrop, INPUT);
  pinMode(Soil1, INPUT);    

  pinMode(ledPin, OUTPUT);
  pinMode(relay1, OUTPUT);
  Serial.begin(9600);

  lcd.init();            
  lcd.backlight();
  lcd.createChar(1, topPlant);
  lcd.createChar(2, can);
  lcd.createChar(3, a);
  lcd.createChar(4, a2);
  lcd.createChar(5, a3);
  lcd.createChar(6, a4);
  lcd.createChar(7, a5);
  lcd.createChar(8, a6);

}

void loop() {
  buttonState = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin2);
  int sensorValue = analogRead(raiddrop);
  Serial.println(analogRead(A2));

  digitalWrite(onled, HIGH);  
  digitalWrite(ledPin, HIGH);    

  digitalWrite(relay1, HIGH);

  if (sensorValue < raindrop_tesh) {
      rained = true;
      lastRainCheck = millis();
  } else if (millis() - lastRainCheck >= checkInterval) {
      rained = false;
  }

  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(relay1, LOW);
    watering_now = 1;

    if (digitalRead(relay1) == HIGH){
      times_water++;
    }
  
  } else {
    if (rained == false){
      digitalWrite(ledPin, LOW);
      digitalWrite(relay1, LOW);

      if (analogRead(Soil1) < soil1_tesh){
        digitalWrite(ledPin, HIGH);
        digitalWrite(relay1, HIGH);
        watering_now = 1;
        times_water++;
      }
    }
  }

 
  // Serial.println(watering_now);
  if (watering_now == 1){
    digitalWrite(ledPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(1); 
    lcd.setCursor(4,0);
    lcd.print("A.P.W.SYSTEM");
    lcd.setCursor(19,0);
    lcd.write(1);
    lcd.setCursor(0,1);
    lcd.print("---------------");  
    lcd.setCursor(0,2);
    lcd.print("Watering");
    lcd.print("      ");  

    for (int i =2; i <= 8; i++) {
      if (i == 7 || i == 8) {
        for (int j = 0; j < 2; j++) {
          lcd.setCursor(i-1 + (i-1), 3); 
          lcd.write(i); 
          delay(50);
        }
      } else {
        lcd.setCursor(i-1 + (i-1), 3); 
        lcd.write(i); 
        delay(50);
      }
    }
  }

  if (buttonState2 == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(1); 
    lcd.setCursor(4,0);
    lcd.print("A.P.W.SYSTEM");
    lcd.setCursor(19,0);
    lcd.write(1);
    lcd.setCursor(0,1);
    lcd.print("------General-------");  
    lcd.setCursor(0,2);
    lcd.print("Times Watered:");
    lcd.print(times_water);  
    lcd.print("      ");  
    lcd.setCursor(0,3);
    lcd.print("Rained today:");

    if (rained == true){
      lcd.print("Yes");      
    }else{
      lcd.print("No");     
    }
    delay(200);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(1); 
    lcd.setCursor(4,0);
    lcd.print("A.P.W.SYSTEM");
    lcd.setCursor(19,0);
    lcd.write(1);
    lcd.setCursor(0,1);
    lcd.print("---Sensor Readings--");  
    lcd.setCursor(0,2);
    lcd.print("S1:");
    lcd.print(analogRead(Soil1));      
    lcd.setCursor(7,2);
    lcd.print("|~~~~");
    lcd.setCursor(12,2);
    lcd.print("|S2:");
    lcd.print(analogRead(Soil2)); 
    lcd.setCursor(0,3);
    lcd.print("Raindrop:");
    lcd.print(analogRead(raiddrop));
    lcd.print("    ");

    delay(2000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(1); 
    lcd.setCursor(4,0);
    lcd.print("A.P.W.SYSTEM");
    lcd.setCursor(19,0);
    lcd.write(1);
    lcd.setCursor(0,1);
    lcd.print("-----Thresholds-----");  
    lcd.setCursor(0,2);
    lcd.print("Soil:");
    lcd.print(soil1_tesh);      
    lcd.setCursor(0,3);
    lcd.print("Raindrop:");
    lcd.print(raindrop_tesh);
    lcd.print("    ");

    delay(2000);
  }
  // delay();
  watering_now = 0;
}