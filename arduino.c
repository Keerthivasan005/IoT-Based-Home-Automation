#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN D3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

#define IRSensor D5  // connect ir sensor to arduino pin 2
//int LED = 13; // conect Led to arduino pin 13

#define FAN D8
#define LIGHT D7

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("DHT11 test!");

  dht.begin();

  pinMode (IRSensor, INPUT); // sensor pin INPUT
  // pinMode (LED, OUTPUT); // Led pin OUTPUT

  // initialize the LCD, 
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("IoT DIY PROJECT");
  lcd.print("Fall Detected");
  lcd.setCursor(3,1);
  lcd.print("SMART HOME");

  pinMode(FAN, OUTPUT);
  pinMode(LIGHT,OUTPUT);
     
  delay(5000);   

}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("C ");

  lcd.clear();

  // Code to measure the light intensity using IR Sensor 
  int statusSensor = digitalRead (IRSensor);

  if (statusSensor == 1)
  {
    //digitalWrite(LED, LOW); // LED LOW
    Serial.print("\t High Light Intensity");
    lcd.setCursor(0,0);
    lcd.print("Light-OFF");
    Serial.print("\t Light-OFF");
    digitalWrite(LIGHT, LOW);
  }
  
  else
  {
    //digitalWrite(LED, HIGH); // LED High
    Serial.print("\t Low Light Intensity");
    lcd.setCursor(0,0);
    lcd.print("Light-ON");
    Serial.print("\t Light-ON");
    digitalWrite(LIGHT, HIGH);
  }

  
  if(t > 32.0)
  {
    //lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Fan-ON");
    Serial.println("\t Fan-ON");
    digitalWrite(FAN,HIGH);
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("Fan-OFF");
    Serial.println("\t Fan-OFF");
    digitalWrite(FAN,LOW);
  }

  
 }
