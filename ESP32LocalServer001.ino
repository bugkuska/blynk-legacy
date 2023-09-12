#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "";      //ชื่อ SSID ที่เราต้องการเชื่อมต่อ
char pass[] = "";  //รหัสผ่าน WI-FI
char auth[] = "";            //Auth token from blynk app

#include <SimpleTimer.h>
SimpleTimer timer;

//LDR
#define INPUT_1 35
//SHCT3 Humidity and Temperature Sensor
#include <Arduino.h>
#include "SHTC3.h"
SHTC3 s(Wire);
int temp = 0;  //Variables to store sensor readings
int humid = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  delay(500);

  Wire.begin();

  //Connect to Blynk Server
  //Blynk.begin(auth, ssid, pass, "192.168.1.200", 8080);
  Blynk.begin(auth, ssid, pass, "iotservices.thddns.net", 5536);
  timer.setInterval(1000L, sht3Data);
}

void loop() {
  Blynk.run();
  timer.run();
}


//SHCT3
void sht3Data() {
  int Temp;
  int Humid;
  s.begin(true);
  s.sample();
  Temp = s.readTempC();
  Humid = s.readHumidity();
  Serial.print(F("[SHTC3] T:"));
  Serial.print(s.readTempC());
  Serial.print(F(" C? / H: "));
  Serial.print(s.readHumidity());
  Serial.println(F(" %"));
  delay(2000);
  Blynk.virtualWrite(V1, s.readHumidity());
  Blynk.virtualWrite(V2, s.readTempC());
}
