#include <Wire.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>

const int sensorPin = A0;
const int sensorPower = 7;
const int waterPin = 10;
const int touchPin = 8;

String time;
String date;
//set to current moisture level
int sensorVal;

//calibrated through trial and error
int threshold = 600;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  // put your setup code here, to run once:
  pinMode(sensorPower, OUTPUT);
  pinMode(waterPin, OUTPUT);
  pinMode(touchPin, INPUT);
  Serial.begin(9600);

  lcd.begin();
	lcd.backlight();
}

void loop() {
  //while dry & not being overridden
  while((sensorRead() > threshold) && (digitalRead(touchPin) == LOW)){
    lcd.clear();
    lcd.setCursor(0, 1);
    digitalWrite(waterPin, LOW);
    lcd.print("Last watered: " + String(time) + ", " + String(date));
    for(int pos = 0; pos < 35; pos++){
      lcd.scrollDisplayLeft();
      delay(150);
    }
  }
  //while wet or being overridden
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Watering...");
  digitalWrite(waterPin, HIGH);
  time = String(__TIME__);
  date = String(__DATE__);
  delay(1);
}

int sensorRead(){
  //turn on sensor
  digitalWrite(sensorPower, HIGH);
  delay(10);
  //collects 100 samples
  for(int i = 0; i <= 100; i++){
    sensorVal += analogRead(sensorPin);
    delay(1);
  }
  //turn off sensor
  digitalWrite(sensorPower, LOW);
  //averages the samples
  sensorVal = sensorVal/100.0;
  return sensorVal;
}
