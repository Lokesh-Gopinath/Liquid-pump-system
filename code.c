#define BLYNK_TEMPLATE_ID           "Your template ID"
#define BLYNK_TEMPLATE_NAME         "Your template name"
#define BLYNK_AUTH_TOKEN            "Your Blynk Token"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

const char auth[] = "Your auth";
const char ssid[] = "Your WIFI name";
const char pass[] = "Your WIFI password";

#define DHTPIN D4         
#define RELAY_PIN D1      
#define DHTTYPE DHT11   

float flowRate = 0;
float measuredVoltage =0;
float calculatedCurrent =0;

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

BLYNK_WRITE(V4)
{
  int relayState = param.asInt();

  if (relayState == 0)
  {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("Relay is ON");
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Relay is OFF");
  }
}

void sendSensor()
{
  float t = dht.readTemperature(); 

  Blynk.virtualWrite(V0, t);

  if(t >= 35){
    Blynk.logEvent("pump_overheat", String("Pump-over Heat Detected!"));
  }
  else{

  }
}

void sendSensorData()
{
  measuredVoltage = analogRead(A0) * (3.3 / 1023.0); 
  calculatedCurrent = (measuredVoltage / 1833) * 1000;
  
  Blynk.virtualWrite(V1, measuredVoltage);
  Blynk.virtualWrite(V2, calculatedCurrent);

}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, sendSensorData);
}

void loop()
{
  Blynk.run();
  flowRate = (measuredVoltage * 26) / 60;
  Blynk.virtualWrite(V3, flowRate);
  timer.run();
}