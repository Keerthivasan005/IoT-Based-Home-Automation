#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp8266.h>

#define DHTPIN D3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

#define IRSensor D5  // connect ir sensor to arduino pin 2
//int LED = 13; // conect Led to arduino pin 13

#define FAN D8
#define LIGHT D7
#define AUTOMATIC D6
#define BLYNK_PRINT Serial
// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);
char auth[] = "6cB37uo_p-1nLbpdEjnn951Cg6QwmLz7";

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "WIFI";
char pass[] = "PASSWORD";

int pinValue0, pinValue1, pinValue2;

BlynkTimer timer;

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
  lcd.setCursor(3,1);
  lcd.print("SMART HOME");

  pinMode(FAN, OUTPUT);
  pinMode(LIGHT,OUTPUT);
  
  delay(5000);   

}

BLYNK_WRITE(V2) // V2 is the number of Virtual Pin  
{
  pinValue0 = param.asInt();
  //Serial.println(pinValue0);
}

BLYNK_WRITE(V3) // V3 is the number of Virtual Pin  
{
  pinValue1 = param.asInt();
  //Serial.println(pinValue);
}

BLYNK_WRITE(V4) // V4 is the number of Virtual Pin  
{
  pinValue2 = param.asInt();
  //Serial.println(pinValue2);
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

  int statusSensor = digitalRead (IRSensor);
  int fan = digitalRead(FAN);
  int light = digitalRead(LIGHT);
  int automatic = digitalRead(AUTOMATIC);
  
  // Code to measure the light intensity using IR Sensor
  if (automatic == 1)
  {
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
  
  else
  {
    if (fan == 1)
    {
      digitalWrite(FAN,HIGH);
      lcd.setCursor(0,1);
      lcd.print("Fan-ON");
      Serial.println("\t Fan-ON");
    }
    else
    {
      digitalWrite(FAN,LOW);
      lcd.setCursor(0,1);
      lcd.print("Fan-OFF");
      Serial.println("\t Fan-OFF");
    }
    
    if (light == 1)
    {
      digitalWrite(LIGHT,HIGH);
      lcd.setCursor(0,0);
      lcd.print("Light-ON");
      Serial.print("\t Light-ON");
    }
    else
    {
      digitalWrite(LIGHT,LOW);
      lcd.setCursor(0,0);
      lcd.print("Light-OFF");
      Serial.print("\t Light-OFF");
    }
  }
  Blynk.run();
  timer.run();
}
