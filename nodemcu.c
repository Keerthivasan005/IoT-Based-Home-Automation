#define BLYNK_PRINT Serial

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "6cB37uo_p-1nLbpdEjnn951Cg6QwmLz7";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "NSK_Res";
char pass[] = "Karthik6363";

int pinValue0, pinValue1, pinValue2;

#define DHTPIN D3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

#define IRSensor D5

#define LIGHT D7
#define FAN D8

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // Debug console
  Serial.begin(115200);

  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode(FAN,OUTPUT);
  digitalWrite(FAN,LOW);
  pinMode(LIGHT,OUTPUT);
  digitalWrite(LIGHT,LOW);  

  dht.begin();

  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.clear();
  lcd.setCursor (0,0); 
  lcd.print("IoT DIY PROJECT");
  lcd.setCursor(0,1);
  lcd.print("HOME AUTOMATION"); 

  delay(3000);
  
  Blynk.begin(auth, ssid, pass);
}

BLYNK_WRITE(V2)
{
  pinValue0 = param.asInt(); // assigning incoming value from pin V2 to a variable
}
BLYNK_WRITE(V3)
{
  pinValue1 = param.asInt(); // assigning incoming value from pin V3 to a variable
}
BLYNK_WRITE(V4)
{
  pinValue2 = param.asInt(); // assigning incoming value from pin V4 to a variable
}

void loop()
{
  int fan_status = digitalRead(FAN);
  int light_status = digitalRead(LIGHT);        // Variables to find the status of devices

  if (pinValue0 == 0)               // Check if Mode selected is Manual Mode
  {
    if (pinValue1 == 1)             // Check if FAN Switch is ON
      digitalWrite(FAN,HIGH);
    else
      digitalWrite(FAN,LOW);

    if (pinValue2 == 1)             // Check if LIGHT Switch is ON
      digitalWrite(LIGHT,HIGH);
    else
      digitalWrite(LIGHT,LOW);
  }
  else
  {
    int statusSensor = digitalRead (IRSensor);  
    Serial.print("Light Intensity: ");
    Serial.print(statusSensor);
    Serial.print("\t");
    if (statusSensor == 0)          // Check if Light Intensity is LOW
      digitalWrite(LIGHT,HIGH);
    else
      digitalWrite(LIGHT,LOW);


    float t = dht.readTemperature();
    if (isnan(t))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print("\t");
    if(t > 32)                  // Check if temperature is HIGH
      digitalWrite(FAN,HIGH);
    else
      digitalWrite(FAN,LOW);
  }

  lcd.clear();
  lcd.setCursor(0,0);
  
  if (light_status == 1)
  {
    Serial.print("Light - ON");
    lcd.print("Light - ON");
  }
  else
  {
    Serial.print("Light - OFF");
    lcd.print("Light - OFF");
  }
  
  lcd.setCursor(0,1);
  
  if (fan_status == 1)
  {
    Serial.println("\t Fan - ON");
    lcd.print("Fan - ON");
  }
  else
  {
    Serial.println("\t Fan - OFF");
    lcd.print("Fan - OFF");
  }
  delay(1000);
  Blynk.run();
}
