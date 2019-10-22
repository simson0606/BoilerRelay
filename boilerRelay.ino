#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define BOILER_PIN  A0
#define REPORT_PIN  A1
#define CS_PIN      10

LiquidCrystal_I2C lcd(0x27, 16, 2); 

int potValue = 10;
int loops = 0;
int boilerRead = 0;
int reportRead = 0;

void setup() {
  pinMode(BOILER_PIN, INPUT);
  pinMode(REPORT_PIN, INPUT);
  pinMode(CS_PIN, OUTPUT);
  
  SPI.begin();
  
  lcd.begin();
  lcd.backlight();
}

void loop() {

  boilerRead = analogRead(BOILER_PIN);
  reportRead = analogRead(REPORT_PIN);

  if (boilerRead < 100) {
    potValue = 150; //safety
  } else {
    if (boilerRead > reportRead) {
      potValue++;
    } 
    if (boilerRead < reportRead) {
      potValue--;
    }
  
    if (potValue <= 0) potValue = 0;
    if (potValue >= 255) potValue = 255;
  }
  writeLcd();

  writePot(potValue);

  delay(10);
}

void writeLcd(){
  if (loops >= 100) {
    loops = 0;
    lcd.setCursor(0,0);
    lcd.print("Boiler");
     
    lcd.setCursor(13,0);
    lcd.print(boilerRead);
  
    lcd.setCursor(0,1);
    lcd.print("Report");
  
    lcd.setCursor(8,1);
    lcd.print(potValue);
      
    lcd.setCursor(13,1);
    lcd.print(reportRead);
  }
  loops++;
}

void writePot(int value) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(B00010011); 
  SPI.transfer(value);
  digitalWrite(CS_PIN, HIGH);
}
