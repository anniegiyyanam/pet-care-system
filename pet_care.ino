#define BLYNK_TEMPLATE_ID           "TMPL3z_uY9e9w"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "fxX3SYWAVZ7sMUocy86U8fJo7imkr46a"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <ESP32_Servo.h> 

static const int servoPin = 14;
Servo servo1;

// Your WiFi credentials
 
//char ssid[] = "Don't connect";
//char pass[] = "hillpark";
char ssid[] = "Nikitha"; 
char pass[] = "Tanay123"; 

#define VIRTUAL_PIN0 V0
#define VIRTUAL_PIN1 V1
#define VIRTUAL_PIN2 V2
#define VIRTUAL_PIN3 V3
#define VIRTUAL_PIN4 V4

#define Food_Level_1 32
#define Food_Level_2 35

#define water_Level_1 13
#define water_Level_2 34
#define water_Level_3 33

#define DHTPIN 27

#define PumpPin 26

#define DHTTYPE DHT11 
DHT dht (DHTPIN, DHTTYPE);

// variable
int pumpState = 0;
int Servo_State = 0;
int Food_sw_status=0;
int Water_sw_status=0;

int Food_Level_Stat1 = 0;
int Food_Level_Stat2 = 0;

int Water_Level_Stat1 = 0;
int Water_Level_Stat2 = 0;
int Water_Level_Stat3 = 0;

int temperature = 0;

BlynkTimer timer;

// Function to send data to Blynk
void sendDataToBlynk(){

  Blynk.virtualWrite(VIRTUAL_PIN0, temperature);
  Blynk.virtualWrite(VIRTUAL_PIN1, Food_sw_status);
  Blynk.virtualWrite(VIRTUAL_PIN2, Water_sw_status);
}

BLYNK_WRITE(VIRTUAL_PIN3){
  if(param.asInt() == 1) Servo_State=1;
  else Servo_State=0;
}

BLYNK_WRITE(VIRTUAL_PIN4){
  if(param.asInt() == 1) pumpState=1;
  else pumpState=0;
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  WiFi.begin(ssid, pass);
  pinMode(Food_Level_1, INPUT);
  pinMode(Food_Level_2, INPUT);

  pinMode(water_Level_1, INPUT);
  pinMode(water_Level_2, INPUT);
  pinMode(water_Level_3, INPUT);

  pinMode(2, OUTPUT);
  pinMode(PumpPin, OUTPUT);
  digitalWrite(PumpPin,LOW);

  servo1.attach(servoPin);
  dht.begin();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
    digitalWrite(2,LOW);
  }
  Serial.println("Connected to WiFi");
  digitalWrite(2,HIGH);
  timer.setInterval(1000L, sendDataToBlynk);
}

void loop() {
  Blynk.run();
  timer.run();

  Food_Level_Stat1 = digitalRead(Food_Level_1);
  Food_Level_Stat2 = digitalRead(Food_Level_2);

  Water_Level_Stat1 = digitalRead(water_Level_1);
  Water_Level_Stat2 = digitalRead(water_Level_2);
  Water_Level_Stat3 = digitalRead(water_Level_3);

  temperature = dht.readTemperature();
  temperature = temperature;

  if (Food_Level_Stat1 == 0 & Food_Level_Stat2 == 1) Food_sw_status= 1;
  else if (Food_Level_Stat1 == 1 & Food_Level_Stat2 == 1)Food_sw_status= 0;
  else if (Food_Level_Stat1 == 0 & Food_Level_Stat2 == 0) Food_sw_status= 2;

  if (Water_Level_Stat1 == 0 & Water_Level_Stat2 == 0 & Water_Level_Stat3 == 0) Water_sw_status= 0;
  else if (Water_Level_Stat1 == 1 & Water_Level_Stat2 == 0 & Water_Level_Stat3 == 0)Water_sw_status= 1;
  else if (Water_Level_Stat1 == 1 & Water_Level_Stat2 == 1 & Water_Level_Stat3 == 0)Water_sw_status= 2;
  else if (Water_Level_Stat1 == 1 & Water_Level_Stat2 == 1 & Water_Level_Stat3 == 1)Water_sw_status= 3;

  if (Servo_State==0) servo1.write(0);
  else servo1.write(180);

  if (pumpState==0)digitalWrite(PumpPin,LOW);
  else digitalWrite(PumpPin,HIGH);
}